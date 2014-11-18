
#ifndef QUEUE_SERVICE_IMPL_H_
#define QUEUE_SERVICE_IMPL_H_

#include "celix_errno.h"
#include "inaetics_demonstrator_api/sample_queue.h"
#include <pthread.h>


#define MAX_QUEUE_SIZE       	10000

#define WAIT_TIME_SECONDS       2
#define VERBOSE					1


struct sample_queue {
	bool statisticsRunning;
	pthread_t statistics;
	pthread_mutex_t lock;
	pthread_cond_t listEmpty;
	array_list_pt queue;
	long takeCnt;
	long putCnt;
};

celix_status_t queueService_create(struct sample_queue_service** qService);
celix_status_t queueService_destroy(struct sample_queue_service* qService);

int queueService_put(sample_queue_type *sampleQueue, struct sample sample, bool *sampleTaken);
int queueService_putAll(sample_queue_type *sampleQueue, struct sample *samples, uint32_t size, uint32_t *samplesTaken); 
int queueService_take(sample_queue_type *sampleQueue, struct sample *sample);
int queueService_takeAll(sample_queue_type *sampleQueue, uint32_t min, uint32_t max, struct sample **samples, uint32_t *samplesSize);


#endif
