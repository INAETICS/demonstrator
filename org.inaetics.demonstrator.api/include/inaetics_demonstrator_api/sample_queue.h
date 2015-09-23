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
#ifndef __INAETICS_DEMONSTRATOR_API__SAMPLE_QUEUE_H_
#define __INAETICS_DEMONSTRATOR_API__SAMPLE_QUEUE_H_

#include <stdint.h>
#include <stdbool.h>
#include "inaetics_demonstrator_api/sample.h"

#define INAETICS_DEMONSTRATOR_API__SAMPLE_QUEUE_SERVICE_NAME "org.inaetics.demonstrator.api.queue.SampleQueue"

struct sample_sequence {
	uint32_t cap;
	uint32_t len;
	struct sample *buf;
};

struct sample_queue_service {
	void *sampleQueue;

	int (*put)(void *sampleQueue, struct sample *sample, bool *sampleTaken);

	int (*putAll)(void *sampleQueue, struct sample_sequence seq, uint32_t *samplesTaken);

	int (*take)(void *sampleQueue, struct sample **sample);

	int (*takeAll)(void *sampleQueue, uint32_t min, uint32_t max, struct sample_sequence **out);

};

#endif
