#include <stdlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <stdbool.h>

#include "splice_data_queue.h"
#include "celix_errno.h"

#include "splice_sample_queue_impl.h"

#define MAX_QUEUE_SIZE       	10000

#define WAIT_TIME_SECONDS       2
#define VERBOSE					1
#define QUEUE_DOMAIN_ID         DDS_DOMAIN_ID_DEFAULT 

#define UTILIZATION_NAME_POSTFIX 		" Statistics"
#define UTILIZATION_TYPE 				"(utilization)"
#define UTILIZATION_MEASUREMENT_UNIT	"%"


struct sample_queue {
    char *name;
    char *utilizationStatsName;
    volatile bool statisticsRunning;
    pthread_t statistics;
    pthread_mutex_t lock;
    pthread_cond_t listEmpty;
    splice_data_queue_handle queue;
    long takeCnt;
    long putCnt;
    volatile long currentQueueSize;
    long max_queue_size;
    long size;
    long sequence_number;
};


void *printStatistics(void *handle);

static void msg(int lvl, char *fmsg, ...) {
    if (lvl <= VERBOSE) {
        char msg[512];
        va_list listPointer;
        va_start(listPointer, fmsg);
        vsprintf(msg, fmsg, listPointer);

        printf("[%d] : %s\n", lvl, msg);
    }
}

celix_status_t sampleQueue_create(char *name, sample_queue_type **result) {

    celix_status_t status = CELIX_ENOMEM;
    sample_queue_type *sampleQueue = NULL;

    sampleQueue = calloc(1, sizeof(struct sample_queue));
    if (sampleQueue != NULL) {
        sampleQueue->name = strdup(name);
        sampleQueue->utilizationStatsName = calloc(1, strlen(name) + strlen(UTILIZATION_NAME_POSTFIX) + 1);
    }

    if (sampleQueue != NULL && sampleQueue->name != NULL && sampleQueue->utilizationStatsName != NULL) {
        sprintf(sampleQueue->utilizationStatsName, "%s%s", sampleQueue->name, (char*)UTILIZATION_NAME_POSTFIX);
/*
        pthread_mutex_init(&(sampleQueue->lock), NULL);
        pthread_cond_init(&sampleQueue->listEmpty, NULL);
*/
        sampleQueue->queue = splice_data_queue_create(QUEUE_DOMAIN_ID);

        sampleQueue->putCnt = 0;
        sampleQueue->takeCnt = 0;
        sampleQueue->max_queue_size = MAX_QUEUE_SIZE;
        sampleQueue->currentQueueSize = 0;
        sampleQueue->statisticsRunning = false;
        sampleQueue->size = 0;
        sampleQueue->sequence_number = 1;
        pthread_create(&sampleQueue->statistics, NULL, printStatistics, sampleQueue);

        (*result) = sampleQueue;
        status = CELIX_SUCCESS;
    }

    return status;
}

celix_status_t sampleQueue_destroy(sample_queue_type* sampleQueue) {

    celix_status_t status = CELIX_SUCCESS;

    sampleQueue->statisticsRunning = false;

 /*   pthread_mutex_lock(&(sampleQueue->lock));*/

    /* Empty the queue */
    /* TODO May be the queue have to be cleared or a destroy of the queue is enough. */

	/* Destroy the queue */
    splice_data_queue_destroy(sampleQueue->queue);
    sampleQueue->queue = NULL;
/*
    pthread_cond_broadcast(&(sampleQueue->listEmpty));

    pthread_cond_destroy(&(sampleQueue->listEmpty));

    pthread_mutex_unlock(&(sampleQueue->lock));
    pthread_mutex_destroy(&(sampleQueue->lock));
*/
    void *exitStatus = NULL;
    pthread_join(sampleQueue->statistics, &exitStatus);


    free(sampleQueue->name);
    free(sampleQueue->utilizationStatsName);
    free(sampleQueue);

    return status;

}

int sampleQueue_put(sample_queue_type *sampleQueue, struct sample sample, bool *sampleTaken) {

    celix_status_t status = CELIX_SUCCESS;
    queue_sample dds_sample;

/*    pthread_mutex_lock(&sampleQueue->lock);*/
    if (sampleQueue->queue != NULL)
    {
        if(sampleQueue->size < MAX_QUEUE_SIZE)
        {
            dds_sample.sequence_number = sampleQueue->sequence_number;
            sampleQueue->sequence_number++;
            dds_sample.time = sample.time;
            dds_sample.value1 = sample.value1;
            dds_sample.value2 = sample.value2;
            splice_data_queue_put(sampleQueue->queue, &dds_sample);
            sampleQueue->putCnt++;
            sampleQueue->size++;
            sampleQueue->currentQueueSize += 1;
            msg(3, "SAMPLE_QUEUE: Added sample {%llu | %f | %f }to queue\n", sample.time, sample.value1, sample.value2);
 /*           pthread_cond_signal(&sampleQueue->listEmpty);*/
            *sampleTaken = true;
        }
    }
    else {
        msg(0, "SAMPLE_QUEUE: put denied because service is removed");
        status = CELIX_ILLEGAL_STATE;
    }

/*    pthread_mutex_unlock(&sampleQueue->lock); */

    return (int) status;
}

