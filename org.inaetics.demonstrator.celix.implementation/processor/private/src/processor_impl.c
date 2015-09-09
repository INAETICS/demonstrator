#define _GNU_SOURCE

#include "processor_impl.h"

#include <stdarg.h>
#include <stdint.h>
#include <unistd.h>

#include "celix_errno.h"
#include "service_reference.h"
#include "utils.h"
#include "hash_map.h"
#include "inaetics_demonstrator_api/result.h"
#include "inaetics_demonstrator_api/sample.h"

#define SINGLE_SAMPLES_PER_SEC  1000
#define BURST_SAMPLES_PER_SEC 	0

#define MIN_BURST_LEN 			2
#define MAX_BURST_LEN 			200

#define VERBOSE					2

#define THROUGHPUT_NAME_POSTFIX 		" Statistics"
#define THROUGHPUT_TYPE 				"(throughput)"
#define THROUGHPUT_MEASUREMENT_UNIT		"result/sec"

struct processor {
	char* name;
	char *utilizationStatsName;

	hash_map_pt queueServices;
	pthread_rwlock_t queueLock;

	array_list_pt dataStoreServices;
	pthread_rwlock_t dataStoreLock;

	bool running;
};

struct processor_thread_data {
	pthread_t thread;
	bool running;

	processor_pt processor;
	struct sample_queue_service* service;

	pthread_rwlock_t throughputLock;
	unsigned long single_throughput;
	unsigned long burst_throughput;
};

