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

#include "inaetics_demonstrator_api/service_statistics.h"
#include "statistic_tracker_impl.h"

struct bundle_activator {
	service_tracker_pt tracker;
	statistic_tracker_pt statTracker;
};

typedef struct bundle_activator* bundle_activator_pt;

celix_status_t bundleActivator_create(bundle_context_pt context, void **userData) {
	celix_status_t status = CELIX_SUCCESS;
	bundle_activator_pt activator = (bundle_activator_pt) calloc(1, sizeof(*activator));

	if (activator) {
		activator->tracker = NULL;
		activator->statTracker = NULL;

		status = statistic_tracker_create(&activator->statTracker);

		*userData = activator;
	} else {
		status = CELIX_ENOMEM;
	}
	return status;
}

celix_status_t bundleActivator_start(void * userData, bundle_context_pt context) {
	celix_status_t status = CELIX_SUCCESS;
	bundle_activator_pt activator = (bundle_activator_pt) userData;

	printf("STAT_TRACKER: Starting bundle...\n");

	service_tracker_customizer_pt customizer = NULL;
	serviceTrackerCustomizer_create(activator->statTracker, NULL, statistic_tracker_statServiceAdded, NULL, statistic_tracker_statServiceRemoved, &customizer);

	serviceTracker_create(context, INAETICS_DEMONSTRATOR_API_SERVICE_STATISTICS_SERVICE_NAME, customizer, &activator->tracker);
	serviceTracker_open(activator->tracker);

	return status;
}

celix_status_t bundleActivator_stop(void * userData, bundle_context_pt context) {
	celix_status_t status = CELIX_SUCCESS;
	bundle_activator_pt activator = (bundle_activator_pt) userData;

	printf("STAT_TRACKER: Stopping bundle...\n");

	statistic_tracker_stop(activator->statTracker);
	serviceTracker_close(activator->tracker);

	return status;
}

celix_status_t bundleActivator_destroy(void * userData, bundle_context_pt context) {
	celix_status_t status = CELIX_SUCCESS;
	bundle_activator_pt activator = (bundle_activator_pt) userData;

	free(activator);

	return status;
}
