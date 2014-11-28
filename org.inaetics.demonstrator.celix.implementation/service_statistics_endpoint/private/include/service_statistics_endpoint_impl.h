#ifndef SERVICE_STATISTICS_ENDPOINT_IMPL_H_
#define SERVICE_STATISTICS_ENDPOINT_IMPL_H_

#include "celix_errno.h"
#include "remote_endpoint_impl.h"

#include "inaetics_demonstrator_api/service_statistics.h"

celix_status_t serviceStatisticsEndpoint_create(remote_endpoint_pt *endpoint);
celix_status_t serviceStatisticsEndpoint_setService(remote_endpoint_pt endpoint, void *service);
celix_status_t serviceStatisticsEndpoint_handleRequest(remote_endpoint_pt endpoint, char *data, char **reply);

celix_status_t serviceStatisticsEndpoint_getServiceName(remote_endpoint_pt endpoint, char *data, char **reply);
celix_status_t serviceStatisticsEndpoint_getServiceType(remote_endpoint_pt endpoint, char *data, char **reply);
celix_status_t serviceStatisticsEndpoint_getStatistic(remote_endpoint_pt endpoint, char *data, char **reply);
celix_status_t serviceStatisticsEndpoint_getMeasurementUnit(remote_endpoint_pt endpoint, char *data, char **reply);

#endif
