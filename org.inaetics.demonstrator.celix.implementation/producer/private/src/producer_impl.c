#define _GNU_SOURCE

#include "producer_impl.h"

#include <stdarg.h>
#include <stdint.h>
#include <unistd.h>

#include "celix_errno.h"
#include "service_reference.h"
#include "utils.h"
#include "hash_map.h"
#include "inaetics_demonstrator_api/sample.h"

#define MAX_SAMPLES_PER_SEC 	500

#define MIN_BURST_LEN 			10
#define MAX_BURST_LEN 			50

#define VERBOSE					2
#define WAIT_TIME_USECONDS      500000

#define THROUGHPUT_NAME_POSTFIX 		" Statistics"
#define THROUGHPUT_TYPE 				"(throughput)"
#define THROUGHPUT_MEASUREMENT_UNIT		"sample/sec"

struct producer {
	char *name;
	char *utilizationStatsName;

	hash_map_pt queueServices;
	pthread_rwlock_t queueLock;
};

struct producer_thread_data {
	pthread_t thread;
	bool running;

	struct sample_queue_service* service;

	pthread_rwlock_t sampleRateLock;
	unsigned int sampleRate;

	pthread_rwlock_t throughputLock;
	unsigned long single_throughput;
	unsigned long burst_throughput;
};

typedef struct producer_thread_data* producer_thread_data_pt;

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

static void timespec_diff(struct timespec* diff, struct timespec* start, struct timespec* end)
{

	if ((end->tv_nsec-start->tv_nsec)<0) {
		diff->tv_sec = end->tv_sec-start->tv_sec-1;
		diff->tv_nsec = 1000000000+end->tv_nsec-start->tv_nsec;
	} else {
		diff->tv_sec = end->tv_sec-start->tv_sec;
		diff->tv_nsec = end->tv_nsec-start->tv_nsec;
	}

}

celix_status_t producer_sendSamples(producer_thread_data_pt th_data, int samplesPerSec)
{
	celix_status_t status = CELIX_SUCCESS;
	struct sample_queue_service* queueService = th_data->service;
	struct sample smpl;
	struct timespec ts_start;
	struct timespec ts_end;
	struct timespec ts_diff;
	int singleSampleCnt = 0;

	clock_gettime(CLOCK_REALTIME, &ts_start);
	timespec_diff(&ts_diff,&ts_start,&ts_start);

	// send single samples per second
	for (ts_end = ts_start; (singleSampleCnt < samplesPerSec) && (ts_diff.tv_sec<=0);) {
		bool ret = false;

		memset(&smpl, 0, sizeof(struct sample));
		producer_fillSample(&smpl);

		if (queueService != NULL) {
			int rc = queueService->put(queueService->sampleQueue, &smpl, &ret);
			if (rc != 0) {
				msg(1, "PRODUCER: Error invoking put");
			}

			if (ret == true) {
				msg(3, "PRODUCER: Sample {Time:%llu | V1=%f | V2=%f} stored.", smpl.time, smpl.value1,
						smpl.value2);
				singleSampleCnt++;
			}
		}
		else {
			status = CELIX_BUNDLE_EXCEPTION;
		}

		clock_gettime(CLOCK_REALTIME, &ts_end);
		timespec_diff(&ts_diff,&ts_start,&ts_end);
	}

	/* Update the statistic */
	pthread_rwlock_rdlock(&th_data->throughputLock);
	th_data->single_throughput = singleSampleCnt;
	pthread_rwlock_unlock(&th_data->throughputLock);

	msg(1, "PRODUCER: %d single samples sent.", singleSampleCnt);

	usleep(WAIT_TIME_USECONDS);

	return status;
}

