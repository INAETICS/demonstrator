
#ifndef QUEUE_ENDPOINT_IMPL_H_
#define QUEUE_ENDPOINT_IMPL_H_

#include "celix_errno.h"
#include "remote_endpoint_impl.h"

#include "inaetics_demonstrator_api/sample_queue.h"

celix_status_t queueEndpoint_create(remote_endpoint_pt *endpoint);
celix_status_t queueEndpoint_setService(remote_endpoint_pt endpoint, void *service);
celix_status_t queueEndpoint_handleRequest(remote_endpoint_pt endpoint, char *data, char **reply);

celix_status_t queueEndpoint_put(remote_endpoint_pt endpoint, char *data, char **reply);
celix_status_t queueEndpoint_putAll(remote_endpoint_pt endpoint, char *data, char **reply);
celix_status_t queueEndpoint_take(remote_endpoint_pt endpoint, char *data, char **reply);
celix_status_t queueEndpoint_takeAll(remote_endpoint_pt endpoint, char *data, char **reply);

#endif
