#ifndef QUEUE_SERVICE_STATISTICS_IMPL_H_
#define QUEUE_SERVICE_STATISTICS_IMPL_H_

#include "celix_errno.h"

#include "inaetics_demonstrator_api/service_statistics.h"
#include "arraylist_data_store_impl.h"

#include <pthread.h>

#define DS_STATS_NAME_PREFIX "Celix Data Store"
#define DS_STATS_TYPE "datastore load"
#define DS_STATS_UNIT "%"

struct service_statistics {
	struct data_store_service* dsService;
	char* service_stats_name;
	char* service_stats_type;
	char* service_stats_measurementUnit;
};

celix_status_t dataStoreStatService_create(struct service_statistics_service** dsStatService, struct data_store_service* dsService, char* fwUUID);
celix_status_t dataStoreStatService_destroy(struct service_statistics_service* dsStatService);

int dataStoreStatService_getServiceStatName(service_statistics_type *serviceStatistics, char **name);
int dataStoreStatService_getServiceStatType(service_statistics_type *serviceStatistics, char **type);
int dataStoreStatService_getServiceStatVal(service_statistics_type *serviceStatistics, double* statVal);
int dataStoreStatService_getServiceStatUnit(service_statistics_type *serviceStatistics, char **mUnit);

#endif
