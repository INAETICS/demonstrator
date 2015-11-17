/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */

#ifndef PRODUCER_ENDPOINT_IMPL_H_
#define PRODUCER_ENDPOINT_IMPL_H_

#include "celix_errno.h"
#include "remote_endpoint_impl.h"

#include "inaetics_demonstrator_api/producer.h"

celix_status_t producerEndpoint_create(remote_endpoint_pt *endpoint);
celix_status_t producerEndpoint_destroy(remote_endpoint_pt *endpoint);
celix_status_t producerEndpoint_setService(remote_endpoint_pt endpoint, void *service);
celix_status_t producerEndpoint_handleRequest(remote_endpoint_pt endpoint, char *data, char **reply);

celix_status_t producerEndpoint_getMaxSampleRate(remote_endpoint_pt endpoint, char *data, char **reply);
celix_status_t producerEndpoint_getSampleRate(remote_endpoint_pt endpoint, char *data, char **reply);
celix_status_t producerEndpoint_setSampleRate(remote_endpoint_pt endpoint, char *data, char **reply);

#endif
