#include <stdlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <stdbool.h>

#include "array_list.h"
#include "celix_errno.h"

#include "arraylist_sample_queue_impl.h"
#include <android/log.h>

#define  LOG_TAG    "celix"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)


#define MAX_QUEUE_SIZE       	10000

#define WAIT_TIME_SECONDS       2
#define VERBOSE					1

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
	array_list_pt queue;
	long takeCnt;
	long putCnt;
	volatile long currentQueueSize;
	long max_queue_size;
};


void *printStatistics(void *handle);

static void msg(int lvl, char *fmsg, ...) {
	if (lvl <= VERBOSE) {
		char msg[512];
		va_list listPointer;
		va_start(listPointer, fmsg);
		vsprintf(msg, fmsg, listPointer);

		LOGI("[%d] : %s\n", lvl, msg);
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

		pthread_mutex_init(&(sampleQueue->lock), NULL);
		pthread_cond_init(&sampleQueue->listEmpty, NULL);

		arrayList_create(&(sampleQueue->queue));

		sampleQueue->putCnt = 0;
		sampleQueue->takeCnt = 0;
		sampleQueue->max_queue_size = MAX_QUEUE_SIZE;
		sampleQueue->currentQueueSize = 0;
		sampleQueue->statisticsRunning = false;

		pthread_create(&sampleQueue->statistics, NULL, printStatistics, sampleQueue);

		(*result) = sampleQueue;
		status = CELIX_SUCCESS;
	}

	return status;
}

celix_status_t sampleQueue_destroy(sample_queue_type* sampleQueue) {

	celix_status_t status = CELIX_SUCCESS;
	int i = 0;

	sampleQueue->statisticsRunning = false;

	pthread_mutex_lock(&(sampleQueue->lock));

	/* Empty the queue */
	for (; i < arrayList_size(sampleQueue->queue); i++) {
		free(arrayList_get(sampleQueue->queue, i));
	}

	/* Destroy the queue */
	arrayList_destroy(sampleQueue->queue);
	sampleQueue->queue = NULL;

	pthread_cond_broadcast(&(sampleQueue->listEmpty));

	pthread_cond_destroy(&(sampleQueue->listEmpty));

	pthread_mutex_unlock(&(sampleQueue->lock));
	pthread_mutex_destroy(&(sampleQueue->lock));

	void *exitStatus = NULL;
	pthread_join(sampleQueue->statistics, &exitStatus);


	free(sampleQueue->name);
	free(sampleQueue->utilizationStatsName);
	free(sampleQueue);

	return status;

}

int sampleQueue_put(sample_queue_type *sampleQueue, struct sample sample, bool *sampleTaken) {

	celix_status_t status = CELIX_SUCCESS;

	pthread_mutex_lock(&sampleQueue->lock);

	if (sampleQueue->queue != NULL) {

		if ((MAX_QUEUE_SIZE == 0) || arrayList_size(sampleQueue->queue) < MAX_QUEUE_SIZE)
		{
			struct sample* localSample = calloc(1, sizeof(struct sample));

			if (localSample != NULL) {
				memcpy(localSample, &sample, sizeof(struct sample));
				bool ret = arrayList_add(sampleQueue->queue, localSample);
				if (ret) {
					sampleQueue->putCnt++;
					sampleQueue->currentQueueSize += 1;
					msg(3, "SAMPLE_QUEUE: Added sample {%llu | %f | %f }to queue\n", localSample->time, localSample->value1, localSample->value2);
					pthread_cond_signal(&sampleQueue->listEmpty);
				}
				else {
					free(localSample);
				}

				*sampleTaken = ret;
			}
			else {
				status = CELIX_ENOMEM;
			}
		}

	}
	else {
		msg(0, "SAMPLE_QUEUE: put denied because service is removed");
		status = CELIX_ILLEGAL_STATE;
	}

	pthread_mutex_unlock(&sampleQueue->lock);

	return (int) status;
}

