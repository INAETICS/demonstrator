#include <stdbool.h>
#include <string.h>
#include <jansson.h>

#include "celix_errno.h"

#include "service_statistics_endpoint_impl.h"


celix_status_t serviceStatisticsEndpoint_create(remote_endpoint_pt *endpoint){
	celix_status_t status = CELIX_SUCCESS;

	*endpoint = calloc(1, sizeof(**endpoint));
	if (!*endpoint) {
		status = CELIX_ENOMEM;
	} else {
		(*endpoint)->service = NULL;
	}

	return status;
}

celix_status_t serviceStatisticsEndpoint_setService(remote_endpoint_pt endpoint, void *service){
	celix_status_t status = CELIX_SUCCESS;
	endpoint->service = service;
	return status;
}

/**
 * Request: http://host:port/services/{service}
 */
celix_status_t serviceStatisticsEndpoint_handleRequest(remote_endpoint_pt endpoint, char *data, char **reply){

	celix_status_t status = CELIX_SUCCESS;
	json_error_t jsonError;
	json_t *root;
	const char *sig;

	root = json_loads(data, 0, &jsonError);
	json_unpack(root, "{s:s}", "m", &sig);

	if (strcmp(sig, "getServiceName") == 0) {
		serviceStatisticsEndpoint_getServiceName(endpoint, data, reply);
	} else if (strcmp(sig, "getServiceType") == 0) {
		serviceStatisticsEndpoint_getServiceType(endpoint, data, reply);
	}
	else if (strcmp(sig, "getStatistic") == 0) {
		serviceStatisticsEndpoint_getStatistic(endpoint, data, reply);
	}
	else if (strcmp(sig, "getMeasurementUnit") == 0) {
		serviceStatisticsEndpoint_getMeasurementUnit(endpoint, data, reply);
	}
	else {
		printf("SERVIVE_STATISTICS_ENDPOINT: Unknown request \"%s\" with data \"%s\"\n", sig, data);
		status = CELIX_ILLEGAL_ARGUMENT;
	}

	json_decref(root);

	return status;
}


celix_status_t serviceStatisticsEndpoint_getServiceName(remote_endpoint_pt endpoint, char *data, char **reply){

	celix_status_t status = CELIX_SUCCESS;
	json_error_t jsonError;
	json_t *root;

	root = json_loads(data, 0, &jsonError);

	if (!root) {
		status = CELIX_ILLEGAL_ARGUMENT;
	} else if (endpoint->service != NULL) {
		json_t *resultRoot;

		char* serviceName = NULL;
		struct service_statistics_service* service = endpoint->service;

		int result = service->getServiceName(service->serviceStatistics,&serviceName);

		if (result == 0) {
			resultRoot = json_pack("{s:s}", "serviceName", serviceName);
		}
		else
		{
			resultRoot = json_pack("n");
		}

		char *c = json_dumps(resultRoot, JSON_ENCODE_ANY);
		*reply = c;
		json_decref(resultRoot);

		/* serviceName is allocated by the getServiceName */
		free(serviceName);
	} else {
		printf("SERVICE_STATISTICS_ENDPOINT: No service available");
		status = CELIX_BUNDLE_EXCEPTION;
	}

	json_decref(root);

	return status;
}

celix_status_t serviceStatisticsEndpoint_getServiceType(remote_endpoint_pt endpoint, char *data, char **reply){

	celix_status_t status = CELIX_SUCCESS;
	json_error_t jsonError;
	json_t *root;

	root = json_loads(data, 0, &jsonError);

	if (!root) {
		status = CELIX_ILLEGAL_ARGUMENT;
	} else if (endpoint->service != NULL) {
		json_t *resultRoot;

		char* serviceType = NULL;
		struct service_statistics_service* service = endpoint->service;

		int result = service->getServiceType(service->serviceStatistics,&serviceType);

		if (result == 0) {
			resultRoot = json_pack("{s:s}", "serviceType", serviceType);
		}
		else
		{
			resultRoot = json_pack("n");
		}

		char *c = json_dumps(resultRoot, JSON_ENCODE_ANY);
		*reply = c;
		json_decref(resultRoot);

		/* serviceType is allocated by the getServiceType */
		free(serviceType);
	} else {
		printf("SERVICE_STATISTICS_ENDPOINT: No service available");
		status = CELIX_BUNDLE_EXCEPTION;
	}

	json_decref(root);

	return status;
}

celix_status_t serviceStatisticsEndpoint_getMeasurementUnit(remote_endpoint_pt endpoint, char *data, char **reply){

	celix_status_t status = CELIX_SUCCESS;
	json_error_t jsonError;
	json_t *root;

	root = json_loads(data, 0, &jsonError);

	if (!root) {
		status = CELIX_ILLEGAL_ARGUMENT;
	} else if (endpoint->service != NULL) {
		json_t *resultRoot;

		char* unitMeasurement = NULL;
		struct service_statistics_service* service = endpoint->service;

		int result = service->getMeasurementUnit(service->serviceStatistics,&unitMeasurement);

		if (result == 0) {
			resultRoot = json_pack("{s:s}", "unitMeasurement", unitMeasurement);
		}
		else
		{
			resultRoot = json_pack("n");
		}

		char *c = json_dumps(resultRoot, JSON_ENCODE_ANY);
		*reply = c;
		json_decref(resultRoot);

		/* unitMeasurement is allocated by the getUnitMeasurement */
		free(unitMeasurement);
	} else {
		printf("SERVICE_STATISTICS_ENDPOINT: No service available");
		status = CELIX_BUNDLE_EXCEPTION;
	}

	json_decref(root);

	return status;
}

celix_status_t serviceStatisticsEndpoint_getStatistic(remote_endpoint_pt endpoint, char *data, char **reply){

	celix_status_t status = CELIX_SUCCESS;
	json_error_t jsonError;
	json_t *root;

	root = json_loads(data, 0, &jsonError);

	if (!root) {
		status = CELIX_ILLEGAL_ARGUMENT;
	} else if (endpoint->service != NULL) {
		json_t *resultRoot;

		double stat = 0.0f;
		struct service_statistics_service* service = endpoint->service;

		int result = service->getStatistic(service->serviceStatistics,&stat);

		if (result == 0) {
			resultRoot = json_pack("{s:f}", "statistic", stat);
		}
		else
		{
			resultRoot = json_pack("n");
		}

		char *c = json_dumps(resultRoot, JSON_ENCODE_ANY);
		*reply = c;
		json_decref(resultRoot);


	} else {
		printf("SERVICE_STATISTICS_ENDPOINT: No service available");
		status = CELIX_BUNDLE_EXCEPTION;
	}

	json_decref(root);

	return status;
}
