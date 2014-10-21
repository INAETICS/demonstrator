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



#define SAMPLE_STRING_MAX_LENGTH 2048
#define SINGLE_PUT_NUMBER 5
#define SAMPLE_GEN_PERIOD 1
#define MIN_BURST_LEN 2
#define MAX_BURST_LEN 10

struct localSample{
	uint64_t time; //milliseconds since unix epoch
	double value1;
	double value2;
};

struct activator {
	service_tracker_pt tracker;
	array_list_pt queueServices;
	bool running;
	pthread_t worker;
};


void *produceSamples(void *handle);

#endif
