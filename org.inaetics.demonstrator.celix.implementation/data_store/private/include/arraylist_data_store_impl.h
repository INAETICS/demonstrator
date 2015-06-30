/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
#ifndef DATA_STORE_SERVICE_IMPL_H_
#define DATA_STORE_SERVICE_IMPL_H_

#include <pthread.h>

#include "celix_errno.h"
#include "array_list.h"
#include "inaetics_demonstrator_api/data_store.h"

typedef struct data_store data_store_type;


celix_status_t dataStore_create(char* name, data_store_type** dataStore);
celix_status_t dataStore_destroy(data_store_type* dataStore);

int dataStore_store(data_store_type *dataStore, struct result result, bool *resultStored);
int dataStore_storeAll(data_store_type *dataStore, struct result *results, uint32_t size, uint32_t *storedResult);

int dataStore_getUtilizationStatsName(data_store_type *dataStore, char **name);
int dataStore_getUtilizationStatsType(data_store_type *dataStore, char **type);
int dataStore_getUtilizationStatsValue(data_store_type *dataStore, double* statVal);
int dataStore_getUtilizationStatsMeasurementUnit(data_store_type *dataStore, char **mUnit);

#endif
