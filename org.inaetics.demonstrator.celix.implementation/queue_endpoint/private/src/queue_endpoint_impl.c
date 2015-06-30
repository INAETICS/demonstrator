#include "queue_endpoint_impl.h"

#include <jansson.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "celix_threads.h"
#include "celix_errno.h"
#include "remote_endpoint.h"
#include "remote_endpoint_impl.h"
#include "inaetics_demonstrator_api/sample.h"
#include "inaetics_demonstrator_api/sample_queue.h"

celix_status_t queueEndpoint_create(remote_endpoint_pt *endpoint) {
	celix_status_t status = CELIX_SUCCESS;

	*endpoint = calloc(1, sizeof(**endpoint));
	if (!*endpoint) {
		status = CELIX_ENOMEM;
	} else {
		(*endpoint)->service = NULL;
		celixThreadMutex_create(&(*endpoint)->serviceLock, NULL);
	}

	return status;
}

celix_status_t queueEndpoint_setService(remote_endpoint_pt endpoint, void *service) {
	celix_status_t status = CELIX_SUCCESS;

	celixThreadMutex_lock(&endpoint->serviceLock);
	endpoint->service = service;
	celixThreadMutex_unlock(&endpoint->serviceLock);
	return status;
}

/**
 * Request: http://host:port/services/{service}
 */
celix_status_t queueEndpoint_handleRequest(remote_endpoint_pt endpoint, char *data, char **reply) {
	celix_status_t status = CELIX_SUCCESS;
	json_error_t jsonError;
	json_t *root;

	const char *sig;

	root = json_loads(data, 0, &jsonError);
	json_unpack(root, "{s:s}", "m", &sig);

	if (strcmp(sig, "put(Lorg/inaetics/demonstrator/api/data/Sample;)Z") == 0) {
		queueEndpoint_put(endpoint, data, reply);
	} else if (strcmp(sig, "putAll(Ljava/util/Collection;)I") == 0) {
		queueEndpoint_putAll(endpoint, data, reply);
	} else if (strcmp(sig, "take()Lorg/inaetics/demonstrator/api/data/Sample;") == 0) {
		queueEndpoint_take(endpoint, data, reply);
	} else if (strcmp(sig, "takeMinMax(II)Ljava/util/Collection;") == 0) {
		queueEndpoint_takeAll(endpoint, data, reply);
	} else {
		printf("queueEndpoint received unknown incoming sig %s w/ data %s\n", sig, data);
		status = CELIX_ILLEGAL_ARGUMENT;
	}

	json_decref(root);

	return status;
}

static celix_status_t queueEndpoint_decodeToSample(uint64_t time, double value1, double value2, struct sample* workSample) {

	workSample->time = time;
	workSample->value1 = value1;
	workSample->value2 = value2;

	return CELIX_SUCCESS;
}

celix_status_t queueEndpoint_put(remote_endpoint_pt endpoint, char *data, char **reply) {
	celix_status_t status = CELIX_SUCCESS;
	json_error_t jsonError;
	json_t *root;

	root = json_loads(data, 0, &jsonError);

	if (!root) {
		status = CELIX_ILLEGAL_ARGUMENT;
	} else {
		uint64_t time;
		double value1;
		double value2;

		json_unpack(root, "{s:[{s:I,s:f,s:f}]}", "a", "sampleTime", &time, "value1", &value1, "value2", &value2);

		celixThreadMutex_lock(&endpoint->serviceLock);

		if (endpoint->service != NULL) {
			struct sample_queue_service* service = endpoint->service;

			json_t *resultRoot;
			struct sample workSample;
			bool result = false;
			int putResult;

			queueEndpoint_decodeToSample(time, value1, value2, &workSample);
			putResult = service->put(service->sampleQueue, workSample, &result);

			if (putResult == 0) {
				resultRoot = json_pack("{s:b}", "r", result);
			}
			else {
				resultRoot = json_pack("{s:b}", "r", false);
			}

			char *c = json_dumps(resultRoot, JSON_ENCODE_ANY);

			*reply = c;

			json_decref(resultRoot);
		} else {
			printf("QUEUE_ENDPOINT: No service available");
			status = CELIX_BUNDLE_EXCEPTION;
		}

		celixThreadMutex_unlock(&endpoint->serviceLock);

	}

	json_decref(root);

	return status;
}

