/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
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

struct bundle_activator {
	service_tracker_pt queueTracker;
	service_tracker_pt dataStoreTracker;
	processor_pt processor;
};

typedef struct bundle_activator* bundle_activator_pt;

celix_status_t bundleActivator_create(bundle_context_pt context, void **userData) {
	celix_status_t status = CELIX_SUCCESS;

	bundle_activator_pt activator = (bundle_activator_pt) calloc(1, sizeof(*activator));

	if (activator) {
		activator->queueTracker = NULL;
		activator->dataStoreTracker = NULL;

		status = processor_create(&activator->processor);

		*userData = activator;
	} else {
		status = CELIX_ENOMEM;
	}

	return status;
}

celix_status_t bundleActivator_start(void * userData, bundle_context_pt context) {
	celix_status_t status = CELIX_SUCCESS;
	service_tracker_customizer_pt queueCustomizer = NULL;
	service_tracker_customizer_pt dataStoreCustomizer = NULL;

	bundle_activator_pt activator = (bundle_activator_pt) userData;

	printf("PROCESSOR: Starting bundle...\n");


	/*Track QueueService*/
	serviceTrackerCustomizer_create(activator->processor, NULL, processor_queueServiceAdded, NULL, processor_queueServiceRemoved, &queueCustomizer);
	serviceTracker_create(context, INAETICS_DEMONSTRATOR_API__SAMPLE_QUEUE_SERVICE_NAME, queueCustomizer, &activator->queueTracker);
	serviceTracker_open(activator->queueTracker);

	/*Track DataStoreService*/
	serviceTrackerCustomizer_create(activator->processor, NULL, processor_dataStoreServiceAdded, NULL, processor_dataStoreServiceRemoved, &dataStoreCustomizer);
	serviceTracker_create(context, INAETICS_DEMONSTRATOR_API__DATA_STORE_SERVICE_NAME, dataStoreCustomizer, &activator->dataStoreTracker);
	serviceTracker_open(activator->dataStoreTracker);

	return status;
}

celix_status_t bundleActivator_stop(void * userData, bundle_context_pt context) {
	celix_status_t status = CELIX_SUCCESS;
	bundle_activator_pt activator = userData;

	printf("PROCESSOR: Stopping bundle...\n");

	processor_stop(activator->processor);
	serviceTracker_close(activator->queueTracker);
	serviceTracker_close(activator->dataStoreTracker);

	return status;
}

celix_status_t bundleActivator_destroy(void * userData, bundle_context_pt context) {
	celix_status_t status = CELIX_SUCCESS;
	bundle_activator_pt activator = userData;

	serviceTracker_destroy(activator->queueTracker);
	serviceTracker_destroy(activator->dataStoreTracker);

	processor_destroy(activator->processor);

	free(activator);

	return status;
}
