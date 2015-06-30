#include <stdlib.h>

#include "bundle_activator.h"
#include "service_registration.h"

#include "stats_provider_proxy_impl.h"
#include "inaetics_demonstrator_api/stats_provider.h"

#include "remote_proxy.h"

struct activator {
    bundle_context_pt context;
    remote_proxy_factory_pt factory_ptr;
};


static celix_status_t statsProviderProxyFactory_create(void *handle, endpoint_description_pt endpointDescription, remote_service_admin_pt rsa, sendToHandle sendToCallback, properties_pt properties,
        void **service);
static celix_status_t statsProviderProxyFactory_destroy(void *handle, void *service);

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

    remoteProxyFactory_create(context, (char *) INAETICS_DEMONSTRATOR_API__STATS_PROVIDER_SERVICE_NAME, activator, statsProviderProxyFactory_create, statsProviderProxyFactory_destroy, &activator->factory_ptr);
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




static celix_status_t statsProviderProxyFactory_create(void *handle, endpoint_description_pt endpointDescription, remote_service_admin_pt rsa, sendToHandle sendToCallback, properties_pt properties,
        void **service) {
    celix_status_t status = CELIX_SUCCESS;
    struct activator *activator = handle;
    struct stats_provider_service* statService = NULL;

    statService = calloc(1, sizeof(*statService));

    if (statService) {
        stats_provider_type* stat;
        statsProviderProxy_create(activator->context, &stat);

        statService->getName = statsProviderProxy_getName;
        statService->getType = statsProviderProxy_getType;
        statService->getValue = statsProviderProxy_getvalue;
        statService->getMeasurementUnit = statsProviderProxy_getMeasurementUnitUnit;

        stat->endpoint = endpointDescription;
        stat->sendToHandler = rsa;
        stat->sendToCallback = sendToCallback;

        statService->statsProvider = (void*) stat;
        *service = statService;
    } else {
        status = CELIX_ENOMEM;
    }
    return status;
}

static celix_status_t statsProviderProxyFactory_destroy(void *handle, void *service) {
    celix_status_t status = CELIX_SUCCESS;
    struct stats_provider_service* statService = NULL;

    if (!statService) {
        status = CELIX_ILLEGAL_ARGUMENT;
    }
    else {
        statsProviderProxy_destroy((stats_provider_type**) &statService->statsProvider);
        free(statService);
    }

    return status;
}
