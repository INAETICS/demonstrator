#include <stdarg.h>
#include <time.h>

#include "utils.h"
#include "celix_errno.h"
#include "processor_impl.h"

struct processor {
	hash_map_pt queueServices;
	hash_map_pt queueServiceLocks;
	pthread_rwlock_t queueLock;

	array_list_pt dataStoreServices;
	pthread_rwlock_t dataStoreLock;

	bool running;
};

static void msg(int lvl, char *fmsg, ...) {
	if (lvl <= VERBOSE) {
		char msg[512];
		va_list listPointer;
		va_start(listPointer, fmsg);
		vsprintf(msg, fmsg, listPointer);

		printf("[%d] : %s\n", lvl, msg);
	}
}

static void processor_sendResult(processor_pt processor, struct result result) {

	int i = 0;

	pthread_rwlock_rdlock(&processor->dataStoreLock);

	for (; i < arrayList_size(processor->dataStoreServices); i++) {
		bool resultStored = false;
		struct data_store_service* dsService = arrayList_get(processor->dataStoreServices, i);

		if ( (dsService->store(dsService->dataStore, result, &resultStored) != 0)  || resultStored == false) {
			msg(2, "PROCESSOR: Could not store single sample.");
		}
	}

	pthread_rwlock_unlock(&processor->dataStoreLock);
}

static void processor_processSample(struct sample* sample, struct result* result) {

	result->time = sample->time;
	result->value1 = sample->value1 + sample->value2;
	memcpy(&(result->sample), sample, sizeof(struct sample));
}

celix_status_t processor_receiveSamples(processor_pt processor, int samplesPerSec) {
	celix_status_t status = CELIX_SUCCESS;
	struct sample_queue_service* queueService = NULL;
	struct timespec ts_start;
	struct timespec ts_end;
	int singleSampleCnt = 0;

	clock_gettime(CLOCK_REALTIME, &ts_start);

	for (ts_end = ts_start; (singleSampleCnt < SINGLE_SAMPLES_PER_SEC) && (ts_start.tv_sec == ts_end.tv_sec);) {
		struct sample *recvSample = calloc(1, sizeof(struct sample));

		pthread_t self = pthread_self();
		pthread_rwlock_rdlock(&processor->queueLock);
		queueService = (struct sample_queue_service*) hashMap_get(processor->queueServices, &self);

		if (queueService != NULL && queueService->take(queueService->sampleQueue, recvSample) == 0) {
			struct result* result_pt = calloc(1, sizeof(*result_pt));

			msg(3, "\tPROCESSOR: Received and Processing Sample {Time:%llu | V1=%f | V2=%f}", recvSample->time, recvSample->value1, recvSample->value2);
			processor_processSample(recvSample, result_pt);
			processor_sendResult(processor, *result_pt);

			singleSampleCnt++;
		}
		else {
			msg(2, "PROCESSOR: Could not take a single sample.");
		}
		pthread_rwlock_unlock(&processor->queueLock);

		free(recvSample);
		clock_gettime(CLOCK_REALTIME, &ts_end);
	}

	msg(1, "PROCESSOR: %d single samples received.", singleSampleCnt);

	while (ts_start.tv_sec >= ts_end.tv_sec) {
		clock_gettime(CLOCK_REALTIME, &ts_end);
	}

	return status;
}

celix_status_t processor_receiveBursts(processor_pt processor, int samplesPerSec) {
	celix_status_t status = CELIX_SUCCESS;
	struct sample_queue_service* queueService = NULL;
	struct sample *recvSamples[MAX_BURST_LEN];
	struct timespec ts_start;
	struct timespec ts_end;
	uint32_t numOfRecvSamples;
	int burstSampleCnt = 0;

	clock_gettime(CLOCK_REALTIME, &ts_start);

	for (ts_end = ts_start; (burstSampleCnt < samplesPerSec) && (ts_start.tv_sec == ts_end.tv_sec);) {
		int j;

		for (j = 0; j < MAX_BURST_LEN; j++) {
			recvSamples[j] = calloc(1, sizeof(struct sample));
		}

		msg(3, "PROCESSOR: TakeAll (min: %d, max: %d)", MIN_BURST_LEN, MAX_BURST_LEN);

		pthread_rwlock_rdlock(&processor->queueLock);
		pthread_t self = pthread_self();

		queueService = hashMap_get(processor->queueServices, &self);

		if (queueService != NULL && queueService->takeAll(queueService->sampleQueue, MIN_BURST_LEN, MAX_BURST_LEN, &recvSamples[0], &numOfRecvSamples) == 0) {
			msg(3, "PROCESSOR:  %u samples received", numOfRecvSamples);

			for (j = 0; j < numOfRecvSamples; j++) {
				msg(3, "\tPROCESSOR: Processing Sample (%d/%d)  {Time:%llu | V1=%f | V2=%f}", j, numOfRecvSamples, recvSamples[j]->time, recvSamples[j]->value1, recvSamples[j]->value2);
				struct result* result_pt = calloc(1, sizeof(*result_pt));

				processor_processSample(recvSamples[j], result_pt);
				processor_sendResult(processor, *result_pt);
			}

			burstSampleCnt += j;
		}
		else {
			msg(2, "PROCESSOR: Could not take all samples.");
		}

		pthread_rwlock_unlock(&processor->queueLock);

		for (j = 0; j < MAX_BURST_LEN; j++) {
			free(recvSamples[j]);
		}

		clock_gettime(CLOCK_REALTIME, &ts_end);
	}

	msg(1, "PROCESSOR:  %d samples in bursts received.", burstSampleCnt);

	while (ts_start.tv_sec >= ts_end.tv_sec) {
		clock_gettime(CLOCK_REALTIME, &ts_end);
	}

	return status;
}

