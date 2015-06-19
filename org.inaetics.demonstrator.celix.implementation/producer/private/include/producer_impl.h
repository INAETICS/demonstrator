#ifndef PRODUCER_IMPL_H_
#define PRODUCER_IMPL_H_

#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>

#include <array_list.h>
#include <service_tracker.h>

#include "inaetics_demonstrator_api/sample_queue.h"

typedef struct producer* producer_pt;

celix_status_t producer_create(char* name, producer_pt* producer);
celix_status_t producer_stop(producer_pt producer);
celix_status_t producer_destroy(producer_pt producer);

celix_status_t producer_queueServiceAdded(void *handle, service_reference_pt reference, void *service);
celix_status_t producer_queueServiceRemoved(void *handle, service_reference_pt reference, void *service);

int producer_getUtilizationStatsName(producer_pt producer, char **name);
int producer_getUtilizationStatsType(producer_pt producer, char **type);
int producer_getUtilizationStatsValue(producer_pt producer, double* statVal);
int producer_getUtilizationStatsMeasurementUnit(producer_pt producer, char **mUnit);

int producer_getMaxSampleRate(producer_pt producer);
int producer_getSampleRate(producer_pt producer);
void producer_setSampleRate(producer_pt producer, int rate);


#endif
