#include "stats_provider_endpoint_impl.h"

#include <jansson.h>
#include <stdbool.h>
#include <string.h>

#include "celix_errno.h"

celix_status_t serviceStatisticsEndpoint_create(remote_endpoint_pt *endpoint) {
	celix_status_t status = CELIX_SUCCESS;

	*endpoint = calloc(1, sizeof(**endpoint));
	if (!*endpoint) {
		status = CELIX_ENOMEM;
	} else {
		(*endpoint)->service = NULL;
	}

	return status;
}

celix_status_t statsProviderEndpoint_setService(remote_endpoint_pt endpoint, void *service) {
	celix_status_t status = CELIX_SUCCESS;
	endpoint->service = service;
	return status;
}

/**
 * Request: http://host:port/services/{service}
 */
celix_status_t statsProviderEndpoint_handleRequest(remote_endpoint_pt endpoint, char *data, char **reply) {

	celix_status_t status = CELIX_SUCCESS;
	json_error_t jsonError;
	json_t *root;
	const char *sig;

	root = json_loads(data, 0, &jsonError);
	json_unpack(root, "{s:s}", "m", &sig);

	if (strcmp(sig, "getName()Ljava/lang/String;") == 0) {
		statsProviderEndpoint_getName(endpoint, data, reply);
	} else if (strcmp(sig, "getType()Ljava/lang/String;") == 0) {
		statsProviderEndpoint_getType(endpoint, data, reply);
	}
	else if (strcmp(sig, "getValue()D") == 0) {
		statsProviderEndpoint_getValue(endpoint, data, reply);
	}
	else if (strcmp(sig, "getMeasurementUnit()Ljava/lang/String;") == 0) {
		statsProviderEndpoint_getMeasurementUnit(endpoint, data, reply);
	}
	else {
		printf("SERVIVE_STATISTICS_ENDPOINT: Unknown request \"%s\" with data \"%s\"\n", sig, data);
		status = CELIX_ILLEGAL_ARGUMENT;
	}

	json_decref(root);

	return status;
}

celix_status_t statsProviderEndpoint_getName(remote_endpoint_pt endpoint, char *data, char **reply) {

	celix_status_t status = CELIX_SUCCESS;
	json_error_t jsonError;
	json_t *root;

	root = json_loads(data, 0, &jsonError);

	if (!root) {
		status = CELIX_ILLEGAL_ARGUMENT;
	} else if (endpoint->service != NULL) {
		json_t *resultRoot;

		char* name = NULL;
		struct stats_provider_service* service = endpoint->service;

		int result = service->getName(service->statsProvider, &name);

		if (result == 0) {
			resultRoot = json_pack("{s:s}", "r", name);
		}
		else
		{
			resultRoot = json_pack("n");
		}

		char *c = json_dumps(resultRoot, JSON_ENCODE_ANY);
		*reply = c;
		json_decref(resultRoot);

	} else {
		printf("STATS_PROVIDER_ENDPOINT: No service available");
		status = CELIX_BUNDLE_EXCEPTION;
	}

	json_decref(root);

	return status;
}

celix_status_t statsProviderEndpoint_getType(remote_endpoint_pt endpoint, char *data, char **reply) {

	celix_status_t status = CELIX_SUCCESS;
	json_error_t jsonError;
	json_t *root;

	root = json_loads(data, 0, &jsonError);

	if (!root) {
		status = CELIX_ILLEGAL_ARGUMENT;
	} else if (endpoint->service != NULL) {
		json_t *resultRoot;

		char* type = NULL;
		struct stats_provider_service* service = endpoint->service;

		int result = service->getType(service->statsProvider, &type);

		if (result == 0) {
			resultRoot = json_pack("{s:s}", "r", type);
		}
		else
		{
			resultRoot = json_pack("n");
		}

		char *c = json_dumps(resultRoot, JSON_ENCODE_ANY);
		*reply = c;
		json_decref(resultRoot);

	} else {
		printf("STATS_PROVIDER_ENDPOINT: No service available");
		status = CELIX_BUNDLE_EXCEPTION;
	}

	json_decref(root);

	return status;
}

celix_status_t statsProviderEndpoint_getMeasurementUnit(remote_endpoint_pt endpoint, char *data, char **reply) {

	celix_status_t status = CELIX_SUCCESS;
	json_error_t jsonError;
	json_t *root;

	root = json_loads(data, 0, &jsonError);

	if (!root) {
		status = CELIX_ILLEGAL_ARGUMENT;
	} else if (endpoint->service != NULL) {
		json_t *resultRoot;

		char* unitMeasurement = NULL;
		struct stats_provider_service* service = endpoint->service;

		int result = service->getMeasurementUnit(service->statsProvider, &unitMeasurement);

		if (result == 0) {
			resultRoot = json_pack("{s:s}", "r", unitMeasurement);
		}
		else
		{
			resultRoot = json_pack("n");
		}

		char *c = json_dumps(resultRoot, JSON_ENCODE_ANY);
		*reply = c;
		json_decref(resultRoot);

	} else {
		printf("STATS_PROVIDER_ENDPOINT: No service available");
		status = CELIX_BUNDLE_EXCEPTION;
	}

	json_decref(root);

	return status;
}

celix_status_t statsProviderEndpoint_getValue(remote_endpoint_pt endpoint, char *data, char **reply) {

	celix_status_t status = CELIX_SUCCESS;
	json_error_t jsonError;
	json_t *root;

	root = json_loads(data, 0, &jsonError);

	if (!root) {
		status = CELIX_ILLEGAL_ARGUMENT;
	} else if (endpoint->service != NULL) {
		json_t *resultRoot;

		double stat = 0.0f;
		struct stats_provider_service* service = endpoint->service;

		int result = service->getValue(service->statsProvider, &stat);

		if (result == 0) {
			resultRoot = json_pack("{s:f}", "r", stat);
		}
		else
		{
			resultRoot = json_pack("n");
		}

		char *c = json_dumps(resultRoot, JSON_ENCODE_ANY);
		*reply = c;
		json_decref(resultRoot);

	} else {
		printf("STATS_PROVIDER_ENDPOINT: No service available");
		status = CELIX_BUNDLE_EXCEPTION;
	}

	json_decref(root);

	return status;
}
