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

#include "inaetics_demonstrator_api/sample_queue.h"
#include "producer_impl.h"

struct bundle_activator {
	service_tracker_pt tracker;
	producer_pt producer;
};

typedef struct bundle_activator* bundle_activator_pt;

celix_status_t bundleActivator_create(bundle_context_pt context, void **userData) {
	celix_status_t status = CELIX_SUCCESS;
	bundle_activator_pt activator = (bundle_activator_pt) calloc(1, sizeof(*activator));

	if (activator) {
		activator->tracker = NULL;
		activator->producer = NULL;

		status = producer_create(&activator->producer);

		*userData = activator;
	} else {
		status = CELIX_ENOMEM;
	}
	return status;
}

celix_status_t bundleActivator_start(void * userData, bundle_context_pt context) {
	celix_status_t status = CELIX_SUCCESS;
	bundle_activator_pt activator = (bundle_activator_pt) userData;

	printf("PRODUCER: Starting bundle...\n");

	service_tracker_customizer_pt customizer = NULL;
	serviceTrackerCustomizer_create(activator->producer, NULL, producer_queueServiceAdded, NULL, producer_queueServiceRemoved, &customizer);

	serviceTracker_create(context, INAETICS_DEMONSTRATOR_API__SAMPLE_QUEUE_SERVICE_NAME, customizer, &activator->tracker);
	serviceTracker_open(activator->tracker);

	return status;
}

celix_status_t bundleActivator_stop(void * userData, bundle_context_pt context) {
	celix_status_t status = CELIX_SUCCESS;
	bundle_activator_pt activator = (bundle_activator_pt) userData;

	printf("PRODUCER: Stopping bundle...\n");

	producer_stop(activator->producer);
	serviceTracker_close(activator->tracker);

	return status;
}

celix_status_t bundleActivator_destroy(void * userData, bundle_context_pt context) {
	celix_status_t status = CELIX_SUCCESS;
	bundle_activator_pt activator = (bundle_activator_pt) userData;

	serviceTracker_close(activator->tracker);
	producer_stop(activator->producer);

	free(activator);

	return status;
}
