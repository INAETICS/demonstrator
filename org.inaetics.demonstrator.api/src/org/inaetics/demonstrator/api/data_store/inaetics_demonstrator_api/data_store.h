/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
#ifndef __INAETICS_DEMONSTRATOR_API__DATA_STORE_H_
#define __INAETICS_DEMONSTRATOR_API__DATA_STORE_H_

#include <stdint.h>
#include <stdbool.h>
#include "inaetics_demonstrator_api/result.h"

#define INAETICS_DEMONSTRATOR_API__DATA_STORE_SERVICE_NAME "org.inaetics.demonstrator.api.datastore.DataStore"

typedef struct data_store data_store_type; //ADT

struct data_store_service {
	data_store_type *dataStore;

	int (*store)(data_store_type *dataStore, struct result result, bool *resultStored);

	int (*storeAll)(data_store_type *dataStore, struct result *results, uint32_t size, uint32_t *storedResult);

	int (*findResultsBetween)(data_store_type *dataStore, uint64_t begin, uint64_t end, struct result **results, uint32_t *resultSize);
};

#endif
