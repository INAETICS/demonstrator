
#ifndef QUEUE_SERVICE_IMPL_H_
#define QUEUE_SERVICE_IMPL_H_

#include "celix_errno.h"
#include "inaetics_demonstrator_api/sample_queue.h"
#include <pthread.h>

typedef struct sample_queue sample_queue_type;

celix_status_t sampleQueue_create(char *name, sample_queue_type **sampleQueue);
celix_status_t sampleQueue_destroy(sample_queue_type* sampleQueue);

//queue service
int sampleQueue_put(sample_queue_type *sampleQueue, struct sample *sample, bool *sampleTaken);
int sampleQueue_putAll(sample_queue_type *sampleQueue, struct sample_sequence samples, uint32_t *samplesTaken);
int sampleQueue_take(sample_queue_type *sampleQueue, struct sample **sample);
int sampleQueue_takeAll(sample_queue_type *sampleQueue, uint32_t min, uint32_t max, struct sample_sequence samples);

//stats provider service
int sampleQueue_getUtilizationStatsName(sample_queue_type *sampleQueue, char **name);
int sampleQueue_getUtilizationStatsType(sample_queue_type *sampleQueue, char **type);
int sampleQueue_getUtilizationStatsValue(sample_queue_type *sampleQueue, double* statVal);
int sampleQueue_getUtilizationStatsMeasurementUnit(sample_queue_type *sampleQueue, char **mUnit);

//stats provider service
//int sampleQueue_getThroughputStatsName(sample_queue_type *sampleQueue, char **name);
//int sampleQueue_getThroughputStatsType(sample_queue_type *sampleQueue, char **type);
//int SampleQueue_getThroughputStatsValue(sample_queue_type *sampleQueue, double* statVal);
//int sampleQueue_getThroughputStatsMeasurementUnit(sample_queue_type *sampleQueue, char **mUnit);


#endif
