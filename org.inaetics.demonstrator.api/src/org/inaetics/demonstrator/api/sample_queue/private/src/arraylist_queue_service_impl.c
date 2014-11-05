#include <stdlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>

#include "array_list.h"
#include "celix_errno.h"

#include "arraylist_queue_service_impl.h"


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

celix_status_t queueService_create(struct sample_queue_service** qService, sample_queue_type** qHandler) {

	celix_status_t status= CELIX_ENOMEM;

	*qService = calloc(1, sizeof(struct sample_queue_service));
	*qHandler = calloc(1, sizeof(struct sample_queue));

	if( (*qService !=NULL) && (*qHandler!=NULL) ){

		pthread_mutex_init(&((*qHandler)->lock), NULL);
		pthread_cond_init(&(*qHandler)->listEmpty, NULL);

		arrayList_create(&((*qHandler)->queue));

		(*qHandler)->service_active = true;
		(*qHandler)->putCnt = 0;
		(*qHandler)->takeCnt = 0;
		(*qHandler)->statisticsRunning = false;

		(*qService)->sampleQueue = *qHandler;
		(*qService)->put = queueService_put;
		(*qService)->putAll = queueService_putAll;
		(*qService)->take = queueService_take;
		(*qService)->takeAll = queueService_takeAll;

		pthread_create(&(*qHandler)->statistics, NULL, printStatistics, *qHandler);

		status = CELIX_SUCCESS;
	}

	return status;
}

celix_status_t queueService_destroy(struct sample_queue_service* qService, sample_queue_type* qHandler){

	celix_status_t status = CELIX_SUCCESS;
	int i =0;

	qHandler->statisticsRunning = false;

	pthread_mutex_lock(&(qHandler->lock));

	/* Empty the queue */
	for(;i < arrayList_size(qHandler->queue); i++) {
		free(arrayList_get(qHandler->queue, i));
	}

	/* Destroy the queue */
	arrayList_destroy(qHandler->queue);
	qHandler->queue = NULL;

	qHandler->service_active=false;

	pthread_mutex_unlock(&(qHandler->lock));
	pthread_mutex_destroy(&(qHandler->lock));

	void *exitStatus = NULL;
	pthread_join(qHandler->statistics, &exitStatus);

	free(qService);
	free(qHandler);

	return status;

}

int queueService_put(sample_queue_type *sampleQueue, struct sample sample, bool *sampleTaken) {

	celix_status_t status = CELIX_SUCCESS;

	if(sampleQueue->service_active==true && sampleQueue->queue!=NULL){
		pthread_mutex_lock(&sampleQueue->lock);

		if ((MAX_QUEUE_SIZE == 0) || arrayList_size(sampleQueue->queue) < MAX_QUEUE_SIZE)
		{
			struct sample* localSample = calloc(1,sizeof(struct sample));

			if (localSample != NULL) {
				memcpy(localSample, &sample, sizeof(struct sample));
				bool ret = arrayList_add(sampleQueue->queue, localSample);
				if (ret) {
					sampleQueue->putCnt++;
					msg(3, "SAMPLE_QUEUE: Added sample {%llu | %f | %f }to queue\n",localSample->time,localSample->value1,localSample->value2);
					pthread_cond_signal(&sampleQueue->listEmpty);
				}
				else {
					free(localSample);
				}

				*sampleTaken=ret;
			}
			else {
				status=CELIX_ENOMEM;
			}
		}
		pthread_mutex_unlock(&sampleQueue->lock);
	}
	else {
		msg(0, "SAMPLE_QUEUE: put denied because service is removed");
		status=CELIX_ILLEGAL_STATE;
	}

	return (int)status;
}

