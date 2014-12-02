#include <stdlib.h>

#include "bundle_activator.h"
#include "service_registration.h"

#include "stats_provider_proxy_impl.h"
#include "inaetics_demonstrator_api/stats_provider.h"

struct activator {
	service_registration_pt proxyFactoryService;
};

celix_status_t bundleActivator_create(bundle_context_pt context, void **userData) {
	celix_status_t status = CELIX_SUCCESS;
	struct activator *activator;

	activator = calloc(1, sizeof(*activator));
	if (!activator) {
		status = CELIX_ENOMEM;
	} else {
		activator->proxyFactoryService = NULL;

		*userData = activator;
	}

	return status;
}

celix_status_t bundleActivator_start(void * userData, bundle_context_pt context) {
	celix_status_t status = CELIX_SUCCESS;
	struct activator *activator = userData;
	remote_proxy_factory_service_pt statsProviderProxyFactoryService;

	statsProviderProxyFactoryService = calloc(1, sizeof(*statsProviderProxyFactoryService));
	statsProviderProxyFactoryService->context = context;
	statsProviderProxyFactoryService->proxy_registrations = hashMap_create(NULL, NULL, NULL, NULL);
	statsProviderProxyFactoryService->registerProxyService = statsProviderProxy_registerProxyService;
	statsProviderProxyFactoryService->unregisterProxyService = statsProviderProxy_unregisterProxyService;

	properties_pt props = properties_create();
	properties_set(props, (char *) "proxy.interface", (char *) INAETICS_DEMONSTRATOR_API__STATS_PROVIDER_SERVICE_NAME);

	if (bundleContext_registerService(context, OSGI_RSA_REMOTE_PROXY_FACTORY, statsProviderProxyFactoryService, props, &activator->proxyFactoryService) == CELIX_SUCCESS)
			{
		printf("STATS_PROVIDER_PROXY: Proxy registered OSGI_RSA_REMOTE_PROXY_FACTORY (%s)\n", OSGI_RSA_REMOTE_PROXY_FACTORY);
	}

	return status;
}

celix_status_t bundleActivator_stop(void * userData, bundle_context_pt context) {
	celix_status_t status = CELIX_SUCCESS;
	struct activator *activator = userData;

	serviceRegistration_unregister(activator->proxyFactoryService);
	activator->proxyFactoryService = NULL;

	return status;
}

celix_status_t bundleActivator_destroy(void * userData, bundle_context_pt context) {
	celix_status_t status = CELIX_SUCCESS;

	struct activator *activator = userData;
	free(activator);

	return status;
}
