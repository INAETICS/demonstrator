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

celix_status_t statsProviderProxy_create(bundle_context_pt context, stats_provider_type **stat);
celix_status_t statsProviderProxy_destroy(stats_provider_type **stat);

int statsProviderProxy_getName(void* statsProvider, char** name);
int statsProviderProxy_getType(void* statsProvider, char** type);
int statsProviderProxy_getvalue(void* statsProvider, double* statVal);
int statsProviderProxy_getMeasurementUnitUnit(void* statsProvider, char** mUnit);

#endif /* STATS_PROVIDER_PROXY_IMPL_H_ */
