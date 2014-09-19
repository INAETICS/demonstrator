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
#ifndef __ORG_INAETICS_DEMONSTATOR_API__SAMPLE_PROCESSOR_H_
#define __ORG_INAETICS_DEMONSTATOR_API__SAMPLE_PROCESSOR_H_

#include <stdint.h>
#include <stdbool.h>
#include "org/inaetics/demonstrator/api/result.h"

#define org_inaetics_demonstator_api__sample_processor_SERVICE_NAME "org.inaetics.demonstator.api.SampleProcessor"

typedef struct org_inaetics_demonstator_api__sample_processor org_inaetics_demonstator_api__sample_processor_type; //ADT

struct org_inaetics_demonstator_api__sample_processor_service {
	org_inaetics_demonstator_api__sample_processor_type sampleProcessor;

	int (*take)(org_inaetics_demonstator_api__sample_queue_type sampleQueue, struct org_inaetics_demonstator_api__result *result);

	int (*takeAll)(org_inaetics_demonstator_api__sample_queue_type sampleQueue, uint32_t min, uint32_t max, struct org_inaetics_demonstator_api__result **results, uint32_t *resultSize);
};

#endif