int sampleQueue_putAll(sample_queue_type *sampleQueue, struct sample *samples, uint32_t size, uint32_t *samplesTaken) {

    celix_status_t status = CELIX_SUCCESS;
    uint32_t i = 0;
    uint32_t samples_added = 0;
    queue_sample dds_sample;

/*    pthread_mutex_lock(&sampleQueue->lock);*/
    if (sampleQueue->queue != NULL)
    {
        msg(3, "SAMPLE_QUEUE: Adding a burst of %u samples\n", size);

        for (i = 0; i < size && sampleQueue->size < MAX_QUEUE_SIZE; i++)
        {
            dds_sample.sequence_number = sampleQueue->sequence_number;
            sampleQueue->sequence_number++;
            dds_sample.time = samples[i].time;
            dds_sample.value1 = samples[i].value1;
            dds_sample.value2 = samples[i].value2;
 
            splice_data_queue_put(sampleQueue->queue, &dds_sample);
            msg(3, "\tSAMPLE_QUEUE: Added sample {%llu | %f | %f } to queue\n", samples[i].time, samples[i].value1, samples[i].value2);
            samples_added++;
            sampleQueue->currentQueueSize += 1;
/*            pthread_cond_signal(&sampleQueue->listEmpty); */
        }

        msg(3, "SAMPLE_QUEUE: End burst\n");

        *samplesTaken = samples_added;
        sampleQueue->putCnt += samples_added;
        sampleQueue->size += samples_added;

        if (*samplesTaken != size) {
            msg(2, "SAMPLE_QUEUE: Could not add all the requested samples (requested:%u, added%u)\n", size, *samplesTaken);
            if (status == CELIX_SUCCESS) //Don't mask the ENOMEM
                status = CELIX_BUNDLE_EXCEPTION;
        }
    }
    else {
        msg(0, "SAMPLE_QUEUE: putAll denied because service is removed");
        status = CELIX_ILLEGAL_STATE;
    }

/*    pthread_mutex_unlock(&sampleQueue->lock);*/

    return (int) status;
}

int sampleQueue_take(sample_queue_type *sampleQueue, struct sample *sample)
{
    celix_status_t status = CELIX_ILLEGAL_STATE;
    queue_sample dds_sample;
/*    pthread_mutex_lock(&sampleQueue->lock); */
    if (sampleQueue->queue != NULL)
    {
        splice_data_queue_get_one(sampleQueue->queue, &dds_sample);
        sample->time = dds_sample.time;
        sample->value1 = dds_sample.value1;
        sample->value2 = dds_sample.value2;
        sampleQueue->takeCnt++;
        sampleQueue->size--;
        sampleQueue->currentQueueSize -= 1;
        status = CELIX_SUCCESS;
    }

/*    pthread_mutex_unlock(&sampleQueue->lock); */

    return (int) status;
}

int sampleQueue_takeAll(sample_queue_type *sampleQueue, uint32_t min, uint32_t max, struct sample **samples, uint32_t *samplesSize) {

    celix_status_t status = CELIX_ILLEGAL_STATE;
    int num_samples = 0;
    int read_samples = 0;
    int max_num_samples = 0;
    int i = 0;
    queue_sample dds_samples[500];

/*    pthread_mutex_lock(&sampleQueue->lock); */

    if (sampleQueue->queue!=NULL)
    {
        status = CELIX_SUCCESS;
        do
        {
            max_num_samples = max - num_samples;
            read_samples = splice_data_queue_get_multiple(sampleQueue->queue, max_num_samples, dds_samples);
            num_samples = num_samples + read_samples;
            sampleQueue->size = sampleQueue->size - read_samples;
        }
        while(num_samples < min);
        for(i = 0; i < num_samples; i++)
        {
            samples[i]->time = dds_samples[i].time;
            samples[i]->value1 = dds_samples[i].value1;
            samples[i]->value2 = dds_samples[i].value2;
        }
    }
    *samplesSize = num_samples;
    sampleQueue->takeCnt += num_samples;
    sampleQueue->currentQueueSize -= num_samples;


/*    pthread_mutex_unlock(&sampleQueue->lock); */

    return (int) status;
}

int sampleQueue_getUtilizationStatsName(sample_queue_type *sampleQueue, char **name) {
    (*name) = sampleQueue->utilizationStatsName;
    return 0;
}

int sampleQueue_getUtilizationStatsType(sample_queue_type *sampleQueue, char **type) {
    (*type) = (char *)UTILIZATION_TYPE;
    return 0;
}

int sampleQueue_getUtilizationStatsValue(sample_queue_type *sampleQueue, double* statVal) {
    //Note; read only access to maxQueueSize and currentQueueSize with no special need for precise value -> no synchronization needed.
    (*statVal)=(double)((((double)sampleQueue->currentQueueSize)/((double)sampleQueue->max_queue_size))*100.0f);
    return 0;
}

int sampleQueue_getUtilizationStatsMeasurementUnit(sample_queue_type *sampleQueue, char **mUnit) {
    (*mUnit) = (char *)UTILIZATION_MEASUREMENT_UNIT;
    return 0;
}

void *printStatistics(void *handle) {
    sample_queue_type* sampleQueue = (sample_queue_type*) handle;
    sampleQueue->statisticsRunning = true;
    while (sampleQueue->statisticsRunning) {
        // TODO: add lock
        msg(1, "QUEUE: \tsamples put: %d \tsamples taken: %d \tqueue size: %d", sampleQueue->putCnt, sampleQueue->takeCnt, sampleQueue->size);
        sampleQueue->putCnt = 0;
        sampleQueue->takeCnt = 0;
        sleep(2);
    }

    pthread_exit(NULL);
    return NULL;
}
