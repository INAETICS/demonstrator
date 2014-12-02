#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bundle_context.h"
#include "celix_errno.h"
#include "constants.h"
#include "properties.h"
#include "service_registration.h"
#include "service_tracker.h"
#include "service_tracker_customizer.h"
#include "remote_constants.h"
#include "inaetics_demonstrator_api/sample_queue.h"
#include "inaetics_demonstrator_api/stats_provider.h"
#include "producer_impl.h"

struct bundle_activator {
	bundle_context_pt context;

	service_tracker_pt tracker;
	producer_pt producer;

	service_registration_pt producerStatsRegistration;
	struct stats_provider_service* producerStatsService;
};

typedef struct bundle_activator* bundle_activator_pt;

static celix_status_t bundleActivator_createStatsService(bundle_activator_pt act);
static celix_status_t bundleActivator_registerStatsService(bundle_activator_pt act);

celix_status_t bundleActivator_create(bundle_context_pt context, void **userData) {
	celix_status_t status = CELIX_SUCCESS;
	bundle_activator_pt activator = (bundle_activator_pt) calloc(1, sizeof(*activator));

	if (activator) {
		activator->context = context;
		activator->tracker = NULL;
		activator->producer = NULL;
		activator->producerStatsService = NULL;
		activator->producerStatsRegistration = NULL;

		*userData = activator;
	} else {
		status = CELIX_ENOMEM;
	}
	return status;
}

celix_status_t bundleActivator_start(void * userData, bundle_context_pt context) {
	celix_status_t status = CELIX_SUCCESS;
	bundle_activator_pt activator = (bundle_activator_pt) userData;

	char *name = NULL;
	char *uuid = NULL;
	char uuidName[128];
	memset(uuidName, 0, 128);

	bundleContext_getProperty(context, "inaetics.demonstrator.producer.name", &name);
	bundleContext_getProperty(context, OSGI_FRAMEWORK_FRAMEWORK_UUID, &uuid);

	if (name != NULL) {
		snprintf(uuidName, 128, "Producer %s", name);
	}
	else if (name == NULL && uuid != NULL) {
		snprintf(uuidName, 128, "Producer %.8s", uuid);
	} else {
		snprintf(uuidName, 128, "Producer (unknown ID)");
	}

	printf("PRODUCER: Starting bundle...\n");

	status = producer_create(uuidName, &activator->producer);

	if (status == CELIX_SUCCESS) {

		bundleActivator_createStatsService(activator);

		if (activator->producerStatsService != NULL) {
			bundleActivator_registerStatsService(activator);
		}

		if (activator->producerStatsService == NULL || activator->producerStatsRegistration == NULL) {
			printf("PRODUCER: Error creating/registering statService\n");
		}
		else {
			printf("PRODUCER: Created processor with name %s\n", uuidName);
		}

	}

	service_tracker_customizer_pt customizer = NULL;
	serviceTrackerCustomizer_create(activator->producer, NULL, producer_queueServiceAdded, NULL, producer_queueServiceRemoved, &customizer);

	serviceTracker_create(context, INAETICS_DEMONSTRATOR_API__SAMPLE_QUEUE_SERVICE_NAME, customizer, &activator->tracker);
	serviceTracker_open(activator->tracker);

	return status;
}

static celix_status_t bundleActivator_createStatsService(bundle_activator_pt act) {
	celix_status_t status = CELIX_SUCCESS;

	act->producerStatsService = calloc(1, sizeof(*act->producerStatsService));

	if (act->producerStatsService != NULL) {
		act->producerStatsService->statsProvider = act->producer;
		act->producerStatsService->getName = (void *) producer_getUtilizationStatsName;
		act->producerStatsService->getType = (void *) producer_getUtilizationStatsType;
		act->producerStatsService->getMeasurementUnit = (void *) producer_getUtilizationStatsMeasurementUnit;
		act->producerStatsService->getValue = (void *) producer_getUtilizationStatsValue;
	} else {
		status = CELIX_ENOMEM;
	}

	return status;
}

static celix_status_t bundleActivator_registerStatsService(bundle_activator_pt act) {
	celix_status_t status = CELIX_SUCCESS;
	properties_pt props = NULL;
	props = properties_create();
	properties_set(props, (char *) OSGI_RSA_SERVICE_EXPORTED_INTERFACES, (char *) INAETICS_DEMONSTRATOR_API__STATS_PROVIDER_SERVICE_NAME);

	status = bundleContext_registerService(act->context, (char *) INAETICS_DEMONSTRATOR_API__STATS_PROVIDER_SERVICE_NAME,
			act->producerStatsService, props, &act->producerStatsRegistration);

	return status;
}

celix_status_t bundleActivator_stop(void * userData, bundle_context_pt context) {
	celix_status_t status = CELIX_SUCCESS;
	bundle_activator_pt activator = (bundle_activator_pt) userData;

	printf("PRODUCER: Stopping bundle...\n");

	serviceTracker_close(activator->tracker);
	serviceTracker_destroy(activator->tracker);

	serviceRegistration_unregister(activator->producerStatsRegistration);

	producer_stop(activator->producer);


	return status;
}

celix_status_t bundleActivator_destroy(void * userData, bundle_context_pt context) {
	celix_status_t status = CELIX_SUCCESS;
	bundle_activator_pt activator = (bundle_activator_pt) userData;

	producer_destroy(activator->producer);

	free(activator->producerStatsService);
	free(activator);

	return status;
}
