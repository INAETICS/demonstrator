#include <stdarg.h>
#include <time.h>

#include "processor_impl.h"

static void msg(int lvl, char *fmsg, ...) {
	if (lvl <= VERBOSE) {
		char msg[512];
		va_list listPointer;
		va_start(listPointer, fmsg);
		vsprintf(msg, fmsg, listPointer);

		printf("[%d] : %s\n", lvl, msg);
        }
}

static void processSample(struct sample* sample, struct result* result){

        result->time=sample->time;
        result->value1 = sample->value1 + sample->value2;
        memcpy(&(result->sample),sample,sizeof(struct sample));

}

void *processSamples(void *handle) {
        struct activator *activator = handle;

        while (activator->running) {
                struct timespec ts;
                int i;
                int rc = ETIMEDOUT;

                clock_gettime(CLOCK_REALTIME, &ts);
                ts.tv_sec += WAIT_TIME_SECONDS;

                pthread_mutex_lock(&activator->queueLock);

                /* block, till a queue is available */
                while ((arrayList_size(activator->queueServices) == 0) && (rc == ETIMEDOUT) && (activator->running)) {
                        rc = pthread_cond_timedwait(&activator->queueAvailable, &activator->queueLock, &ts);
                }
                pthread_mutex_unlock(&activator->queueLock);

                pthread_mutex_lock(&activator->dataStoreLock);

                rc=ETIMEDOUT;

                /* block, till a dataStore is available */
                while ((arrayList_size(activator->dataStoreServices) == 0) && (rc == ETIMEDOUT) && (activator->running)) {
                        rc = pthread_cond_timedwait(&activator->dataStoreAvailable, &activator->dataStoreLock, &ts);
                }
                pthread_mutex_unlock(&activator->dataStoreLock);

                pthread_mutex_lock(&activator->queueLock);
                pthread_mutex_lock(&activator->dataStoreLock);

                if (arrayList_size(activator->queueServices) > 0 && arrayList_size(activator->dataStoreServices)>0) {
                        for (i = 0; i < arrayList_size(activator->queueServices) && (activator->running) && (arrayList_size(activator->dataStoreServices)>0); i++) {
                                int j;
                                uint32_t numOfRecvSamples;
                                struct sample *recvSamples[MAX_BURST_LEN];
                                struct sample_queue_service* qService = arrayList_get(activator->queueServices, i);

                                uint32_t storedResults;
                                struct result processedSamples[MAX_BURST_LEN];
                                struct data_store_service* dsService=arrayList_get(activator->dataStoreServices, 0); /* Get just the first datastore available*/

                                struct timespec ts_start;
                                struct timespec ts_end;

                                int singleSampleCnt = 0;
                                int burstSampleCnt = 0;

				clock_gettime(CLOCK_REALTIME, &ts_start);

				for (ts_end = ts_start; (burstSampleCnt < BURST_SAMPLES_PER_SEC) && (ts_start.tv_sec == ts_end.tv_sec) && (activator->running);) {

					for (j=0; j < MAX_BURST_LEN; j++) {
						recvSamples[j] = calloc(1,sizeof(struct sample));
					}

					msg(2, "PROCESSOR: TakeAll (min: %d, max: %d)", MIN_BURST_LEN, MAX_BURST_LEN);

					if ( qService->takeAll(qService->sampleQueue, MIN_BURST_LEN, MAX_BURST_LEN , &recvSamples[0], &numOfRecvSamples) == 0) {
                                                msg(2, "PROCESSOR:  %u samples received", numOfRecvSamples);

                                                for (j = 0; j < numOfRecvSamples; j++) {
                                                        msg(2, "\tPROCESSOR: Processing Sample (%d/%d)  {Time:%llu | V1=%f | V2=%f}", j, numOfRecvSamples, recvSamples[j]->time, recvSamples[j]->value1, recvSamples[j]->value2);

                                                        processSample(recvSamples[j],&processedSamples[j]);

                                                }

                                                msg(2, "PROCESSOR: StoreAll after sample processing");
                                                dsService->storeAll(dsService->dataStore,processedSamples,numOfRecvSamples,&storedResults);

                                                burstSampleCnt += j;
                                        }
                                        else  {
						msg(0, "PROCESSOR: Could not take all samples.");
					}

					for (j=0; j < MAX_BURST_LEN; j++) {
						free(recvSamples[j]);
					}

					clock_gettime(CLOCK_REALTIME, &ts_end);
				}

				while(ts_start.tv_sec >= ts_end.tv_sec) {
					clock_gettime(CLOCK_REALTIME, &ts_end);
				}

				clock_gettime(CLOCK_REALTIME, &ts_start);

				// send single samples per second
                                for (ts_end = ts_start; (singleSampleCnt < SINGLE_SAMPLES_PER_SEC) && (ts_start.tv_sec == ts_end.tv_sec) && (activator->running); )
                                {
                                        struct sample *recvSample = calloc(1,sizeof(struct sample));
                                        struct result result;

                                        if ( qService->take(qService->sampleQueue, recvSample) == 0) {
                                                msg(3, "\tPROCESSOR: Received and Processing Sample {Time:%llu | V1=%f | V2=%f}", recvSample->time, recvSample->value1, recvSample->value2);

                                                processSample(recvSample,&result);

                                                msg(2, "PROCESSOR: Store after sample processing");
                                                bool resultStored=false;

                                                dsService->store(dsService->dataStore,result,&resultStored);

                                                singleSampleCnt++;
                                        }
                                        else {
						msg(2, "PROCESSOR: Could not take a single sample.");
					}

					free(recvSample);
					clock_gettime(CLOCK_REALTIME, &ts_end);
				}

				msg(1, "PROCESSOR: %d single samples // %d samples in bursts receives.", singleSampleCnt, burstSampleCnt);

				while(ts_start.tv_sec >= ts_end.tv_sec) {
					clock_gettime(CLOCK_REALTIME, &ts_end);
				}
                        }
                }

                pthread_mutex_unlock(&activator->dataStoreLock);
                pthread_mutex_unlock(&activator->queueLock);
        }

	pthread_exit(NULL);

	return NULL ;
}
