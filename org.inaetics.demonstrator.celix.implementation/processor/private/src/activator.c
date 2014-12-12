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
#include "inaetics_demonstrator_api/data_store.h"
#include "inaetics_demonstrator_api/sample_queue.h"
#include "inaetics_demonstrator_api/stats_provider.h"
#include "processor_impl.h"

#define PROCESSOR_STATS_SERVICE_NAME_PREFIX "Celix Processor"

struct bundle_activator {
	bundle_context_pt context;

	service_tracker_pt queueTracker;
	service_tracker_pt dataStoreTracker;
	processor_pt processor;

	service_registration_pt processorStatsRegistration;
	struct stats_provider_service* processorStatsService;
};

typedef struct bundle_activator* bundle_activator_pt;

static celix_status_t bundleActivator_createStatsService(bundle_activator_pt act);
static celix_status_t bundleActivator_registerStatsService(bundle_activator_pt act);

celix_status_t bundleActivator_create(bundle_context_pt context, void **userData) {
	celix_status_t status = CELIX_SUCCESS;

	bundle_activator_pt activator = (bundle_activator_pt) calloc(1, sizeof(*activator));

	if (activator) {
		activator->context = context;
		activator->queueTracker = NULL;
		activator->dataStoreTracker = NULL;
		activator->processor = NULL;
		activator->processorStatsService = NULL;
		activator->processorStatsRegistration = NULL;

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

	char *name = NULL;
	char *uuid = NULL;
	char uuidName[128];
	memset(uuidName, 0, 128);

	bundleContext_getProperty(context, "inaetics.demonstrator.processor.name", &name);
	bundleContext_getProperty(context, OSGI_FRAMEWORK_FRAMEWORK_UUID, &uuid);

	if (name != NULL) {
		snprintf(uuidName, 128, "%s %s",PROCESSOR_STATS_SERVICE_NAME_PREFIX, name);
	}
	else if (name == NULL && uuid != NULL) {
		snprintf(uuidName, 128, "%s %.8s",PROCESSOR_STATS_SERVICE_NAME_PREFIX, uuid);
	} else {
		snprintf(uuidName, 128, "%s (unknown ID)",PROCESSOR_STATS_SERVICE_NAME_PREFIX);
	}

	printf("PROCESSOR: Starting bundle...\n");

	status = processor_create(uuidName, &activator->processor);

	if (status == CELIX_SUCCESS) {

		bundleActivator_createStatsService(activator);

		if (activator->processorStatsService != NULL) {
			bundleActivator_registerStatsService(activator);
		}

		if (activator->processorStatsService == NULL || activator->processorStatsRegistration == NULL) {
			printf("PROCESSOR: Error creating/registering statService\n");
		}
		else {
			printf("PROCESSOR: Created processor with name %s\n", uuidName);
		}

	}

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

static celix_status_t bundleActivator_createStatsService(bundle_activator_pt act) {
	celix_status_t status = CELIX_SUCCESS;

	act->processorStatsService = calloc(1, sizeof(*act->processorStatsService));

	if (act->processorStatsService != NULL) {
		act->processorStatsService->statsProvider = act->processor;
		act->processorStatsService->getName = (void *) processor_getUtilizationStatsName;
		act->processorStatsService->getType = (void *) processor_getUtilizationStatsType;
		act->processorStatsService->getMeasurementUnit = (void *) processor_getUtilizationStatsMeasurementUnit;
		act->processorStatsService->getValue = (void *) processor_getUtilizationStatsValue;
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
			act->processorStatsService, props, &act->processorStatsRegistration);

	return status;
}

celix_status_t bundleActivator_stop(void * userData, bundle_context_pt context) {
	celix_status_t status = CELIX_SUCCESS;
	bundle_activator_pt activator = userData;

	printf("PROCESSOR: Stopping bundle...\n");
	serviceTracker_close(activator->queueTracker);
	serviceTracker_close(activator->dataStoreTracker);

	serviceTracker_destroy(activator->queueTracker);
	serviceTracker_destroy(activator->dataStoreTracker);

	serviceRegistration_unregister(activator->processorStatsRegistration);

	processor_stop(activator->processor);

	free(activator->processorStatsService);

	processor_destroy(activator->processor);

	return status;
}

celix_status_t bundleActivator_destroy(void * userData, bundle_context_pt context) {
	celix_status_t status = CELIX_SUCCESS;
	bundle_activator_pt activator = userData;

	free(activator);

	return status;
}
