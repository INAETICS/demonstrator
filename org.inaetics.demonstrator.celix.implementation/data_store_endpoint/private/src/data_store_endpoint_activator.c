/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
#include <stdlib.h>

#include "bundle_activator.h"

#include "data_store_endpoint_impl.h"
#include "service_registration.h"
#include "remote_endpoint.h"


struct activator {
	service_registration_pt registration;
	remote_endpoint_service_pt endpointService;
};

celix_status_t bundleActivator_create(bundle_context_pt context, void **userData) {
	celix_status_t status = CELIX_SUCCESS;

	struct activator *activator;

	activator = calloc(1, sizeof(*activator));
	if (!activator) {
		status = CELIX_ENOMEM;
	} else {
		activator->registration = NULL;
		activator->endpointService = NULL;

		*userData = activator;
	}

	return status;
}

celix_status_t bundleActivator_start(void * userData, bundle_context_pt context) {
	celix_status_t status = CELIX_SUCCESS;
	struct activator *activator = userData;
	remote_endpoint_pt endpoint = NULL;
	remote_endpoint_service_pt endpointService = NULL;

	dataStoreEndpoint_create(&endpoint);
	endpointService = calloc(1, sizeof(*endpointService));
	if (endpointService) {
		endpointService->endpoint = endpoint;
		endpointService->handleRequest = dataStoreEndpoint_handleRequest;
		endpointService->setService = dataStoreEndpoint_setService;

		properties_pt props = properties_create();
		properties_set(props, (char *) "remote.interface", (char *) INAETICS_DEMONSTRATOR_API__DATA_STORE_SERVICE_NAME);

		bundleContext_registerService(context, OSGI_RSA_REMOTE_ENDPOINT, endpointService, props, &activator->registration);

		if (status == CELIX_SUCCESS) {
			activator->endpointService = endpointService;
		}
	} else {
		status = CELIX_ENOMEM;
	}

	return status;
}

celix_status_t bundleActivator_stop(void * userData, bundle_context_pt context) {
	celix_status_t status = CELIX_SUCCESS;
	struct activator *activator = userData;

	serviceRegistration_unregister(activator->registration);

	if (activator->endpointService) {
		dataStoreEndpoint_create(&activator->endpointService->endpoint);
		free(activator->endpointService);
	}

	return status;
}

celix_status_t bundleActivator_destroy(void * userData, bundle_context_pt context) {
	celix_status_t status = CELIX_SUCCESS;

	free((struct activator *) userData);

	return status;
}
