/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
#include <stdlib.h>

#include "bundle_activator.h"

#include "queue_endpoint_impl.h"
#include "service_registration.h"

struct activator {
	service_registration_pt endpoint;
};

celix_status_t bundleActivator_create(bundle_context_pt context, void **userData) {
	celix_status_t status = CELIX_SUCCESS;

	struct activator *activator;

	activator = calloc(1, sizeof(*activator));
	if (!activator) {
		status = CELIX_ENOMEM;
	} else {
		activator->endpoint = NULL;

		*userData = activator;
	}

	return status;
}

celix_status_t bundleActivator_start(void * userData, bundle_context_pt context) {
	celix_status_t status = CELIX_SUCCESS;
	struct activator *activator = userData;
	remote_endpoint_pt endpoint = NULL;
	remote_endpoint_service_pt endpointService = NULL;

	queueEndpoint_create(&endpoint);
	endpointService = calloc(1, sizeof(*endpointService));
	endpointService->endpoint = endpoint;
	endpointService->handleRequest = queueEndpoint_handleRequest;
	endpointService->setService = queueEndpoint_setService;

	properties_pt props = properties_create();
	properties_set(props, (char *) "remote.interface", (char *) INAETICS_DEMONSTRATOR_API__SAMPLE_QUEUE_SERVICE_NAME);

	bundleContext_registerService(context, OSGI_RSA_REMOTE_ENDPOINT, endpointService, props, &activator->endpoint);


	return status;
}

celix_status_t bundleActivator_stop(void * userData, bundle_context_pt context) {
	celix_status_t status = CELIX_SUCCESS;
	struct activator *activator = userData;

	serviceRegistration_unregister(activator->endpoint);

	return status;
}

celix_status_t bundleActivator_destroy(void * userData, bundle_context_pt context) {
	celix_status_t status = CELIX_SUCCESS;
	return status;
}
