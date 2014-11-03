#include <stdbool.h>
#include <string.h>
#include <jansson.h>

#include "celix_errno.h"

#include "queue_endpoint_impl.h"

celix_status_t queueEndpoint_create(remote_endpoint_pt *endpoint) {
	celix_status_t status = CELIX_SUCCESS;

	*endpoint = calloc(1, sizeof(**endpoint));
	if (!*endpoint) {
		status = CELIX_ENOMEM;
	} else {
		(*endpoint)->service = NULL;
	}

	return status;
}

celix_status_t queueEndpoint_setService(remote_endpoint_pt endpoint, void *service) {
	celix_status_t status = CELIX_SUCCESS;
	endpoint->service = service;
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

	printf("QUEUE_ENDPOINT: Handle request \"%s\" with data \"%s\"\n", sig, data);

	if (strcmp(sig, "put(L)Z") == 0) {
		queueEndpoint_put(endpoint, data, reply);
	} else if (strcmp(sig, "putAll(L)D") == 0) {
		queueEndpoint_putAll(endpoint, data, reply);
	} else {
		status = CELIX_ILLEGAL_ARGUMENT;
	}

	return status;
}

static celix_status_t queueEndpoint_decodeToSample(uint64_t time, double value1, double value2,
		struct sample* workSample) {

	struct localSample {
		uint64_t time;
		double value1;
		double value2;
	} local;

	local.time = time;
	local.value1 = value1;
	local.value2 = value2;

	memcpy(workSample, &local, sizeof(struct localSample));

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

		struct sample_queue_service* service = endpoint->service;

		uint64_t time;
		double value1;
		double value2;

		json_unpack(root, "{s:[Iff]}", "d", &time, &value1, &value2);

		if (endpoint->service != NULL ) {
			json_t *resultRoot;
			struct sample workSample;
			bool result = false;

			queueEndpoint_decodeToSample(time, value1, value2, &workSample);
			service->put(service->sampleQueue, workSample, &result);
			resultRoot = json_pack("{s:b}", "r", result);

			char *c = json_dumps(resultRoot, 0);
			*reply = c;
		} else {
			printf("QUEUE_ENDPOINT: No service available");
			status = CELIX_BUNDLE_EXCEPTION;
		}
	}

	return status;
}

celix_status_t queueEndpoint_putAll(remote_endpoint_pt endpoint, char *data, char **reply) {
	celix_status_t status = CELIX_SUCCESS;
	json_error_t jsonError;
	json_t *root;

	root = json_loads(data, 0, &jsonError);
	if (!root) {
		status = CELIX_ILLEGAL_ARGUMENT;
	} else if (endpoint->service != NULL ) {
		int arrSize;
		json_t* array;
		json_t *resultRoot;
		uint32_t samples_stored = 0;
		uint32_t arrayCnt;
		uint32_t size;

		struct sample_queue_service* service = endpoint->service;

		json_unpack(root, "{s:o}", "d", &array);
		arrSize = json_array_size(array);

		struct sample workSample[arrSize];

		for (arrayCnt = 0; arrayCnt < arrSize; arrayCnt++) {
			uint64_t time;
			double value1;
			double value2;

			json_t* sample = json_array_get(array, arrayCnt);
			json_unpack(sample, "{s:[Iff]}", "d", &time, &value1, &value2);

			queueEndpoint_decodeToSample(time, value1, value2, &workSample[arrayCnt]);
		}

		service->putAll(service->sampleQueue, workSample, json_array_size(array), &samples_stored);
		resultRoot = json_pack("{s:i}", "r", samples_stored);

		char *c = json_dumps(resultRoot, 0);
		*reply = c;
	} else {
		printf("QUEUE_ENDPOINT: No service available");
		status = CELIX_BUNDLE_EXCEPTION;
	}

	return status;
}
