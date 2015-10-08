#include "producer_endpoint_impl.h"

#include <jansson.h>
#include <stdbool.h>
#include <string.h>

#include "celix_errno.h"

#define 	TAG		"PRODUCER_ENDPOINT"

celix_status_t producerEndpoint_create(remote_endpoint_pt *endpoint) {
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

celix_status_t producerEndpoint_destroy(remote_endpoint_pt *endpoint) {
	celix_status_t status = CELIX_SUCCESS;

	if (*endpoint) {
		celixThreadMutex_destroy(&(*endpoint)->serviceLock);
		free(*endpoint);
		*endpoint = NULL;
	}

	return status;
}

celix_status_t producerEndpoint_setService(remote_endpoint_pt endpoint, void *service) {
	celix_status_t status = CELIX_SUCCESS;

	celixThreadMutex_lock(&endpoint->serviceLock);
	endpoint->service = service;
	celixThreadMutex_unlock(&endpoint->serviceLock);

	return status;
}

celix_status_t producerEndpoint_handleRequest(remote_endpoint_pt endpoint, char *data, char **reply) {

	celix_status_t status = CELIX_SUCCESS;
	json_error_t jsonError;
	json_t *root;
	const char *sig;

	root = json_loads(data, 0, &jsonError);
	json_unpack(root, "{s:s}", "m", &sig);

	if (strcmp(sig, "getMaxSampleRate()I") == 0) {
		producerEndpoint_getMaxSampleRate(endpoint, data, reply);
	} else if (strcmp(sig, "getSampleRate()I") == 0) {
		producerEndpoint_getSampleRate(endpoint, data, reply);
	} else if (strcmp(sig, "setSampleRate(I)V") == 0) {
		producerEndpoint_setSampleRate(endpoint, data, reply);
	} else {
		printf("%s: Unknown request \"%s\" with data \"%s\"\n", TAG, sig, data);
		status = CELIX_ILLEGAL_ARGUMENT;
	}

	json_decref(root);

	return status;
}

celix_status_t producerEndpoint_getMaxSampleRate(remote_endpoint_pt endpoint, char *data, char **reply) {

	celix_status_t status = CELIX_SUCCESS;
	json_error_t jsonError;
	json_t *root;

	root = json_loads(data, 0, &jsonError);

	celixThreadMutex_lock(&endpoint->serviceLock);

	if (!root) {
		status = CELIX_ILLEGAL_ARGUMENT;
	} else if (endpoint->service != NULL) {
		json_t *resultRoot;
		producer_service_pt service = endpoint->service;

		int maxSampleRate = service->getMaxSampleRate(service->producer);
		resultRoot = json_pack("{s:i}", "r", maxSampleRate);

		*reply = json_dumps(resultRoot, JSON_ENCODE_ANY);

		json_decref(resultRoot);

	} else {
		printf("%s: No service available", TAG);
		status = CELIX_BUNDLE_EXCEPTION;
	}

	celixThreadMutex_unlock(&endpoint->serviceLock);

	json_decref(root);

	return status;
}

celix_status_t producerEndpoint_getSampleRate(remote_endpoint_pt endpoint, char *data, char **reply) {

	celix_status_t status = CELIX_SUCCESS;
	json_error_t jsonError;
	json_t *root;

	root = json_loads(data, 0, &jsonError);

	celixThreadMutex_lock(&endpoint->serviceLock);

	if (!root) {
		status = CELIX_ILLEGAL_ARGUMENT;
	} else if (endpoint->service != NULL) {
		json_t *resultRoot;
		producer_service_pt service = endpoint->service;

		int sampleRate = service->getSampleRate(service->producer);
		resultRoot = json_pack("{s:i}", "r", sampleRate);

		*reply = json_dumps(resultRoot, JSON_ENCODE_ANY);

		json_decref(resultRoot);
	} else {
		printf("%s: No service available", TAG);
		status = CELIX_BUNDLE_EXCEPTION;
	}

	celixThreadMutex_unlock(&endpoint->serviceLock);

	json_decref(root);

	return status;
}

celix_status_t producerEndpoint_setSampleRate(remote_endpoint_pt endpoint, char *data, char **reply) {

	celix_status_t status = CELIX_SUCCESS;
	json_error_t jsonError;
	json_t *root;

	root = json_loads(data, 0, &jsonError);

	celixThreadMutex_lock(&endpoint->serviceLock);

	if (!root) {
		status = CELIX_ILLEGAL_ARGUMENT;
	} else if (endpoint->service != NULL) {
		producer_service_pt service = endpoint->service;
		int sampleRate = 0;

		json_unpack(root, "{s:i}", "a", &sampleRate);

		service->setSampleRate(service->producer, sampleRate);

	} else {
		printf("%s: No service available", TAG);
		status = CELIX_BUNDLE_EXCEPTION;
	}

	celixThreadMutex_unlock(&endpoint->serviceLock);

	json_decref(root);

	return status;
}
