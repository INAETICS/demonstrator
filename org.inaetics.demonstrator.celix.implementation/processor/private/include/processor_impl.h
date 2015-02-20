/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
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


#define SINGLE_SAMPLES_PER_SEC  250
#define BURST_SAMPLES_PER_SEC 	250

#define MIN_BURST_LEN 			2
#define MAX_BURST_LEN 			10

#define VERBOSE					1
#define WAIT_TIME_SECONDS       2

typedef struct processor* processor_pt;

celix_status_t processor_create(processor_pt* processor);
celix_status_t processor_stop(processor_pt processor);
celix_status_t processor_destroy(processor_pt processor);

celix_status_t processor_queueServiceAdded(void *handle, service_reference_pt reference, void *service);
celix_status_t processor_queueServiceRemoved(void *handle, service_reference_pt reference, void *service);

celix_status_t processor_dataStoreServiceAdded(void *handle, service_reference_pt reference, void *service);
celix_status_t processor_dataStoreServiceRemoved(void *handle, service_reference_pt reference, void *service);

#endif /* PROCESSOR_IMPL_H */
