/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */

#include <stdlib.h>

#include "bundle_activator.h"
#include "service_registration.h"

#include "producer_proxy_impl.h"
#include "inaetics_demonstrator_api/producer.h"

#include "remote_proxy.h"

struct activator {
	bundle_context_pt context;
	remote_proxy_factory_pt factory_ptr;
};

static celix_status_t producerProxyFactory_create(void *handle, endpoint_description_pt endpointDescription, remote_service_admin_pt rsa, sendToHandle sendToCallback, properties_pt properties, void **service);
static celix_status_t producerProxyFactory_destroy(void *handle, void *service);

celix_status_t bundleActivator_create(bundle_context_pt context, void **userData) {
	celix_status_t status = CELIX_SUCCESS;
	struct activator *activator;

	activator = calloc(1, sizeof(*activator));
	if (!activator) {
		status = CELIX_ENOMEM;
	} else {
		activator->factory_ptr = NULL;
		activator->context = context;

		*userData = activator;
	}

	return status;
}

celix_status_t bundleActivator_start(void * userData, bundle_context_pt context) {
	celix_status_t status = CELIX_SUCCESS;
	struct activator *activator = userData;

	remoteProxyFactory_create(context, (char *) INAETICS_DEMONSTRATOR_API__PRODUCER_SERVICE_NAME, activator, producerProxyFactory_create, producerProxyFactory_destroy, &activator->factory_ptr);
	remoteProxyFactory_register(activator->factory_ptr);

	return status;
}

celix_status_t bundleActivator_stop(void * userData, bundle_context_pt context) {
	celix_status_t status = CELIX_SUCCESS;
	struct activator *activator = userData;

	remoteProxyFactory_unregister(activator->factory_ptr);
	remoteProxyFactory_destroy(&activator->factory_ptr);

	return status;
}

celix_status_t bundleActivator_destroy(void * userData, bundle_context_pt context) {
	celix_status_t status = CELIX_SUCCESS;
	struct activator *activator = userData;

	free(activator);

	return status;
}

static celix_status_t producerProxyFactory_create(void *handle, endpoint_description_pt endpointDescription, remote_service_admin_pt rsa, sendToHandle sendToCallback, properties_pt properties, void **service) {
	celix_status_t status = CELIX_SUCCESS;
	struct activator *activator = handle;
	producer_service_pt producerService = NULL;

	producerService = calloc(1, sizeof(*producerService));

	if (producerService) {
		producer_type_pt producer;
		producerProxy_create(activator->context, &producer);

		producerService->getMaxSampleRate = producerProxy_getMaxSampleRate;
		producerService->getSampleRate = producerProxy_getSampleRate;
		producerService->setSampleRate = producerProxy_setSampleRate;

		producer->endpoint = endpointDescription;
		producer->sendToHandler = rsa;
		producer->sendToCallback = sendToCallback;

		producerService->producer = (void*) producer;

		*service = producerService;
	} else {
		status = CELIX_ENOMEM;
	}
	return status;
}

static celix_status_t producerProxyFactory_destroy(void *handle, void *service) {
	celix_status_t status = CELIX_SUCCESS;
	producer_service_pt producerService = (producer_service_pt) service;

	if (!producerService) {
		status = CELIX_ILLEGAL_ARGUMENT;
	} else {
		producerProxy_destroy((producer_type_pt*) &producerService->producer);
		free(producerService);
	}

	return status;
}
