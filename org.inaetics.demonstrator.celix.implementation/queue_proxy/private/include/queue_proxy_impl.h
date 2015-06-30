

#ifndef QUEUE_PROXY_IMPL_H_
#define QUEUE_PROXY_IMPL_H_

#include "celix_errno.h"

#include "remote_proxy.h"
#include "constants.h"
#include "bundle_context.h"
#include "hash_map.h"

#include "endpoint_listener.h"
#include "inaetics_demonstrator_api/sample_queue.h"

typedef struct sample_queue sample_queue_type;

struct sample_queue {
	bundle_context_pt context;

	endpoint_description_pt endpoint;
	sendToHandle sendToCallback;
	void * sendToHandler;
};


celix_status_t queueProxy_create(bundle_context_pt context, sample_queue_type** queue);
celix_status_t queueProxy_destroy(sample_queue_type** queue);

int queueProxy_put(void* queue, struct sample workSample, bool *sampleTaken);
int queueProxy_putAll(void* queue, struct sample *samples, uint32_t size, uint32_t *samplesTaken);
int queueProxy_take(void* queue, struct sample *sample);
int queueProxy_takeAll(void* queue, uint32_t min, uint32_t max, struct sample **samples, uint32_t *samplesSize);

#endif /* QUEUE_PROXY_IMPL_H_ */