celix_status_t producer_sendBursts(producer_thread_data_pt th_data, int samplesPerSec) {

	celix_status_t status = CELIX_SUCCESS;
	struct sample_queue_service* queueService = th_data->service;
	struct timespec ts_start;
	struct timespec ts_end;
	struct timespec ts_diff;
	struct timespec ts_before;
	struct timespec ts_after;
	int burstSampleCnt = 0;

	clock_gettime(CLOCK_REALTIME, &ts_start);
	timespec_diff(&ts_diff,&ts_start,&ts_start);

	int counter=0;

	for (ts_end = ts_start; (burstSampleCnt < samplesPerSec) && (ts_diff.tv_sec<=0);) {
		int burst_len = producer_rand_range(MIN_BURST_LEN, MAX_BURST_LEN);
		uint32_t burst_samples_stored = 0;

		struct sample burst[burst_len];
		memset(burst, 0, burst_len * sizeof(struct sample));
		int j = 0;

		msg(1, "PRODUCER: Preparing burst of %u samples, round %d", burst_len,counter++);

		for (; j < burst_len; j++) {
			producer_fillSample(&burst[j]);
			msg(3, "\tPRODUCER: Prepared sample {Time:%llu | V1=%f | V2=%f}", burst[j].time, burst[j].value1,
					burst[j].value2);
		}

		if (queueService != NULL) {
			clock_gettime(CLOCK_REALTIME, &ts_before);

			struct sample_sequence samples;
            samples.cap = burst_len;
            samples.len = burst_len;
            samples.buf = burst;
			int rc = queueService->putAll(queueService->sampleQueue, samples, &burst_samples_stored);
			if (rc != 0) {
				msg(1, "PRODUCER: Error invoking putAll");
			}
			clock_gettime(CLOCK_REALTIME, &ts_after);
		}
		else {
			status = CELIX_BUNDLE_EXCEPTION;
		}

		burstSampleCnt += burst_samples_stored;

		clock_gettime(CLOCK_REALTIME, &ts_end);
		timespec_diff(&ts_diff,&ts_start,&ts_end);
	}

	/* Update the statistic */
	pthread_rwlock_wrlock(&th_data->throughputLock);
	th_data->burst_throughput = burstSampleCnt;
	pthread_rwlock_unlock(&th_data->throughputLock);

	timespec_diff(&ts_diff,&ts_before,&ts_after);

	msg(1, "PRODUCER: %d burst samples sent in %d %d\n", burstSampleCnt, ts_diff.tv_sec, ts_diff.tv_nsec);

	usleep(WAIT_TIME_USECONDS);

	return status;
}

void *producer_generate(void *handle) {
	producer_thread_data_pt th_data = (producer_thread_data_pt) handle;
	celix_status_t status = CELIX_SUCCESS;
	int sampleRate;

	th_data->running = true;

	while (th_data->running && status == CELIX_SUCCESS) {

		pthread_rwlock_rdlock(&th_data->throughputLock);
		sampleRate = th_data->sampleRate;
		pthread_rwlock_unlock(&th_data->throughputLock);

		if (th_data->sampleRate  > 0) {
			status = producer_sendBursts(th_data, sampleRate);
			status = producer_sendSamples(th_data, sampleRate);
		}

		pthread_yield();
	}

	return NULL;
}

celix_status_t producer_create(char* name, producer_pt* producer)
{
	celix_status_t status = CELIX_SUCCESS;

	producer_pt lclProducer = calloc(1, sizeof(*lclProducer));

	if (lclProducer != NULL) {

		lclProducer->name = strdup(name);
		lclProducer->utilizationStatsName = calloc(1, strlen(name) + strlen(THROUGHPUT_NAME_POSTFIX) + 1);

		if (lclProducer->name != NULL && lclProducer->utilizationStatsName != NULL) {
			pthread_rwlockattr_t queueLockAttr;

			sprintf(lclProducer->utilizationStatsName, "%s%s", lclProducer->name, (char*) THROUGHPUT_NAME_POSTFIX);

			pthread_rwlockattr_init(&queueLockAttr);
			pthread_rwlockattr_setkind_np(&queueLockAttr, PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP);
			pthread_rwlock_init(&lclProducer->queueLock, &queueLockAttr);

			lclProducer->queueServices = hashMap_create(NULL, NULL, NULL, NULL);

			(*producer) = lclProducer;
		}
		else {
			status = CELIX_ENOMEM;
		}
	} else {
		status = CELIX_ENOMEM;
	}

	return status;
}

celix_status_t producer_stop(producer_pt producer)
{
	celix_status_t status = CELIX_SUCCESS;

	printf("PRODUCER: Stopping Producer.\n");

	pthread_rwlock_rdlock(&producer->queueLock);
	hash_map_iterator_pt iter = hashMapIterator_create(producer->queueServices);

	while (hashMapIterator_hasNext(iter)) {
		producer_thread_data_pt th_data = hashMapIterator_nextValue(iter);
		th_data->running = false;
	}

	hashMapIterator_destroy(iter);
	pthread_rwlock_unlock(&producer->queueLock);

	return status;
}

celix_status_t producer_destroy(producer_pt producer)
{
	celix_status_t status = CELIX_SUCCESS;

	pthread_rwlock_wrlock(&producer->queueLock);
	hashMap_destroy(producer->queueServices, false, true);
	pthread_rwlock_unlock(&producer->queueLock);

	pthread_rwlock_destroy(&producer->queueLock);

	if (producer->utilizationStatsName != NULL) {
		free(producer->utilizationStatsName);
	}

	if (producer->name != NULL) {
		free(producer->name);
	}

	free(producer);

	return status;
}

