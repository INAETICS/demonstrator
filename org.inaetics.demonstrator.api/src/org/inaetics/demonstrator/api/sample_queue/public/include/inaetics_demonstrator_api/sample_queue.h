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
#ifndef __INAETICS_DEMONSTATOR_API__SAMPLE_QUEUE_H_
#define __INAETICS_DEMONSTATOR_API__SAMPLE_QUEUE_H_

#include <stdint.h>
#include <stdbool.h>
#include "inaetics_demonstrator_api/sample.h"

#define INAETICS_DEMONSTATOR_API__SAMPLE_QUEUE_SERVICE_NAME "org.inaetics.demonstator.api.SampleQueue"

typedef struct sample_queue sample_queue_type; //ADT

struct sample_queue_service {
	sample_queue_type *sampleQueue;

	int (*put)(sample_queue_type *sampleQueue, struct sample sample, bool *sampleTaken);

	int (*putAll)(sample_queue_type *sampleQueue, struct sample *samples, uint32_t size, uint32_t *samplesTaken);

	int (*take)(sample_queue_type *sampleQueue, struct sample *sample);

	int (*takeAll)(sample_queue_type *sampleQueue, uint32_t min, uint32_t max, struct sample **samples, uint32_t *samplesSize);
};

#endif
