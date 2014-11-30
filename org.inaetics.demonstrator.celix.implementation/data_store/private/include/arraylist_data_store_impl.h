#ifndef DATA_STORE_SERVICE_IMPL_H_
#define DATA_STORE_SERVICE_IMPL_H_

#include <pthread.h>

#include "celix_errno.h"
#include "array_list.h"
#include "inaetics_demonstrator_api/data_store.h"

#define MAX_STORE_SIZE       	100000

#define WAIT_TIME_SECONDS       2
#define VERBOSE					3

typedef struct data_store data_store_type;

struct data_store {
	pthread_mutex_t lock;
	pthread_cond_t listEmpty;
	array_list_pt store;
};

celix_status_t dataStoreService_create(struct data_store_service** dsService);
celix_status_t dataStoreService_destroy(struct data_store_service* dsService);

int dataStoreService_store(data_store_type *dataStore, struct result result, bool *resultStored);
int dataStoreService_storeAll(data_store_type *dataStore, struct result *results, uint32_t size, uint32_t *storedResult);
int dataStoreService_getLoad(data_store_type *dataStore, double* storeLoad);

#endif
