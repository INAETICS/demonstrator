/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */

#ifndef DATA_STORE_ENDPOINT_IMPL_H_
#define DATA_STORE_ENDPOINT_IMPL_H_

#include "celix_errno.h"
#include "remote_endpoint_impl.h"

#include "inaetics_demonstrator_api/data_store.h"
#include "inaetics_demonstrator_api/sample.h"

celix_status_t dataStoreEndpoint_create(remote_endpoint_pt *endpoint);
celix_status_t dataStoreEndpoint_setService(remote_endpoint_pt endpoint, void *service);
celix_status_t dataStoreEndpoint_handleRequest(remote_endpoint_pt endpoint, char *data, char **reply);

celix_status_t dataStoreEndpoint_store(remote_endpoint_pt endpoint, char *data, char **reply);
celix_status_t dataStoreEndpoint_storeAll(remote_endpoint_pt endpoint, char *data, char **reply);

#endif
