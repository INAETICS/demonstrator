/*
 * activator.c
 *
 *  Created on: 18 Sep 2014
 *      Author: abroekhuis
 */

#include <stdlib.h>
#include <unistd.h>

#include <sys/time.h>

#include <bundle_activator.h>
#include <service_tracker.h>
#include <remote_constants.h>
#include <constants.h>

#include <array_list.h>
#include <pthread.h>

#include "inaetics_demonstrator_api/sample_queue.h"
#include "inaetics_demonstrator_api/service_statistics.h"
#include "inaetics_demonstrator_api/stats_provider.h"

#include "arraylist_sample_queue_impl.h"

struct activator {
	bundle_context_pt context;
	sample_queue_type *sampleQueue;

	service_registration_pt queueRegistration;
	struct sample_queue_service* queueService;

	service_registration_pt queueStatsRegistration;
	struct stats_provider_service* queueStatsService;
};

static celix_status_t bundleActivator_createQueueService(struct activator *act);
static celix_status_t bundleActivator_createStatsService(struct activator *act);

static celix_status_t bundleActivator_registerQueueService(struct activator *act);
static celix_status_t bundleActivator_registerStatsService(struct activator *act);

celix_status_t bundleActivator_create(bundle_context_pt context, void **userData) {
	celix_status_t status = CELIX_SUCCESS;

	*userData = calloc(1, sizeof(struct activator));

	if (*userData) {
		((struct activator *) *userData)->context = NULL;
		((struct activator *) *userData)->sampleQueue = NULL;
		((struct activator *) *userData)->queueService = NULL;
		((struct activator *) *userData)->queueRegistration = NULL;
		((struct activator *) *userData)->queueStatsService = NULL;
		((struct activator *) *userData)->queueStatsRegistration = NULL;

	} else {
		status = CELIX_ENOMEM;
	}

	return status;
}

celix_status_t bundleActivator_start(void * userData, bundle_context_pt context) {
	celix_status_t status = CELIX_SUCCESS;
	struct activator *activator = userData;

	//Creating sample queue Component

	//Setting up name for sample queue.
	char *name;
	char *uuid;
	bundleContext_getProperty(context, "inaetics.demonstrator.queue.name", &name);
	bundleContext_getProperty(context, OSGI_FRAMEWORK_FRAMEWORK_UUID, &uuid);
	if (name == NULL && uuid != NULL) {
		char uuidName[128];
		sprintf("Sample Queue %.4s", uuid);
		name = uuidName;
	} else {
		name = "queue";
	}

	status = sampleQueue_create(name, &activator->sampleQueue);

	if(status==CELIX_SUCCESS) {

		//Creating service structs for services provided by the component
		bundleActivator_createQueueService(activator);
		bundleActivator_createStatsService(activator);

		if (activator->queueService != NULL) {
			bundleActivator_registerQueueService(activator);
		}
		if (activator->queueStatsService != NULL) {
			bundleActivator_registerStatsService(activator);
		}

		if (	activator->queueService == NULL || activator->queueStatsService == NULL ||
				activator->queueRegistration == NULL || activator->queueStatsRegistration == NULL) {
			printf("SAMPLE QUEUE: Error creating/registering services\n");
			status = CELIX_BUNDLE_EXCEPTION;
		}
	}
	return status;
}

static celix_status_t bundleActivator_createQueueService(struct activator *act) {
	celix_status_t status = CELIX_SUCCESS;

	act->queueService = calloc(1, sizeof(*act->queueService));

	if (act->queueService != NULL) {
		act->queueService->sampleQueue = act->sampleQueue;
		act->queueService->put = (void *)sampleQueue_put;
		act->queueService->putAll = (void *)sampleQueue_putAll;
		act->queueService->take = (void *)sampleQueue_take;
		act->queueService->takeAll = (void *)sampleQueue_takeAll;
	} else {
		status = CELIX_ENOMEM;
	}

	return status;
}

static celix_status_t bundleActivator_createStatsService(struct activator *act) {
	celix_status_t status = CELIX_SUCCESS;

	act->queueStatsService = calloc(1, sizeof(*act->queueStatsService));

	if (act->queueStatsService != NULL) {
		act->queueStatsService->statsProvider = act->sampleQueue;
		act->queueStatsService->getName = (void *)sampleQueue_getUtilizationStatsName;
		act->queueStatsService->getType = (void *)sampleQueue_getUtilizationStatsType;
		act->queueStatsService->getMeasurementUnit = (void *)sampleQueue_getUtilizationStatsMeasurementUnit;
		act->queueStatsService->getValue = (void *)SampleQueue_getUtilizationStatsValue;
	} else {
		status = CELIX_ENOMEM;
	}

	return status;
}

static celix_status_t bundleActivator_registerQueueService(struct activator *act) {
	celix_status_t status = CELIX_SUCCESS;
	properties_pt props = NULL;
	props = properties_create();
	properties_set(props, (char *) OSGI_RSA_SERVICE_EXPORTED_INTERFACES, (char *) INAETICS_DEMONSTRATOR_API__SAMPLE_QUEUE_SERVICE_NAME);

	status = bundleContext_registerService(act->context, (char *)INAETICS_DEMONSTRATOR_API__SAMPLE_QUEUE_SERVICE_NAME,
						act->queueService, props, &act->queueRegistration);

	return status;
}

static celix_status_t bundleActivator_registerStatsService(struct activator *act) {
	celix_status_t status = CELIX_SUCCESS;
	properties_pt props = NULL;
	props = properties_create();
	properties_set(props, (char *) OSGI_RSA_SERVICE_EXPORTED_INTERFACES, (char *) INAETICS_DEMONSTRATOR_API__STATS_PROVIDER_SERVICE_NAME);

	status = bundleContext_registerService(act->context, (char *)INAETICS_DEMONSTRATOR_API__STATS_PROVIDER_SERVICE_NAME,
						act->queueStatsService, props, &act->queueStatsRegistration);

	return status;
}


celix_status_t bundleActivator_stop(void * userData, bundle_context_pt context) {

	celix_status_t status = CELIX_SUCCESS;
	struct activator *activator = userData;

	serviceRegistration_unregister(activator->queueStatsRegistration);
	serviceRegistration_unregister(activator->queueRegistration);

	free(activator->queueService);
	free(activator->queueStatsService);

	sampleQueue_destroy(activator->sampleQueue);

	return status;
}

celix_status_t bundleActivator_destroy(void * userData, bundle_context_pt context) {
	celix_status_t status = CELIX_SUCCESS;
	struct activator *activator = userData;

	free(activator);

	return status;
}

