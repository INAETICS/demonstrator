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
#include "inaetics_demonstrator_api/data_store.h"


#define SINGLE_SAMPLES_PER_SEC  250
#define BURST_SAMPLES_PER_SEC 	250

#define MIN_BURST_LEN 			2
#define MAX_BURST_LEN 			10

#define VERBOSE					1
#define WAIT_TIME_SECONDS       2

struct activator {
        /*QueueService stuffs*/
        service_tracker_pt queueTracker;
        array_list_pt queueServices;
        pthread_mutex_t queueLock;
        pthread_cond_t queueAvailable;
        /*DataStore stuffs*/
        service_tracker_pt dataStoreTracker;
        array_list_pt dataStoreServices;
        pthread_mutex_t dataStoreLock;
        pthread_cond_t dataStoreAvailable;
        /*Processing thread stuffs*/
        bool running;
        pthread_t worker;
};

void *processSamples(void *handle);

#endif /* PROCESSOR_IMPL_H */
