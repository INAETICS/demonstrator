#include <stdbool.h>
#include <string.h>
#include <jansson.h>

#include "celix_errno.h"

#include "data_store_endpoint_impl.h"

celix_status_t dataStoreEndpoint_create(remote_endpoint_pt *endpoint) {
	celix_status_t status = CELIX_SUCCESS;

	*endpoint = calloc(1, sizeof(**endpoint));
	if (!*endpoint) {
		status = CELIX_ENOMEM;
	} else {
		(*endpoint)->service = NULL;
	}

	return status;
}

celix_status_t dataStoreEndpoint_setService(remote_endpoint_pt endpoint, void *service) {
	celix_status_t status = CELIX_SUCCESS;
	endpoint->service = service;
	return status;
}

/**
 * Request: http://host:port/services/{service}
 */
celix_status_t dataStoreEndpoint_handleRequest(remote_endpoint_pt endpoint, char *data, char **reply) {
	celix_status_t status = CELIX_SUCCESS;
	json_error_t jsonError;
	json_t *root;
	const char *sig;

	root = json_loads(data, 0, &jsonError);
	json_unpack(root, "{s:s}", "m", &sig);

	//printf("DATA_STORE_ENDPOINT: Handle request \"%s\" with data \"%s\"\n", sig, data);

	if (strcmp(sig, "store(LZ)D") == 0) {
		dataStoreEndpoint_store(endpoint, data, reply);
	} else if (strcmp(sig, "storeAll(LDD)D") == 0) {
		dataStoreEndpoint_storeAll(endpoint, data, reply);
	}
	else {
		status = CELIX_ILLEGAL_ARGUMENT;
	}

	json_decref(root);

	return status;
}

static celix_status_t dataStoreEndpoint_decodeToSample(uint64_t time, double value1, struct sample sample,struct result* workResult) {

	workResult->time=time;
	workResult->value1=value1;
	memcpy(&(workResult->sample),&sample,sizeof(struct sample));

	return CELIX_SUCCESS;
}

celix_status_t dataStoreEndpoint_store(remote_endpoint_pt endpoint, char *data, char **reply) {
	celix_status_t status = CELIX_SUCCESS;
	json_error_t jsonError;
	json_t *root;

	root = json_loads(data, 0, &jsonError);
	if (!root) {
		status = CELIX_ILLEGAL_ARGUMENT;
	} else {

		struct data_store_service* service = endpoint->service;

		uint64_t time;
		double value1;
		struct sample sample;

		json_unpack(root, "{s:[IfIff]}", "d", &time, &value1, &(sample.time),&(sample.value1),&(sample.value2));

		if (endpoint->service != NULL ) {
			json_t *resultRoot;
			struct result workResult;
			bool result = false;
			int putResult;

			dataStoreEndpoint_decodeToSample(time, value1, sample, &workResult);
			putResult = service->store(service->dataStore, workResult, &result);

			if (putResult == 0) {
				resultRoot = json_pack("{s:i, s:b}", "r", putResult , "d", result);
			}
			else {
				resultRoot = json_pack("{s:i, s:b}", "r", putResult , "d", false);
			}

			char *c = json_dumps(resultRoot, 0);

			*reply = c;
		} else {
			printf("DATA_STORE_ENDPOINT: No service available\n");
			status = CELIX_BUNDLE_EXCEPTION;
		}
	}

	json_decref(root);

	return status;
}


celix_status_t dataStoreEndpoint_storeAll(remote_endpoint_pt endpoint, char *data, char **reply) {
	celix_status_t status = CELIX_SUCCESS;
	json_error_t jsonError;
	json_t *root;

	root = json_loads(data, 0, &jsonError);

	if (!root) {
		status = CELIX_ILLEGAL_ARGUMENT;
	} else if (endpoint->service != NULL ) {
		int storeAllRetVal;
		int arrSize;
		json_t* array;
		json_t *resultRoot;
		uint32_t samples_stored = 0;
		uint32_t arrayCnt;

		struct data_store_service* service = endpoint->service;

		json_unpack(root, "{s:o}", "d", &array);
		arrSize = json_array_size(array);

		struct result workResult[arrSize];

		for (arrayCnt = 0; arrayCnt < arrSize; arrayCnt++) {
			uint64_t time;
			double value1;
			struct sample smpl;

			json_t* sample = json_array_get(array, arrayCnt);
			json_unpack(sample, "{s:[IfIff]}", "d", &time, &value1, &(smpl.time),&(smpl.value1),&(smpl.value2));

			dataStoreEndpoint_decodeToSample(time, value1, smpl, &workResult[arrayCnt]);
		}

		storeAllRetVal = service->storeAll(service->dataStore, workResult, json_array_size(array), &samples_stored);

		if (storeAllRetVal == 0) {
			resultRoot = json_pack("{s:i, s:i}", "r", storeAllRetVal, "d", samples_stored);
		}
		else {
			resultRoot = json_pack("{s:i, s:i}", "r", storeAllRetVal, "d", 0);
		}

		char *c = json_dumps(resultRoot, 0);
		*reply = c;
		json_decref(resultRoot);
	} else {
		printf("DATA_STORE_ENDPOINT: No service available");
		status = CELIX_BUNDLE_EXCEPTION;
	}

	json_decref(root);

	return status;
}
