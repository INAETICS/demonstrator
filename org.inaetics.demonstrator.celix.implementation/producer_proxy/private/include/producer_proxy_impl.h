/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */


#ifndef PRODUCER_PROXY_IMPL_H_
#define PRODUCER_PROXY_IMPL_H_

#include "celix_errno.h"

#include "remote_proxy.h"
#include "constants.h"
#include "bundle_context.h"
#include "hash_map.h"

#include "endpoint_listener.h"
#include "inaetics_demonstrator_api/producer.h"

typedef struct producer* producer_type_pt;

struct producer {
	bundle_context_pt context;

	endpoint_description_pt endpoint;
	sendToHandle sendToCallback;
	void * sendToHandler;
};

celix_status_t producerProxy_create(bundle_context_pt context, producer_type_pt* queue);
celix_status_t producerProxy_destroy(producer_type_pt* queue);

int producerProxy_getMaxSampleRate(void* producer);
int producerProxy_getSampleRate(void* producer);
void producerProxy_setSampleRate(void* producer, int sampleRate);

#endif /* PRODUCER_PROXY_IMPL_H_ */
