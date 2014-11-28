#ifndef SERVICE_STATISTICS_PROXY_IMPL_H_
#define SERVICE_STATISTICS_PROXY_IMPL_H_

#include "celix_errno.h"

#include "remote_proxy.h"
#include "constants.h"
#include "bundle_context.h"
#include "hash_map.h"

#include "endpoint_listener.h"
#include "inaetics_demonstrator_api/service_statistics.h"

struct service_statistics {
	bundle_context_pt context;

	endpoint_description_pt endpoint;
	sendToHandle sendToCallback;
	void * sendToHandler;
};


celix_status_t serviceStatisticsProxy_getServiceName(service_statistics_type* serviceStatistics, char** name);
celix_status_t serviceStatisticsProxy_getServiceType(service_statistics_type* serviceStatistics, char** type);
celix_status_t serviceStatisticsProxy_getStatistic(service_statistics_type* serviceStatistics, double* statVal);
celix_status_t serviceStatisticsProxy_getMeasurementUnit(service_statistics_type* serviceStatistics, char** mUnit);

celix_status_t serviceStatisticsProxy_registerProxyService(void* proxyFactoryService, endpoint_description_pt endpoint, void* handler, sendToHandle callback);
celix_status_t serviceStatisticsProxy_unregisterProxyService(void* proxyFactoryService, endpoint_description_pt endpoint);

#endif /* QUEUE_PROXY_IMPL_H_ */