int queueService_putAll(sample_queue_type *sampleQueue, struct sample *samples, uint32_t size, uint32_t *samplesTaken){

	celix_status_t status = CELIX_SUCCESS;
	uint32_t i=0;
	uint32_t samples_added=0;

	if ((sampleQueue->service_active == true) && (sampleQueue->queue != NULL)) {
		pthread_mutex_lock(&sampleQueue->lock);

		msg(3, "SAMPLE_QUEUE: Adding a burst of %u samples\n",size);

		for(; (i < size) && ((MAX_QUEUE_SIZE == 0) || (arrayList_size(sampleQueue->queue) < MAX_QUEUE_SIZE)); i++){
			struct sample* s = calloc(1,sizeof(struct sample));

			if (s!=NULL) {

				memcpy(s,&samples[i],sizeof(struct sample));

				if(arrayList_add(sampleQueue->queue, s)){
					msg(3, "\tSAMPLE_QUEUE: Added sample {%llu | %f | %f } to queue\n",s->time,s->value1,s->value2);
					samples_added++;
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
		pthread_mutex_unlock(&sampleQueue->lock);

		*samplesTaken=samples_added;
		sampleQueue->putCnt += samples_added;

		if(*samplesTaken!=size){
			msg(2, "SAMPLE_QUEUE: Could not add all the requested samples (requested:%u, added%u)\n",size,*samplesTaken);
			if(status==CELIX_SUCCESS) //Don't mask the ENOMEM
				status=CELIX_BUNDLE_EXCEPTION;
		}
	}
	else{
		msg(0, "SAMPLE_QUEUE: putAll denied because service is removed");
		status=CELIX_ILLEGAL_STATE;
	}

	return (int)status;


}

int queueService_take(sample_queue_type *sampleQueue, struct sample *sample){
	celix_status_t status = CELIX_ILLEGAL_STATE;
	struct timespec ts;
	int rc = 0;

	clock_gettime(CLOCK_REALTIME, &ts);
	ts.tv_sec += WAIT_TIME_SECONDS;

	pthread_mutex_lock(&sampleQueue->lock);

	while (arrayList_size(sampleQueue->queue) == 0 && rc != ETIMEDOUT) {
		rc = pthread_cond_timedwait(&sampleQueue->listEmpty, &sampleQueue->lock, &ts);
	}

	if (rc == 0)
	{
		struct sample *tmpSample = arrayList_remove(sampleQueue->queue, 0);
		memcpy(sample, tmpSample, sizeof(struct sample));
		free(tmpSample);
		sampleQueue->takeCnt++;
		status = CELIX_SUCCESS;
	}

	pthread_mutex_unlock(&sampleQueue->lock);

	return (int) status;
}


int queueService_takeAll(sample_queue_type *sampleQueue, uint32_t min, uint32_t max, struct sample **samples, uint32_t *samplesSize) {

	celix_status_t status = CELIX_ILLEGAL_STATE;
	struct timespec ts;
	int i, rc = 0;

	clock_gettime(CLOCK_REALTIME, &ts);
	ts.tv_sec += WAIT_TIME_SECONDS;

	pthread_mutex_lock(&sampleQueue->lock);

	/* block, till sufficient elements available */
	while (arrayList_size(sampleQueue->queue) < min && rc != ETIMEDOUT) {
		rc = pthread_cond_timedwait(&sampleQueue->listEmpty, &sampleQueue->lock, &ts);
	}

	if (rc == 0) {
		status = CELIX_SUCCESS;
		for(i = 0; i < max && arrayList_size(sampleQueue->queue) > 0 ; i++) {
			struct sample *tmpSample = arrayList_remove(sampleQueue->queue, 0);
			memcpy(samples[i], tmpSample, sizeof(struct sample));
			free(tmpSample);
		}
	}

	*samplesSize = i;
	sampleQueue->takeCnt += i;
	pthread_mutex_unlock(&sampleQueue->lock);

	return (int) status;
}


void *printStatistics(void *handle) {
	sample_queue_type* qHandler = handle;
	qHandler->statisticsRunning = true;
	while (qHandler->statisticsRunning) {
		// TODO: add lock
		msg(1, "QUEUE: \tsamples put: %d \tsamples taken: %d \tqueue size: %d", qHandler->putCnt, qHandler->takeCnt, arrayList_size(qHandler->queue));
		qHandler->putCnt = 0;
		qHandler->takeCnt = 0;
		sleep(2);
	}

	pthread_exit(NULL);
	return NULL;
}
