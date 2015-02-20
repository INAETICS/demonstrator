/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */


#ifndef QUEUE_PROXY_IMPL_H_
#define QUEUE_PROXY_IMPL_H_

#include "celix_errno.h"

#include "remote_proxy.h"
#include "constants.h"
#include "bundle_context.h"
#include "hash_map.h"

#include "endpoint_listener.h"
#include "inaetics_demonstrator_api/sample_queue.h"

struct sample_queue {
	bundle_context_pt context;

	endpoint_description_pt endpoint;
	sendToHandle sendToCallback;
	void * sendToHandler;
};


celix_status_t queueProxy_put(sample_queue_type* queue, struct sample workSample, bool *sampleTaken);

celix_status_t queueProxy_registerProxyService(void* proxyFactoryService, endpoint_description_pt endpoint, void* handler, sendToHandle callback);
celix_status_t queueProxy_unregisterProxyService(void* proxyFactoryService, endpoint_description_pt endpoint);

#endif /* QUEUE_PROXY_IMPL_H_ */
