/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */


#ifndef DATA_STORE_PROXY_IMPL_H_
#define DATA_STORE_PROXY_IMPL_H_

#include "celix_errno.h"

#include "remote_proxy.h"
#include "constants.h"
#include "bundle_context.h"
#include "hash_map.h"

#include "endpoint_listener.h"
#include "inaetics_demonstrator_api/data_store.h"

struct data_store {
	bundle_context_pt context;
	endpoint_description_pt endpoint;
	sendToHandle sendToCallback;
	void * sendToHandler;
};


celix_status_t dataStoreProxy_store(data_store_type* data_store, struct result workResult, bool *resultStored);
celix_status_t dataStoreProxy_storeAll(data_store_type *store, struct result *results, uint32_t size, uint32_t *storedResults);

celix_status_t dataStoreProxy_registerProxyService(void* proxyFactoryService, endpoint_description_pt endpoint, void* handler, sendToHandle callback);
celix_status_t dataStoreProxy_unregisterProxyService(void* proxyFactoryService, endpoint_description_pt endpoint);

#endif /* DATA_STORE_PROXY_IMPL_H_ */
