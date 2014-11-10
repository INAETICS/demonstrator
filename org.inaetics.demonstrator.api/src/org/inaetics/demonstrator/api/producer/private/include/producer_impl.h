#ifndef PRODUCER_IMPL_H_
#define PRODUCER_IMPL_H_

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


#define SINGLE_SAMPLES_PER_SEC 	1000
#define BURST_SAMPLES_PER_SEC 	1000

#define MIN_BURST_LEN 			2
#define MAX_BURST_LEN 			10

#define VERBOSE					1
#define WAIT_TIME_SECONDS       5


struct activator {
        service_tracker_pt tracker;
        array_list_pt queueServices;
	bool running;
	pthread_t worker;
	pthread_mutex_t queueLock;
	pthread_cond_t queueAvailable;
};


void *produceSamples(void *handle);

#endif