celix_status_t producer_queueServiceAdded(void *handle, service_reference_pt reference, void *service)
{
	producer_pt producer = (producer_pt) handle;

	printf("PRODUCER: QueueService Added.\n");

	pthread_rwlock_wrlock(&producer->queueLock);

	producer_thread_data_pt th_data = calloc(1, sizeof(struct producer_thread_data));
	th_data->service = service;
	th_data->sampleRate = MAX_SAMPLES_PER_SEC;
	pthread_rwlock_init(&th_data->sampleRateLock, NULL);

	pthread_create(&th_data->thread, NULL, producer_generate, th_data);
	hashMap_put(producer->queueServices, service, th_data);

	pthread_rwlock_unlock(&producer->queueLock);

	return CELIX_SUCCESS;
}

celix_status_t producer_queueServiceRemoved(void *handle, service_reference_pt reference, void *service)
{
	celix_status_t status = CELIX_SUCCESS;
	producer_pt producer = (producer_pt) handle;

	printf("PRODUCER: QueueService Removed.\n");

	pthread_rwlock_wrlock(&producer->queueLock);

	producer_thread_data_pt th_data = (producer_thread_data_pt) hashMap_get(producer->queueServices, service);
	th_data->running = false;
	pthread_join(th_data->thread, NULL);

	pthread_rwlock_destroy(&th_data->sampleRateLock);

	hashMap_remove(producer->queueServices, service);
	free(th_data);

	pthread_rwlock_unlock(&producer->queueLock);

	return status;
}

int producer_getUtilizationStatsName(producer_pt producer, char **name) {
	celix_status_t status = CELIX_SUCCESS;

	if (producer->utilizationStatsName != NULL) {
		(*name) = strdup(producer->utilizationStatsName);
	}
	else {
		msg(0, "PRODUCER_STAT: getName denied because service is removed");
		status = CELIX_ILLEGAL_STATE;
	}

	return (int) status;
}

int producer_getUtilizationStatsType(producer_pt producer, char **type) {
	(*type) = strdup((char*) THROUGHPUT_TYPE);
	return (int) CELIX_SUCCESS;
}

int producer_getUtilizationStatsValue(producer_pt producer, double* statVal) {

	double total_average = 0;

	pthread_rwlock_rdlock(&producer->queueLock);

	hash_map_iterator_pt iter = hashMapIterator_create(producer->queueServices);

	while (hashMapIterator_hasNext(iter)) {

		hash_map_entry_pt entry = hashMapIterator_nextEntry(iter);

		if (entry != NULL) {
			producer_thread_data_pt value = (producer_thread_data_pt) hashMapEntry_getValue(entry);
			if (value != NULL) {
				total_average += (double) (((double) (value->single_throughput + value->burst_throughput)) / ((double) 2.0f));
			}
		}
	}

	hashMapIterator_destroy(iter);

	if (hashMap_size(producer->queueServices) > 0) {
		total_average /= hashMap_size(producer->queueServices);
	}

	pthread_rwlock_unlock(&producer->queueLock);

	(*statVal) = total_average;
	return (int) CELIX_SUCCESS;
}

int producer_getUtilizationStatsMeasurementUnit(producer_pt producer, char **mUnit) {
	(*mUnit) = strdup((char*) THROUGHPUT_MEASUREMENT_UNIT);
	return (int) CELIX_SUCCESS;
}




int producer_getMaxSampleRate(void* handle) {
	return MAX_SAMPLES_PER_SEC;
}


int producer_getSampleRate(void* handle) {
	producer_pt producer = (producer_pt) handle;
	int sampleRate = 0;

	pthread_rwlock_rdlock(&producer->queueLock);

	hash_map_iterator_pt iter = hashMapIterator_create(producer->queueServices);

	while (hashMapIterator_hasNext(iter)) {
		producer_thread_data_pt th_data = (producer_thread_data_pt) hashMapIterator_nextValue(iter);

		pthread_rwlock_rdlock(&th_data->sampleRateLock);
		sampleRate += th_data->single_throughput; // + value->burst_throughput
		pthread_rwlock_unlock(&th_data->sampleRateLock);
	}

	hashMapIterator_destroy(iter);

	if (hashMap_size(producer->queueServices) > 0) {
		sampleRate /= hashMap_size(producer->queueServices);
	}

	pthread_rwlock_unlock(&producer->queueLock);

	return sampleRate;
}



void producer_setSampleRate(void* handle, int rate) {
	producer_pt producer = (producer_pt) handle;

	pthread_rwlock_rdlock(&producer->queueLock);

	hash_map_iterator_pt iter = hashMapIterator_create(producer->queueServices);

	while (hashMapIterator_hasNext(iter)) {

		producer_thread_data_pt th_data = (producer_thread_data_pt) hashMapIterator_nextValue(iter);

		pthread_rwlock_wrlock(&th_data->sampleRateLock);
		th_data->sampleRate = rate;
		pthread_rwlock_unlock(&th_data->sampleRateLock);
		msg(0, "PRODUCER: SampleRate set to %d", rate);
	}

	hashMapIterator_destroy(iter);

	pthread_rwlock_unlock(&producer->queueLock);
}