typedef struct processor_thread_data* processor_thread_data_pt;

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

		if ((dsService->store(dsService->dataStore, result, &resultStored) != 0) || resultStored == false) {
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


celix_status_t processor_receiveSamples(processor_thread_data_pt th_data, int samplesPerSec) {
	celix_status_t status = CELIX_SUCCESS;
	struct sample_queue_service* queueService = th_data->service;
	struct timespec ts_start;
	struct timespec ts_end;
	struct timespec ts_diff;
	int singleSampleCnt = 0;

	clock_gettime(CLOCK_REALTIME, &ts_start);
	timespec_diff(&ts_diff,&ts_start,&ts_start);

	for (ts_end = ts_start; (singleSampleCnt < samplesPerSec) && (ts_diff.tv_sec<=0);) {
		struct sample *recvSample = calloc(1, sizeof(struct sample));

		if (recvSample) {
			if (queueService != NULL) {
				if (queueService->take(queueService->sampleQueue, recvSample) == 0) {
					struct result* result_pt = calloc(1, sizeof(*result_pt));

					msg(3, "\tPROCESSOR: Received and Processing Sample {Time:%llu | V1=%f | V2=%f}", recvSample->time, recvSample->value1, recvSample->value2);
					processor_processSample(recvSample, result_pt);
					processor_sendResult(th_data->processor, *result_pt);

					singleSampleCnt++;
				}
				else {
					msg(2, "PROCESSOR: Could not take a single sample.");
				}
			}
			else {
				status = CELIX_BUNDLE_EXCEPTION;
			}

			free(recvSample);
		}
		clock_gettime(CLOCK_REALTIME, &ts_end);
		timespec_diff(&ts_diff,&ts_start,&ts_end);
	}

	/* Update the statistic */
	pthread_rwlock_wrlock(&th_data->throughputLock);
	th_data->single_throughput = singleSampleCnt;
	pthread_rwlock_unlock(&th_data->throughputLock);

	msg(1, "PROCESSOR: %d single samples received.", singleSampleCnt);

	return status;
}


celix_status_t processor_receiveBursts(processor_thread_data_pt th_data, int samplesPerSec) {
	celix_status_t status = CELIX_SUCCESS;
	struct sample_queue_service* queueService = th_data->service;
	struct sample *recvSamples[MAX_BURST_LEN];
	struct timespec ts_start;
	struct timespec ts_end;
	struct timespec ts_diff;
	uint32_t numOfRecvSamples;
	int burstSampleCnt = 0;

	clock_gettime(CLOCK_REALTIME, &ts_start);
	timespec_diff(&ts_diff,&ts_start,&ts_start);

	for (ts_end = ts_start; (burstSampleCnt < samplesPerSec) && (ts_diff.tv_sec<=0);) {
		int j;

		for (j = 0; j < MAX_BURST_LEN; j++) {
			recvSamples[j] = calloc(1, sizeof(struct sample));
		}

		msg(3, "PROCESSOR: TakeAll (min: %d, max: %d)", MIN_BURST_LEN, MAX_BURST_LEN);

		if (queueService != NULL) {
			if (queueService->takeAll(queueService->sampleQueue, MIN_BURST_LEN, MAX_BURST_LEN, &recvSamples[0], &numOfRecvSamples) == 0) {
				msg(3, "PROCESSOR:  %u samples received", numOfRecvSamples);

				for (j = 0; j < numOfRecvSamples; j++) {
					msg(3, "\tPROCESSOR: Processing Sample (%d/%d)  {Time:%llu | V1=%f | V2=%f}", j, numOfRecvSamples, recvSamples[j]->time, recvSamples[j]->value1, recvSamples[j]->value2);
					struct result* result_pt = calloc(1, sizeof(*result_pt));

					if (result_pt) {
						processor_processSample(recvSamples[j], result_pt);
						processor_sendResult(th_data->processor, *result_pt);
						free(result_pt);
					}

				}

				burstSampleCnt += j;
			}
			else {
				msg(2, "PROCESSOR: Could not take all samples.");
			}
		}
		else {
			status = CELIX_BUNDLE_EXCEPTION;
		}

		for (j = 0; j < MAX_BURST_LEN; j++) {
			free(recvSamples[j]);
		}

		clock_gettime(CLOCK_REALTIME, &ts_end);
		timespec_diff(&ts_diff,&ts_start,&ts_end);
	}

	pthread_rwlock_wrlock(&th_data->throughputLock);
	th_data->burst_throughput = burstSampleCnt;
	pthread_rwlock_unlock(&th_data->throughputLock);

	msg(1, "PROCESSOR:  %d samples in bursts received.", burstSampleCnt);

	return status;
}


void* processor_receive(void *handle) {
	processor_thread_data_pt th_data = (processor_thread_data_pt) handle;
	celix_status_t status = CELIX_SUCCESS;
	th_data->running = true;

	while (th_data->running && status == CELIX_SUCCESS) {
		if (BURST_SAMPLES_PER_SEC > 0) {
			status = processor_receiveBursts(th_data, BURST_SAMPLES_PER_SEC);
		}
		if (SINGLE_SAMPLES_PER_SEC > 0) {
			status = processor_receiveSamples(th_data, SINGLE_SAMPLES_PER_SEC);
		}

		pthread_yield();
	}

	return NULL;
}

celix_status_t processor_create(char* name, processor_pt* processor)
{
	celix_status_t status = CELIX_SUCCESS;

	processor_pt lclProcessor = calloc(1, sizeof(*lclProcessor));

	if (lclProcessor != NULL) {

		lclProcessor->name = strdup(name);
		lclProcessor->utilizationStatsName = calloc(1, strlen(name) + strlen(THROUGHPUT_NAME_POSTFIX) + 1);

		if (lclProcessor->name != NULL && lclProcessor->utilizationStatsName != NULL) {

			sprintf(lclProcessor->utilizationStatsName, "%s%s", lclProcessor->name, (char*) THROUGHPUT_NAME_POSTFIX);

			lclProcessor->queueServices = hashMap_create(utils_stringHash, NULL, utils_stringEquals, NULL);
			arrayList_create(&lclProcessor->dataStoreServices);

			pthread_rwlock_init(&lclProcessor->queueLock, NULL);
			pthread_rwlock_init(&lclProcessor->dataStoreLock, NULL);

			lclProcessor->running = false;

			(*processor) = lclProcessor;
		}
		else {
			status = CELIX_ENOMEM;
		}
	} else {
		status = CELIX_ENOMEM;
	}

	return status;
}

celix_status_t processor_stop(processor_pt processor)
{
	celix_status_t status = CELIX_SUCCESS;

	printf("PROCESSOR: Stopping Processor.\n");
	processor->running = false;

	return status;
}

celix_status_t processor_destroy(processor_pt processor)
{
	celix_status_t status = CELIX_SUCCESS;

	pthread_rwlock_wrlock(&processor->queueLock);
	hashMap_destroy(processor->queueServices, false, true);
	pthread_rwlock_unlock(&processor->queueLock);

	pthread_rwlock_wrlock(&processor->dataStoreLock);
	arrayList_destroy(processor->dataStoreServices);
	pthread_rwlock_unlock(&processor->dataStoreLock);

	pthread_rwlock_destroy(&processor->queueLock);
	pthread_rwlock_destroy(&processor->dataStoreLock);

	if (processor->utilizationStatsName != NULL) {
		free(processor->utilizationStatsName);
	}

	if (processor->name != NULL) {
		free(processor->name);
	}

	free(processor);

	return status;
}

celix_status_t processor_queueServiceAdded(void *handle, service_reference_pt reference, void *service)
{
	processor_pt processor = (processor_pt) handle;

	pthread_rwlock_wrlock(&processor->queueLock);

	processor_thread_data_pt th_data = calloc(1, sizeof(struct processor_thread_data));
	th_data->service = service;
	th_data->processor = processor;
	pthread_create(&th_data->thread, NULL, processor_receive, th_data);
	hashMap_put(processor->queueServices, service, th_data);

	pthread_rwlock_unlock(&processor->queueLock);

	return CELIX_SUCCESS;
}

celix_status_t processor_queueServiceRemoved(void *handle, service_reference_pt reference, void *service)
{
	celix_status_t status = CELIX_SUCCESS;
	processor_pt processor = (processor_pt) handle;

	pthread_rwlock_wrlock(&processor->queueLock);
	processor_thread_data_pt th_data = (processor_thread_data_pt) hashMap_get(processor->queueServices, service);
	th_data->running = false;
	pthread_join(th_data->thread, NULL);

	hashMap_remove(processor->queueServices, service);
	th_data->service = NULL;
	th_data->processor = NULL;
	free(th_data);

	pthread_rwlock_unlock(&processor->queueLock);

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

int processor_getUtilizationStatsName(processor_pt processor, char **name) {
	celix_status_t status = CELIX_SUCCESS;

	if (processor->utilizationStatsName != NULL) {
		(*name) = strdup(processor->utilizationStatsName);
	}
	else {
		msg(0, "PROCESSOR_STAT: getName denied because service is removed");
		status = CELIX_ILLEGAL_STATE;
	}

	return (int) status;
}

int processor_getUtilizationStatsType(processor_pt processor, char **type) {
	(*type) = strdup((char*) THROUGHPUT_TYPE);
	return (int) CELIX_SUCCESS;
}

int processor_getUtilizationStatsValue(processor_pt processor, double* statVal) {
	double total_average = 0;

	pthread_rwlock_rdlock(&processor->queueLock);

	hash_map_iterator_pt iter = hashMapIterator_create(processor->queueServices);

	while (hashMapIterator_hasNext(iter)) {

		hash_map_entry_pt entry = hashMapIterator_nextEntry(iter);

		if (entry != NULL) {
			processor_thread_data_pt value = (processor_thread_data_pt) hashMapEntry_getValue(entry);
			if (value != NULL) {
				total_average += (double) (((double) (value->single_throughput + value->burst_throughput)) / ((double) 2.0f));
			}
		}
	}

	hashMapIterator_destroy(iter);

	pthread_rwlock_unlock(&processor->queueLock);

	(*statVal) = total_average;
	return (int) CELIX_SUCCESS;
}

int processor_getUtilizationStatsMeasurementUnit(processor_pt processor, char **mUnit) {
	(*mUnit) = strdup((char*) THROUGHPUT_MEASUREMENT_UNIT);
	return (int) CELIX_SUCCESS;
}
