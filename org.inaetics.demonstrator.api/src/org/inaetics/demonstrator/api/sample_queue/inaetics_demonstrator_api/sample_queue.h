/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
#ifndef __INAETICS_DEMONSTRATOR_API__SAMPLE_QUEUE_H_
#define __INAETICS_DEMONSTRATOR_API__SAMPLE_QUEUE_H_

#include <stdint.h>
#include <stdbool.h>
#include "inaetics_demonstrator_api/sample.h"

#define INAETICS_DEMONSTRATOR_API__SAMPLE_QUEUE_SERVICE_NAME "org.inaetics.demonstrator.api.queue.SampleQueue"

typedef struct sample_queue sample_queue_type; //ADT

struct sample_queue_service {
	sample_queue_type *sampleQueue;

	int (*put)(sample_queue_type *sampleQueue, struct sample sample, bool *sampleTaken);

	int (*putAll)(sample_queue_type *sampleQueue, struct sample *samples, uint32_t size, uint32_t *samplesTaken);

	int (*take)(sample_queue_type *sampleQueue, struct sample *sample);

	int (*takeAll)(sample_queue_type *sampleQueue, uint32_t min, uint32_t max, struct sample **samples, uint32_t *samplesSize);
};

#endif
