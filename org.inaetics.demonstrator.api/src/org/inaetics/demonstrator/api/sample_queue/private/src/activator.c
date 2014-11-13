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
#include "arraylist_queue_service_impl.h"

struct activator {
	sample_queue_type* queueHandler;

	service_registration_pt queueRegistration;

	struct sample_queue_service* queueService;

};

celix_status_t bundleActivator_create(bundle_context_pt context, void **userData) {
	celix_status_t status = CELIX_SUCCESS;

	*userData = calloc(1, sizeof(struct activator));

	if (*userData) {
		((struct activator *) *userData)->queueService = NULL;
		((struct activator *) *userData)->queueHandler = NULL;

	} else {
		status = CELIX_ENOMEM;
	}

	return status;
}

celix_status_t bundleActivator_start(void * userData, bundle_context_pt context) {
	celix_status_t status = CELIX_SUCCESS;
	struct activator *activator = userData;

	struct sample_queue_service* qService = NULL;
	sample_queue_type* qHandler = NULL;

	status = queueService_create(&qService, &qHandler);

	if (status == CELIX_SUCCESS){
		properties_pt properties = NULL;

		activator->queueHandler = qHandler;
		activator->queueService = qService;

		properties = properties_create();
		properties_set(properties, (char *) OSGI_RSA_SERVICE_EXPORTED_INTERFACES, (char *) INAETICS_DEMONSTRATOR_API__SAMPLE_QUEUE_SERVICE_NAME);

		status = bundleContext_registerService(context, INAETICS_DEMONSTRATOR_API__SAMPLE_QUEUE_SERVICE_NAME,
				activator->queueService, properties, &activator->queueRegistration);

		printf("SAMPLE_QUEUE: Service %s %s\n", INAETICS_DEMONSTRATOR_API__SAMPLE_QUEUE_SERVICE_NAME,
				status == CELIX_SUCCESS ? "registered" : "registration failed");
	}

	return status;
}

celix_status_t bundleActivator_stop(void * userData, bundle_context_pt context) {
	celix_status_t status = CELIX_SUCCESS;
	struct activator *activator = userData;

	serviceRegistration_unregister(activator->queueRegistration);

	status = queueService_destroy(activator->queueService, activator->queueHandler);

	return status;
}

celix_status_t bundleActivator_destroy(void * userData, bundle_context_pt context) {
	celix_status_t status = CELIX_SUCCESS;
	struct activator *activator = userData;

	free(activator);

	return status;
}

