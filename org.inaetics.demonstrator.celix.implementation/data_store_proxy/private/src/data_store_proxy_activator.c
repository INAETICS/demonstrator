/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */

#include <stdlib.h>

#include "bundle_activator.h"
#include "service_registration.h"

#include "data_store_proxy_impl.h"
#include "inaetics_demonstrator_api/data_store.h"

#include "remote_proxy.h"

struct activator {
    bundle_context_pt context;
    remote_proxy_factory_pt factory_ptr;
};


static celix_status_t dataStoreProxyFactory_create(void *handle, endpoint_description_pt endpointDescription, remote_service_admin_pt rsa, sendToHandle sendToCallback, properties_pt properties,
        void **service);
static celix_status_t dataStoreProxyFactory_destroy(void *handle, void *service);

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

	remoteProxyFactory_create(context, (char *) INAETICS_DEMONSTRATOR_API__DATA_STORE_SERVICE_NAME, activator, dataStoreProxyFactory_create, dataStoreProxyFactory_destroy, &activator->factory_ptr);
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



static celix_status_t dataStoreProxyFactory_create(void *handle, endpoint_description_pt endpointDescription, remote_service_admin_pt rsa, sendToHandle sendToCallback, properties_pt properties,
        void **service) {
    celix_status_t status = CELIX_SUCCESS;
    struct activator *activator = handle;
    struct data_store_service* storeService = NULL;

    storeService = calloc(1, sizeof(*storeService));

    if (storeService) {
        dataStoreProxy_create(activator->context, &storeService->dataStore);

        storeService->store = dataStoreProxy_store;
        storeService->storeAll = dataStoreProxy_storeAll;

        storeService->dataStore->endpoint = endpointDescription;
        storeService->dataStore->sendToHandler = rsa;
        storeService->dataStore->sendToCallback = sendToCallback;

        *service = storeService;
    } else {
        status = CELIX_ENOMEM;
    }
    return status;
}

static celix_status_t dataStoreProxyFactory_destroy(void *handle, void *service) {
    celix_status_t status = CELIX_SUCCESS;
    struct data_store_service* storeService = NULL;

    if (!storeService) {
        status = CELIX_ILLEGAL_ARGUMENT;
    }
    else {
        dataStoreProxy_destroy(&storeService->dataStore);
        free(storeService);
    }

    return status;
}
