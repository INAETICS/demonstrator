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


typedef struct data_store data_store_type;

struct data_store {
	bundle_context_pt context;
	endpoint_description_pt endpoint;
	sendToHandle sendToCallback;
	void * sendToHandler;
};

celix_status_t dataStoreProxy_create(bundle_context_pt context, data_store_type **data_store);
celix_status_t dataStoreProxy_destroy( data_store_type **data_store);

int dataStoreProxy_store(void* store, struct result workResult, bool *resultStored);
int dataStoreProxy_storeAll(void* store, struct result *results, uint32_t size, uint32_t *storedResults);

#endif /* DATA_STORE_PROXY_IMPL_H_ */
