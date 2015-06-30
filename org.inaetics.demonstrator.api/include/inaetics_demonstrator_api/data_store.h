/*
 * Copyright (c) 2014 INAETICS, <www.inaetics.org>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef __INAETICS_DEMONSTRATOR_API__DATA_STORE_H_
#define __INAETICS_DEMONSTRATOR_API__DATA_STORE_H_

#include <stdint.h>
#include <stdbool.h>
#include "inaetics_demonstrator_api/result.h"

#define INAETICS_DEMONSTRATOR_API__DATA_STORE_SERVICE_NAME "org.inaetics.demonstrator.api.datastore.DataStore"

struct data_store_service {
	void *dataStore;

	int (*store)(void *dataStore, struct result result, bool *resultStored);

	int (*storeAll)(void *dataStore, struct result *results, uint32_t size, uint32_t *storedResult);

	int (*findResultsBetween)(void *dataStore, uint64_t begin, uint64_t end, struct result **results, uint32_t *resultSize);

};

#endif
