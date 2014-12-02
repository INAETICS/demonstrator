#ifndef STATISTIC_TRACKER_IMPL_H_
#define STATISTIC_TRACKER_IMPL_H_

#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>

#include <array_list.h>
#include <service_tracker.h>

#include "inaetics_demonstrator_api/stats_provider.h"


#define VERBOSE					1
#define WAIT_TIME_SECONDS       1


typedef struct statistic_tracker* statistic_tracker_pt;

celix_status_t statistic_tracker_create(statistic_tracker_pt* statTracker);
celix_status_t statistic_tracker_stop(statistic_tracker_pt statTracker);
celix_status_t statistic_tracker_destroy(statistic_tracker_pt statTracker);

celix_status_t statistic_tracker_statServiceAdded(void *handle, service_reference_pt reference, void *service);
celix_status_t statistic_tracker_statServiceRemoved(void *handle, service_reference_pt reference, void *service);


#endif