void* processor_receive(void *handle) {
	processor_pt processor = (processor_pt) handle;

	processor->running = true;

	while (processor->running) {

		if (BURST_SAMPLES_PER_SEC > 0) {
			processor_receiveBursts(processor, BURST_SAMPLES_PER_SEC);
		}

		if (SINGLE_SAMPLES_PER_SEC > 0) {
			processor_receiveSamples(processor, SINGLE_SAMPLES_PER_SEC);
		}
	}

	return NULL;
}

celix_status_t processor_create(processor_pt* processor)
{
	celix_status_t status = CELIX_SUCCESS;

	processor_pt lclProcessor = calloc(1, sizeof(*lclProcessor));

	if (lclProcessor != NULL) {

		lclProcessor->queueServices = hashMap_create(utils_stringHash, NULL, utils_stringEquals, NULL);
		arrayList_create(&lclProcessor->dataStoreServices);

		pthread_rwlock_init(&lclProcessor->queueLock, NULL);
		pthread_rwlock_init(&lclProcessor->dataStoreLock, NULL);

		lclProcessor->running = false;

		(*processor) = lclProcessor;
	} else {
		status = CELIX_ENOMEM;
	}

	return status;
}

celix_status_t processor_stop(processor_pt processor)
{
	celix_status_t status = CELIX_SUCCESS;

	processor->running = false;

	return status;
}

celix_status_t processor_destroy(processor_pt processor)
{
	celix_status_t status = CELIX_SUCCESS;

	pthread_rwlock_wrlock(&processor->queueLock);
	hashMap_destroy(processor->queueServices, false, false);
	pthread_rwlock_unlock(&processor->queueLock);

	pthread_rwlock_wrlock(&processor->dataStoreLock);
	arrayList_destroy(processor->dataStoreServices);
	pthread_rwlock_unlock(&processor->dataStoreLock);

	pthread_rwlock_destroy(&processor->queueLock);
	pthread_rwlock_destroy(&processor->dataStoreLock);

	return status;
}

celix_status_t processor_queueServiceAdded(void *handle, service_reference_pt reference, void *service)
{
	processor_pt processor = (processor_pt) handle;
	pthread_t* thread_pt = calloc(1, sizeof(*thread_pt));

	pthread_rwlock_wrlock(&processor->queueLock);
	pthread_create(thread_pt, NULL, processor_receive, processor);
	hashMap_put(processor->queueServices, thread_pt, service);
	pthread_rwlock_unlock(&processor->queueLock);

	return CELIX_SUCCESS;
}

celix_status_t processor_queueServiceRemoved(void *handle, service_reference_pt reference, void *service)
{
	celix_status_t status = CELIX_BUNDLE_EXCEPTION;
	processor_pt processor = (processor_pt) handle;
	pthread_t* thread_pt = NULL;

	pthread_rwlock_wrlock(&processor->queueLock);

	hash_map_iterator_pt iter = hashMapIterator_create(processor->queueServices);

	while (hashMapIterator_hasNext(iter) && thread_pt == NULL) {
		hash_map_entry_pt entry = hashMapIterator_nextEntry(iter);

		if (service == hashMapEntry_getValue(entry)) {
			thread_pt = hashMapEntry_getKey(entry);
			hashMap_remove(processor->queueServices, thread_pt);
		}
	}

	hashMapIterator_destroy(iter);

	pthread_rwlock_unlock(&processor->queueLock);

	if (thread_pt != NULL) {
		pthread_join(*thread_pt, NULL);
		free(thread_pt);
		status = CELIX_SUCCESS;
	}

	return status;
}

celix_status_t processor_dataStoreServiceAdded(void *handle, service_reference_pt reference, void *service)
{
	processor_pt processor = (processor_pt) handle;

	pthread_rwlock_wrlock(&processor->dataStoreLock);
	arrayList_add(processor->dataStoreServices, service);
	pthread_rwlock_unlock(&processor->dataStoreLock);

	printf("PROCESSOR: DataStore Service Added.\n");

	return CELIX_SUCCESS;
}

celix_status_t processor_dataStoreServiceRemoved(void *handle, service_reference_pt reference, void *service)
{
	processor_pt processor = (processor_pt) handle;

	pthread_rwlock_wrlock(&processor->dataStoreLock);
	arrayList_removeElement(processor->dataStoreServices, service);
	pthread_rwlock_unlock(&processor->dataStoreLock);

	printf("PROCESSOR: DataStore Service Removed.\n");

	return CELIX_SUCCESS;
}
