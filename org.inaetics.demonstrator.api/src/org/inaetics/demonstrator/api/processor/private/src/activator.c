#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>

#include <stdlib.h>
#include <bundle_activator.h>
#include <service_tracker.h>
#include <constants.h>
#include <pthread.h>
#include <stdbool.h>

#include <array_list.h>

#include "processor_impl.h"

celix_status_t queueServiceAdded(void *handle, service_reference_pt reference, void *service);
celix_status_t queueServiceRemoved(void *handle, service_reference_pt reference, void *service);

celix_status_t dataStoreServiceAdded(void *handle, service_reference_pt reference, void *service);
celix_status_t dataStoreServiceRemoved(void *handle, service_reference_pt reference, void *service);

celix_status_t bundleActivator_create(bundle_context_pt context, void **userData) {
        celix_status_t status = CELIX_SUCCESS;

        struct activator *activator  = calloc(1, sizeof(*activator));

        if (activator) {
                /*QueueService stuffs*/
                activator->queueTracker= NULL;
                activator->queueServices = NULL;
                pthread_mutex_init(&activator->queueLock, NULL);
                pthread_cond_init(&activator->queueAvailable, NULL);
                /*DataStore stuffs*/
                activator->dataStoreTracker= NULL;
                activator->dataStoreServices = NULL;
                pthread_mutex_init(&activator->dataStoreLock, NULL);
                pthread_cond_init(&activator->dataStoreAvailable, NULL);

                activator->running = false;

                *userData = activator;
        } else {
		status = CELIX_ENOMEM;
	}

	return status;
}

celix_status_t bundleActivator_start(void * userData, bundle_context_pt context) {
	celix_status_t status = CELIX_SUCCESS;
	struct activator *activator = userData;

	printf("PROCESSOR: Starting bundle...\n");

        // create list for services
        arrayList_create(&activator->queueServices);
        arrayList_create(&activator->dataStoreServices);
        // start the thread
        activator->running = true;
        int rc = pthread_create(&activator->worker, NULL, processSamples, activator);
        if (rc) {
                status = CELIX_BUNDLE_EXCEPTION;
        } else {
                /*Track QueueService*/
                service_tracker_customizer_pt queueCustomizer = NULL;
                serviceTrackerCustomizer_create(userData, NULL, queueServiceAdded, NULL, queueServiceRemoved, &queueCustomizer);
                serviceTracker_create(context, INAETICS_DEMONSTATOR_API__SAMPLE_QUEUE_SERVICE_NAME, queueCustomizer, &activator->queueTracker);
                serviceTracker_open(activator->queueTracker);

                /*Track DataStoreService*/
                service_tracker_customizer_pt dataStoreCustomizer = NULL;
                serviceTrackerCustomizer_create(userData, NULL, dataStoreServiceAdded, NULL, dataStoreServiceRemoved, &dataStoreCustomizer);
                serviceTracker_create(context, INAETICS_DEMONSTRATOR_API__DATA_STORE_SERVICE_NAME, dataStoreCustomizer, &activator->dataStoreTracker);
                serviceTracker_open(activator->dataStoreTracker);

        }

        return status;
}

celix_status_t bundleActivator_stop(void * userData, bundle_context_pt context) {
	celix_status_t status = CELIX_SUCCESS;
	struct activator *activator = userData;

	printf("PROCESSOR: Stopping bundle...\n");
	// stop the thread
	activator->running = false;
        void *exitStatus = NULL;
        pthread_join(activator->worker, &exitStatus);

        serviceTracker_close(activator->queueTracker);
        serviceTracker_destroy(activator->queueTracker);

        serviceTracker_close(activator->dataStoreTracker);
        serviceTracker_destroy(activator->dataStoreTracker);

        // destroy the list of services
        pthread_mutex_lock(&activator->queueLock);
        arrayList_destroy(activator->queueServices);
        pthread_mutex_unlock(&activator->queueLock);

        pthread_mutex_lock(&activator->dataStoreLock);
        arrayList_destroy(activator->dataStoreServices);
        pthread_mutex_unlock(&activator->dataStoreLock);

        return status;
}

celix_status_t bundleActivator_destroy(void * userData, bundle_context_pt context) {
        celix_status_t status = CELIX_SUCCESS;
        struct activator *activator = userData;

        pthread_cond_destroy(&activator->queueAvailable);
        pthread_mutex_destroy(&activator->queueLock);

        pthread_cond_destroy(&activator->dataStoreAvailable);
        pthread_mutex_destroy(&activator->dataStoreLock);

        free(activator);

        return status;
}

celix_status_t queueServiceAdded(void *handle, service_reference_pt reference, void *service)
{
	struct activator *activator = handle;

	pthread_mutex_lock(&activator->queueLock);
	arrayList_add(activator->queueServices, service);
	pthread_cond_signal(&activator->queueAvailable);
	pthread_mutex_unlock(&activator->queueLock);

	printf("PROCESSOR: Queue Service Added.\n");

	return CELIX_SUCCESS;
}

celix_status_t queueServiceRemoved(void *handle, service_reference_pt reference, void *service)
{
	struct activator *activator = handle;

	pthread_mutex_lock(&activator->queueLock);
	arrayList_removeElement(activator->queueServices, service);
	pthread_mutex_unlock(&activator->queueLock);

	printf("PROCESSOR: Queue Service Removed.\n");

        return CELIX_SUCCESS;
}

celix_status_t dataStoreServiceAdded(void *handle, service_reference_pt reference, void *service)
{
        struct activator *activator = handle;

        pthread_mutex_lock(&activator->dataStoreLock);
        arrayList_add(activator->dataStoreServices, service);
        pthread_cond_signal(&activator->dataStoreAvailable);
        pthread_mutex_unlock(&activator->dataStoreLock);

        printf("PROCESSOR: DataStore Service Added.\n");

        return CELIX_SUCCESS;
}

celix_status_t dataStoreServiceRemoved(void *handle, service_reference_pt reference, void *service)
{
        struct activator *activator = handle;

        pthread_mutex_lock(&activator->dataStoreLock);
        arrayList_removeElement(activator->dataStoreServices, service);
        pthread_mutex_unlock(&activator->dataStoreLock);

        printf("PROCESSOR: DataStore Service Removed.\n");

        return CELIX_SUCCESS;
}

