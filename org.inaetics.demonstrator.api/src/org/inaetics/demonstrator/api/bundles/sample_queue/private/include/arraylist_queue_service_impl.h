
#ifndef QUEUE_SERVICE_IMPL_H_
#define QUEUE_SERVICE_IMPL_H_

#include "celix_errno.h"
#include "inaetics_demonstrator_api/sample_queue.h"
#include <pthread.h>

struct sample_queue{
	pthread_mutex_t lock;
	array_list_pt queue;
	bool service_active;
};

celix_status_t queueService_create(struct sample_queue_service** qService, sample_queue_type** qHandler);
celix_status_t queueService_destroy(struct sample_queue_service* qService, sample_queue_type* qHandler);

int queueService_put(sample_queue_type *sampleQueue, struct sample sample, bool *sampleTaken);
int queueService_putAll(sample_queue_type *sampleQueue, struct sample *samples, uint32_t size, uint32_t *samplesTaken); 
int queueService_take(sample_queue_type *sampleQueue, struct sample *sample);
int queueService_takeAll(sample_queue_type *sampleQueue, uint32_t min, uint32_t max, struct sample **samples, uint32_t *samplesSize);


#endif
