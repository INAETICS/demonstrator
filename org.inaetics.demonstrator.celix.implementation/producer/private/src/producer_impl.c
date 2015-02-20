/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
#include <stdarg.h>
#include <pthread.h>
#include <utils.h>

#include "producer_impl.h"

struct producer {
	hash_map_pt queueServices;
	pthread_rwlock_t queueLock;

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

static unsigned int producer_rand_range(unsigned int min, unsigned int max)
{
	struct timespec ts_rand;
	clock_gettime(CLOCK_REALTIME, &ts_rand);
	srand(ts_rand.tv_nsec);
	double scaled = (double) (((double) rand()) / ((double) RAND_MAX));

	return (max - min + 1) * scaled + min;
}

static void producer_fillSample(struct sample* s) {

	struct timeval tv;
	gettimeofday(&tv, NULL);
	s->time = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	srand(s->time);
	s->value1 = (double) rand();
	s->value2 = (double) rand();

}

celix_status_t producer_sendSamples(producer_pt producer, int samplesPerSec)
{
	celix_status_t status = CELIX_SUCCESS;
	struct sample_queue_service* queueService = NULL;
	struct sample smpl;
	struct timespec ts_start;
	struct timespec ts_end;
	int singleSampleCnt = 0;

	clock_gettime(CLOCK_REALTIME, &ts_start);

	// send single samples per second
	for (ts_end = ts_start; (singleSampleCnt < SINGLE_SAMPLES_PER_SEC) && (ts_start.tv_sec == ts_end.tv_sec);) {
		bool ret = false;

		memset(&smpl, 0, sizeof(struct sample));
		producer_fillSample(&smpl);

		pthread_rwlock_rdlock(&producer->queueLock);
		pthread_t self = pthread_self();
		queueService = (struct sample_queue_service*) hashMap_get(producer->queueServices, &self);

		if (queueService != NULL) {
			queueService->put(queueService->sampleQueue, smpl, &ret);

			if (ret == true) {
				msg(3, "PRODUCER: Sample {Time:%llu | V1=%f | V2=%f} stored.", smpl.time, smpl.value1,
						smpl.value2);
				singleSampleCnt++;
			}
			else {
				msg(2, "PRODUCER: Could not store sample.");
			}
		}
		else {
			status = CELIX_BUNDLE_EXCEPTION;
		}
		pthread_rwlock_unlock(&producer->queueLock);

		clock_gettime(CLOCK_REALTIME, &ts_end);
	}

	msg(1, "PRODUCER: %d single samples sent.", singleSampleCnt);

	while (ts_start.tv_sec >= ts_end.tv_sec) {
		clock_gettime(CLOCK_REALTIME, &ts_end);
	}

	return status;
}

celix_status_t producer_sendBursts(producer_pt producer, int samplesPerSec) {

	celix_status_t status = CELIX_SUCCESS;
	struct sample_queue_service* queueService = NULL;
	struct timespec ts_start;
	struct timespec ts_end;
	int burstSampleCnt = 0;

	clock_gettime(CLOCK_REALTIME, &ts_start);

	for (ts_end = ts_start; (burstSampleCnt < BURST_SAMPLES_PER_SEC) && (ts_start.tv_sec == ts_end.tv_sec);) {
		int burst_len = producer_rand_range(MIN_BURST_LEN, MAX_BURST_LEN);
		uint32_t burst_samples_stored = 0;

		struct sample burst[burst_len];
		memset(burst, 0, burst_len * sizeof(struct sample));
		int j = 0;

		msg(3, "PRODUCER: Preparing burst of %u samples", burst_len);

		for (; j < burst_len; j++) {
			producer_fillSample(&burst[j]);
			msg(3, "\tPRODUCER: Prepared sample {Time:%llu | V1=%f | V2=%f}", burst[j].time, burst[j].value1,
					burst[j].value2);
		}

		pthread_rwlock_rdlock(&producer->queueLock);
		pthread_t self = pthread_self();
		queueService = (struct sample_queue_service*) hashMap_get(producer->queueServices, &self);

		if (queueService != NULL) {
			queueService->putAll(queueService->sampleQueue, burst, burst_len, &burst_samples_stored);
		}
		else {
			status = CELIX_BUNDLE_EXCEPTION;
		}

		pthread_rwlock_unlock(&producer->queueLock);
		burstSampleCnt += burst_samples_stored;

		clock_gettime(CLOCK_REALTIME, &ts_end);
	}

	msg(1, "PRODUCER: %d burst samples sent.", burstSampleCnt);

	while (ts_start.tv_sec >= ts_end.tv_sec) {
		clock_gettime(CLOCK_REALTIME, &ts_end);
	}

	return status;
}

void *producer_generate(void *handle) {
	producer_pt producer = (producer_pt) handle;
	celix_status_t status = CELIX_SUCCESS;

	producer->running = true;

	while (producer->running && status == CELIX_SUCCESS) {

		if (BURST_SAMPLES_PER_SEC > 0) {
			status = producer_sendBursts(producer, BURST_SAMPLES_PER_SEC);
		}

		if (SINGLE_SAMPLES_PER_SEC > 0) {
			status = producer_sendSamples(producer, SINGLE_SAMPLES_PER_SEC);
		}
	}

	return NULL;
}

celix_status_t producer_create(producer_pt* producer)
{
	celix_status_t status = CELIX_SUCCESS;

	producer_pt lclProducer = calloc(1, sizeof(*lclProducer));

	if (lclProducer != NULL) {
		lclProducer->running = false;
		pthread_rwlock_init(&lclProducer->queueLock, NULL);
		lclProducer->queueServices = hashMap_create(utils_stringHash, NULL, utils_stringEquals, NULL);

		(*producer) = lclProducer;
	} else {
		status = CELIX_ENOMEM;
	}

	return status;
}

celix_status_t producer_stop(producer_pt producer)
{
	celix_status_t status = CELIX_SUCCESS;

	printf("PRODUCER: Stopping Producer.\n");
	producer->running = false;

	return status;
}

celix_status_t producer_destroy(producer_pt producer)
{
	celix_status_t status = CELIX_SUCCESS;

	pthread_rwlock_wrlock(&producer->queueLock);
	hashMap_destroy(producer->queueServices, false, false);
	pthread_rwlock_unlock(&producer->queueLock);

	pthread_rwlock_destroy(&producer->queueLock);

	return status;
}

celix_status_t producer_queueServiceAdded(void *handle, service_reference_pt reference, void *service)
{
	producer_pt producer = (producer_pt) handle;
	pthread_t* thread_pt = calloc(1, sizeof(*thread_pt));

	pthread_rwlock_wrlock(&producer->queueLock);
	pthread_create(thread_pt, NULL, producer_generate, producer);
	hashMap_put(producer->queueServices, thread_pt, service);
	pthread_rwlock_unlock(&producer->queueLock);

	return CELIX_SUCCESS;
}

celix_status_t producer_queueServiceRemoved(void *handle, service_reference_pt reference, void *service)
{
	celix_status_t status = CELIX_BUNDLE_EXCEPTION;
	producer_pt producer = (producer_pt) handle;
	pthread_t* thread_pt = NULL;

	pthread_rwlock_wrlock(&producer->queueLock);

	hash_map_iterator_pt iter = hashMapIterator_create(producer->queueServices);

	while (hashMapIterator_hasNext(iter) && thread_pt == NULL) {
		hash_map_entry_pt entry = hashMapIterator_nextEntry(iter);

		if (service == hashMapEntry_getValue(entry)) {
			thread_pt = hashMapEntry_getKey(entry);
			hashMap_remove(producer->queueServices, thread_pt);
		}
	}

	hashMapIterator_destroy(iter);

	pthread_rwlock_unlock(&producer->queueLock);

	if (thread_pt != NULL) {
		pthread_join(*thread_pt, NULL);
		free(thread_pt);
		status = CELIX_SUCCESS;
	}

	return status;
}
