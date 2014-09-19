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
#ifndef __ORG_INAETICS_DEMONSTATOR_API__DATA_STORE_H_
#define __ORG_INAETICS_DEMONSTATOR_API__DATA_STORE_H_

#include <stdint.h>
#include <stdbool.h>
#include "org/inaetics/demonstrator/api/result.h"

#define ORG_INAETICS_DEMONSTRATOR_API__DATA_STORE_SERVICE_NAME "org.inaetics.demonstator.api.DataStore"

typedef struct org_inaetics_demonstator_api__data_store org_inaetics_demonstator_api__sample_processor_type; //ADT

struct org_inaetics_demonstator_api__data_store_service {
	org_inaetics_demonstator_api__data_store_type *dataStore;

	int (*findResultsBetween)(org_inaetics_demonstator_api__sample_processor_type *dataStore, uint64_t begin, uint64_t end, struct org_inaetics_demonstator_api__result **results, uint32_t *resultSize);
};

#endif
