
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

#include "inaetics_demonstrator_api/sample_queue.h"
#include "producer_impl.h"

celix_status_t queueServiceAdded(void *handle, service_reference_pt reference, void *service);
celix_status_t queueServiceRemoved(void *handle, service_reference_pt reference, void *service);

celix_status_t bundleActivator_create(bundle_context_pt context, void **userData) {
	celix_status_t status = CELIX_SUCCESS;
	struct activator *activator  = calloc(1, sizeof(*activator));

	if (activator) {
		activator->tracker = NULL;
		activator->queueServices = NULL;
		activator->running = false;
		pthread_mutex_init(&activator->queueLock, NULL);
		pthread_cond_init(&activator->queueAvailable, NULL);

		*userData = activator;
	} else {
		status = CELIX_ENOMEM;
	}
	return status;
}

celix_status_t bundleActivator_start(void * userData, bundle_context_pt context) {
	celix_status_t status = CELIX_SUCCESS;
	struct activator *activator = userData;

	printf("PRODUCER: Starting bundle...\n");

	// create list for services
	arrayList_create(&activator->queueServices);
	// start the thread
	activator->running = true;
	int rc = pthread_create(&activator->worker, NULL, produceSamples, activator);

	if (rc) {
		status = CELIX_BUNDLE_EXCEPTION;
	} else {
		service_tracker_customizer_pt customizer = NULL;
		serviceTrackerCustomizer_create(userData, NULL, queueServiceAdded, NULL, queueServiceRemoved, &customizer);

		serviceTracker_create(context, INAETICS_DEMONSTATOR_API__SAMPLE_QUEUE_SERVICE_NAME, customizer,
				&activator->tracker);
		serviceTracker_open(activator->tracker);
	}

	return status;
}

celix_status_t bundleActivator_stop(void * userData, bundle_context_pt context) {
	celix_status_t status = CELIX_SUCCESS;
	struct activator *activator = userData;

	printf("PRODUCER: Stopping bundle...\n");

	// stop the thread
	activator->running = false;
	void *exitStatus = NULL;
	pthread_join(activator->worker, &exitStatus);

	serviceTracker_close(activator->tracker);
	serviceTracker_destroy(activator->tracker);

	// destroy the list of services
	pthread_mutex_lock(&activator->queueLock);
	arrayList_destroy(activator->queueServices);
	pthread_mutex_unlock(&activator->queueLock);

	return status;
}

celix_status_t bundleActivator_destroy(void * userData, bundle_context_pt context) {
	celix_status_t status = CELIX_SUCCESS;
	struct activator *activator = userData;

	pthread_mutex_destroy(&activator->queueLock);
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

	printf("PRODUCER: Queue Service Added.\n");

	return CELIX_SUCCESS;
}

celix_status_t queueServiceRemoved(void *handle, service_reference_pt reference, void *service)
{
	struct activator *activator = handle;

	pthread_mutex_lock(&activator->queueLock);
	arrayList_removeElement(activator->queueServices, service);
	pthread_mutex_unlock(&activator->queueLock);

	printf("PRODUCER: Queue Service Removed.\n");

	return CELIX_SUCCESS;
}

