#ifndef QUEUE_SERVICE_STATISTICS_IMPL_H_
#define QUEUE_SERVICE_STATISTICS_IMPL_H_

#include "celix_errno.h"

#include "inaetics_demonstrator_api/service_statistics.h"
#include "arraylist_queue_service_impl.h"

#include <pthread.h>

#define QUEUE_STATS_NAME_PREFIX "Celix Sample Queue"
#define QUEUE_STATS_TYPE "queue load"
#define QUEUE_STATS_UNIT "%"

struct service_statistics {
	struct sample_queue_service* qService;
	char* service_stats_name;
	char* service_stats_type;
	char* service_stats_measurementUnit;
};

celix_status_t queueStatService_create(struct service_statistics_service** qStatService, struct sample_queue_service* qService, char* fwUUID);
celix_status_t queueStatService_destroy(struct service_statistics_service* qStatService);

int queueStatService_getServiceStatName(service_statistics_type *serviceStatistics, char **name);
int queueStatService_getServiceStatType(service_statistics_type *serviceStatistics, char **type);
int queueStatService_getServiceStatVal(service_statistics_type *serviceStatistics, double* statVal);
int queueStatService_getServiceStatUnit(service_statistics_type *serviceStatistics, char **mUnit);

#endif
