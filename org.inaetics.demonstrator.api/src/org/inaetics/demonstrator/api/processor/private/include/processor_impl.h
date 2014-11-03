#ifndef PROCESSOR_IMPL_H_
#define PROCESSOR_IMPL_H_

#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>

#include <array_list.h>
#include <service_tracker.h>

#include "inaetics_demonstrator_api/sample_queue.h"


#define MIN_BURST_LEN 0
#define MAX_BURST_LEN 10

struct activator {
	service_tracker_pt tracker;
	array_list_pt queueServices;
	bool running;
	pthread_t worker;
};

void *processSamples(void *handle);

#endif /* PROCESSOR_IMPL_H */
