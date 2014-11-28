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


#define SINGLE_SAMPLES_PER_SEC 	1000
#define BURST_SAMPLES_PER_SEC 	1000

#define MIN_BURST_LEN 			2
#define MAX_BURST_LEN 			10

#define VERBOSE					1
#define WAIT_TIME_SECONDS       5


typedef struct producer* producer_pt;

celix_status_t producer_create(producer_pt* producer);
celix_status_t producer_stop(producer_pt producer);
celix_status_t producer_destroy(producer_pt producer);

celix_status_t producer_queueServiceAdded(void *handle, service_reference_pt reference, void *service);
celix_status_t producer_queueServiceRemoved(void *handle, service_reference_pt reference, void *service);


#endif
