/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
#ifndef INAETICS_DEMONSTRATOR_API__PRODUCER_H_
#define INAETICS_DEMONSTRATOR_API__PRODUCER_H_

#define INAETICS_DEMONSTRATOR_API__PRODUCER_SERVICE_NAME "org.inaetics.demonstrator.api.producer.Producer"

struct producer_service {
	void *producer;

	int (*getMaxSampleRate)(void* producer);
	int (*getSampleRate)(void* producer);
	void (*setSampleRate)(void* producer, int rate);
};

typedef struct producer_service* producer_service_pt;


#endif /* INAETICS_DEMONSTRATOR_API__PRODUCER_H_ */
