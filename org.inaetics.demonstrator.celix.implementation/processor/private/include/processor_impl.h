#ifndef PROCESSOR_IMPL_H_
#define PROCESSOR_IMPL_H_

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
#include "inaetics_demonstrator_api/data_store.h"


typedef struct processor* processor_pt;

celix_status_t processor_create(char* name,processor_pt* processor);
celix_status_t processor_stop(processor_pt processor);
celix_status_t processor_destroy(processor_pt processor);

celix_status_t processor_queueServiceAdded(void *handle, service_reference_pt reference, void *service);
celix_status_t processor_queueServiceRemoved(void *handle, service_reference_pt reference, void *service);

celix_status_t processor_dataStoreServiceAdded(void *handle, service_reference_pt reference, void *service);
celix_status_t processor_dataStoreServiceRemoved(void *handle, service_reference_pt reference, void *service);

int processor_getUtilizationStatsName(processor_pt processor, char **name);
int processor_getUtilizationStatsType(processor_pt processor, char **type);
int processor_getUtilizationStatsValue(processor_pt processor, double* statVal);
int processor_getUtilizationStatsMeasurementUnit(processor_pt processor, char **mUnit);

#endif /* PROCESSOR_IMPL_H */