celix_status_t queueEndpoint_putAll(remote_endpoint_pt endpoint, char *data, char **reply) {
	celix_status_t status = CELIX_SUCCESS;
	json_error_t jsonError;
	json_t *root;

	root = json_loads(data, 0, &jsonError);

	celixThreadMutex_lock(&endpoint->serviceLock);

	if (!root) {
		status = CELIX_ILLEGAL_ARGUMENT;
	} else if (endpoint->service != NULL) {
		int putAllRetVal;
		int arrSize;
		json_t* array;
		json_t *resultRoot;
		uint32_t samples_stored = 0;
		uint32_t arrayCnt;

		struct sample_queue_service* service = endpoint->service;

		json_unpack(root, "{s:[o]}", "a", &array);
		arrSize = json_array_size(array);

		struct sample workSample[arrSize];

		for (arrayCnt = 0; arrayCnt < arrSize; arrayCnt++) {
			uint64_t time;
			double value1;
			double value2;

			json_t* sample = json_array_get(array, arrayCnt);
			json_unpack(sample, "{s:I,s:f,s:f}", "sampleTime", &time, "value1", &value1, "value2", &value2);
			queueEndpoint_decodeToSample(time, value1, value2, &workSample[arrayCnt]);
		}

		putAllRetVal = service->putAll(service->sampleQueue, workSample, json_array_size(array), &samples_stored);

		if (putAllRetVal == 0) {
			resultRoot = json_pack("{s:i}", "r", samples_stored);
		}
		else {
			resultRoot = json_pack("{s:i}", "r", 0);
		}

		char *c = json_dumps(resultRoot, JSON_ENCODE_ANY);
		*reply = c;
		json_decref(resultRoot);
	} else {
		printf("QUEUE_ENDPOINT: No service available");
		status = CELIX_BUNDLE_EXCEPTION;
	}

	celixThreadMutex_unlock(&endpoint->serviceLock);

	json_decref(root);

	return status;
}

celix_status_t queueEndpoint_take(remote_endpoint_pt endpoint, char *data, char **reply) {
	celix_status_t status = CELIX_SUCCESS;
	json_error_t jsonError;
	json_t *root;

	root = json_loads(data, 0, &jsonError);

	celixThreadMutex_lock(&endpoint->serviceLock);

	if (!root) {
		status = CELIX_ILLEGAL_ARGUMENT;
	} else if (endpoint->service != NULL) {
		json_t *resultRoot;
		struct sample* workSample = calloc(1, sizeof(struct sample));
		struct sample_queue_service* service = endpoint->service;

		int result = service->take(service->sampleQueue, workSample);

		if (result == 0) {
			resultRoot = json_pack("{s:{s:i, s:f, s:f}}", "r", "sampleTime", workSample->time, "value1", workSample->value1, "value2", workSample->value2);
		}
		else
		{
			resultRoot = json_pack("n");
		}

		char *c = json_dumps(resultRoot, JSON_ENCODE_ANY);
		*reply = c;
		json_decref(resultRoot);

		free(workSample);
	} else {
		printf("QUEUE_ENDPOINT: No service available");
		status = CELIX_BUNDLE_EXCEPTION;
	}

	celixThreadMutex_unlock(&endpoint->serviceLock);

	json_decref(root);

	return status;
}

celix_status_t queueEndpoint_takeAll(remote_endpoint_pt endpoint, char *data, char **reply) {
	celix_status_t status = CELIX_SUCCESS;
	json_error_t jsonError;
	json_t *root;

	root = json_loads(data, 0, &jsonError);
	celixThreadMutex_lock(&endpoint->serviceLock);

	if (!root) {
		status = CELIX_ILLEGAL_ARGUMENT;
	} else if (endpoint->service != NULL) {
		struct sample_queue_service* service = endpoint->service;
		uint32_t min = 0;
		uint32_t max = 0;
		uint32_t numOfRecvSamples;
		int j, result, arrayCnt;
		json_t *resultRoot;
		json_t *array = json_array();


		json_unpack(root, "{s:[ii]}", "a", &min, &max);

		struct sample *recvSamples[max];

		for (j = 0; j < max; j++)
			recvSamples[j] = calloc(1, sizeof(struct sample));

		result = service->takeAll(service->sampleQueue, min, max, &recvSamples[0], &numOfRecvSamples);

		if (result == 0) {

			for (arrayCnt = 0; arrayCnt < numOfRecvSamples; arrayCnt++) {
				json_t *element = json_pack("{s:I,s:fs:f}", "sampleTime", recvSamples[arrayCnt]->time, "value1",recvSamples[arrayCnt]->value1, "value2", recvSamples[arrayCnt]->value2);
				json_array_append_new(array, element);
			}

			resultRoot = json_pack("{s:O}", "r", array);
		}
		else {
			resultRoot = json_pack("n");
		}

		char *c = json_dumps(resultRoot, JSON_ENCODE_ANY);
		*reply = c;

		json_decref(array);
		json_decref(resultRoot);

		for (j = 0; j < max; j++)
			free(recvSamples[j]);
	} else {
		printf("QUEUE_ENDPOINT: No service available");
		status = CELIX_BUNDLE_EXCEPTION;
	}

	celixThreadMutex_unlock(&endpoint->serviceLock);

	json_decref(root);

	return status;
}