int sampleQueue_putAll(sample_queue_type *sampleQueue, struct sample *samples, uint32_t size, uint32_t *samplesTaken) {

	celix_status_t status = CELIX_SUCCESS;
	uint32_t i = 0;
	uint32_t samples_added = 0;

	pthread_mutex_lock(&sampleQueue->lock);

	if (sampleQueue->queue != NULL) {


		msg(3, "SAMPLE_QUEUE: Adding a burst of %u samples\n", size);

		for (; (i < size) && ((MAX_QUEUE_SIZE == 0) || (arrayList_size(sampleQueue->queue) < MAX_QUEUE_SIZE)); i++) {
			struct sample* s = calloc(1, sizeof(struct sample));

			if (s != NULL) {

				memcpy(s, &samples[i], sizeof(struct sample));

				if (arrayList_add(sampleQueue->queue, s)) {
					msg(3, "\tSAMPLE_QUEUE: Added sample {%llu | %f | %f } to queue\n", s->time, s->value1, s->value2);
					samples_added++;
					sampleQueue->currentQueueSize += 1;
					pthread_cond_signal(&sampleQueue->listEmpty);
				}
				else {
					free(s);
				}
			}
			else {
				status = CELIX_ENOMEM;
				break;
			}

		}

		msg(3, "SAMPLE_QUEUE: End burst\n");


		*samplesTaken = samples_added;
		sampleQueue->putCnt += samples_added;

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

	pthread_mutex_unlock(&sampleQueue->lock);

	return (int) status;

}

int sampleQueue_take(sample_queue_type *sampleQueue, struct sample *sample) {
	celix_status_t status = CELIX_ILLEGAL_STATE;
	struct timespec ts;
	int rc = 0;

	clock_gettime(CLOCK_REALTIME, &ts);
	ts.tv_sec += WAIT_TIME_SECONDS;

	pthread_mutex_lock(&sampleQueue->lock);

	while (sampleQueue->queue!=NULL && arrayList_size(sampleQueue->queue) == 0 && rc != ETIMEDOUT) {
		rc = pthread_cond_timedwait(&sampleQueue->listEmpty, &sampleQueue->lock, &ts);
	}

	if (rc == 0 && sampleQueue->queue!=NULL && arrayList_size(sampleQueue->queue)>0)
	{
		struct sample *tmpSample = arrayList_remove(sampleQueue->queue, 0);
		memcpy(sample, tmpSample, sizeof(struct sample));
		free(tmpSample);
		sampleQueue->takeCnt++;
		sampleQueue->currentQueueSize -= 1;
		status = CELIX_SUCCESS;
	}


	pthread_mutex_unlock(&sampleQueue->lock);


	return (int) status;
}

int sampleQueue_takeAll(sample_queue_type *sampleQueue, uint32_t min, uint32_t max, struct sample **samples, uint32_t *samplesSize) {

	celix_status_t status = CELIX_ILLEGAL_STATE;
	struct timespec ts;
	int i = 0;
	int rc = 0;

	clock_gettime(CLOCK_REALTIME, &ts);
	ts.tv_sec += WAIT_TIME_SECONDS;

	pthread_mutex_lock(&sampleQueue->lock);

	/* block, till sufficient elements available */
	while (sampleQueue->queue!=NULL && arrayList_size(sampleQueue->queue) < min && rc != ETIMEDOUT) {
		rc = pthread_cond_timedwait(&sampleQueue->listEmpty, &sampleQueue->lock, &ts);
	}

	if (rc == 0 && sampleQueue->queue!=NULL) {
		status = CELIX_SUCCESS;
		for (i = 0; i < max && arrayList_size(sampleQueue->queue) > 0; i++) {
			struct sample *tmpSample = arrayList_remove(sampleQueue->queue, 0);
			memcpy(samples[i], tmpSample, sizeof(struct sample));
			free(tmpSample);
		}
	}

	*samplesSize = i;
	sampleQueue->takeCnt += i;
	sampleQueue->currentQueueSize -= i;


	pthread_mutex_unlock(&sampleQueue->lock);

	return (int) status;
}

int sampleQueue_getUtilizationStatsName(sample_queue_type *sampleQueue, char **name) {
	(*name) = strdup(sampleQueue->utilizationStatsName);
	return 0;
}

int sampleQueue_getUtilizationStatsType(sample_queue_type *sampleQueue, char **type) {
	(*type) = strdup((char *) UTILIZATION_TYPE);
	return 0;
}

int sampleQueue_getUtilizationStatsValue(sample_queue_type *sampleQueue, double* statVal) {
	//Note; read only access to maxQueueSize and currentQueueSize with no special need for precise value -> no synchronization needed.
	(*statVal)=(double)((((double)sampleQueue->currentQueueSize)/((double)sampleQueue->max_queue_size))*100.0f);
	return 0;
}

int sampleQueue_getUtilizationStatsMeasurementUnit(sample_queue_type *sampleQueue, char **mUnit) {
	(*mUnit) = strdup((char *) UTILIZATION_MEASUREMENT_UNIT);
	return 0;
}

void *printStatistics(void *handle) {
	sample_queue_type* sampleQueue = (sample_queue_type*) handle;
	sampleQueue->statisticsRunning = true;
	while (sampleQueue->statisticsRunning) {
		// TODO: add lock
		msg(1, "QUEUE: \tsamples put: %d \tsamples taken: %d \tqueue size: %d", sampleQueue->putCnt, sampleQueue->takeCnt, arrayList_size(sampleQueue->queue));
		sampleQueue->putCnt = 0;
		sampleQueue->takeCnt = 0;
		sleep(2);
	}

	pthread_exit(NULL);
	return NULL;
}
