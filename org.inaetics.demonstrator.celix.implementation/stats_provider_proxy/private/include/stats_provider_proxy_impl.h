#ifndef STATS_PROVIDER_PROXY_IMPL_H_
#define STATS_PROVIDER_PROXY_IMPL_H_

#include "celix_errno.h"

#include "remote_proxy.h"
#include "constants.h"
#include "bundle_context.h"
#include "hash_map.h"

#include "endpoint_listener.h"
#include "inaetics_demonstrator_api/stats_provider.h"

typedef struct stats_provider stats_provider_type;

struct stats_provider {
	bundle_context_pt context;

	endpoint_description_pt endpoint;
	sendToHandle sendToCallback;
	void * sendToHandler;
};

celix_status_t statsProviderProxy_getName(stats_provider_type* statsProvider, char** name);
celix_status_t statsProviderProxy_getType(stats_provider_type* statsProvider, char** type);
celix_status_t statsProviderProxy_getvalue(stats_provider_type* statsProvider, double* statVal);
celix_status_t statsProviderProxy_getMeasurementUnitUnit(stats_provider_type* statsProvider, char** mUnit);

celix_status_t statsProviderProxy_registerProxyService(void* proxyFactoryService, endpoint_description_pt endpoint, void* handler, sendToHandle callback);
celix_status_t statsProviderProxy_unregisterProxyService(void* proxyFactoryService, endpoint_description_pt endpoint);

#endif /* STATS_PROVIDER_PROXY_IMPL_H_ */
