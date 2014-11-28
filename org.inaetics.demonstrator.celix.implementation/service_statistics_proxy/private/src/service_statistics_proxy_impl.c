#include <jansson.h>

#include <string.h>
#include <stddef.h>

#include "celix_errno.h"
#include "array_list.h"
#include "service_statistics_proxy_impl.h"

#include "inaetics_demonstrator_api/service_statistics.h"


celix_status_t serviceStatisticsProxy_setEndpointDescription(void *proxy, endpoint_description_pt endpoint);
celix_status_t serviceStatisticsProxy_setHandler(void *proxy, void *handler);
celix_status_t serviceStatisticsProxy_setCallback(void *proxy, sendToHandle callback);


celix_status_t serviceStatisticsProxy_create(bundle_context_pt context, service_statistics_type **stat)  {
	celix_status_t status = CELIX_SUCCESS;
	*stat = calloc(1, sizeof(**stat));
	if (!*stat) {
		status = CELIX_ENOMEM;
	} else {
		(*stat)->context = context;
		(*stat)->endpoint = NULL;
		(*stat)->sendToCallback = NULL;
		(*stat)->sendToHandler = NULL;
	}

	return status;
}


celix_status_t serviceStatisticsProxy_getServiceName(service_statistics_type* serviceStatistics, char** name){

	celix_status_t status = CELIX_SUCCESS;

	if (serviceStatistics->endpoint != NULL) {
		json_t *root;

		root = json_pack("{s:s, s:[]}", "m", "getServiceName", "a");

		char *data = json_dumps(root, 0);
		char *reply = NULL;
		int replyStatus = 0;

		status = serviceStatistics->sendToCallback(serviceStatistics->sendToHandler, serviceStatistics->endpoint, data, &reply, &replyStatus);

		if (status == CELIX_SUCCESS && replyStatus == 0) {
			json_error_t error;
			json_t *js_reply = json_loads(reply, 0, &error);
			if (js_reply) {

				char* sn = NULL;

				if (json_unpack(js_reply, "{s:s}", "serviceName", &sn) != 0){
					status = CELIX_BUNDLE_EXCEPTION;
				}
				else{
					*name=strdup(sn);
				}
			} else {
				printf("SERVICE_STATISTICS_PROXY: got error '%s' for '%s'\n", error.text, reply);
				status = CELIX_BUNDLE_EXCEPTION;
			}
			json_decref(js_reply);
		}
		else {
			status = replyStatus;
		}
		json_decref(root);

		free(data);
		free(reply);
	} else {
		printf("SERVICE_STATISTICS_PROXY: No endpoint information available\n");
		status = CELIX_BUNDLE_EXCEPTION;
	}

	return status;
}

celix_status_t serviceStatisticsProxy_getServiceType(service_statistics_type* serviceStatistics, char** type){

	celix_status_t status = CELIX_SUCCESS;

	if (serviceStatistics->endpoint != NULL) {
		json_t *root;

		root = json_pack("{s:s, s:[]}", "m", "getServiceType", "a");

		char *data = json_dumps(root, 0);
		char *reply = NULL;
		int replyStatus = 0;

		status = serviceStatistics->sendToCallback(serviceStatistics->sendToHandler, serviceStatistics->endpoint, data, &reply, &replyStatus);

		if (status == CELIX_SUCCESS && replyStatus == 0) {
			json_error_t error;
			json_t *js_reply = json_loads(reply, 0, &error);
			if (js_reply) {

				char* st = NULL;

				if (json_unpack(js_reply, "{s:s}", "serviceType", &st) != 0){
					status = CELIX_BUNDLE_EXCEPTION;
				}
				else{
					*type=strdup(st);
				}
			} else {
				printf("SERVICE_STATISTICS_PROXY: got error '%s' for '%s'\n", error.text, reply);
				status = CELIX_BUNDLE_EXCEPTION;
			}
			json_decref(js_reply);
		}
		else {
			status = replyStatus;
		}
		json_decref(root);

		free(data);
		free(reply);
	} else {
		printf("SERVICE_STATISTICS_PROXY: No endpoint information available\n");
		status = CELIX_BUNDLE_EXCEPTION;
	}

	return status;
}

celix_status_t serviceStatisticsProxy_getStatistic(service_statistics_type* serviceStatistics, double* statVal){

	celix_status_t status = CELIX_SUCCESS;

	if (serviceStatistics->endpoint != NULL) {
		json_t *root;

		root = json_pack("{s:s, s:[]}", "m", "getStatistic", "a");

		char *data = json_dumps(root, 0);
		char *reply = NULL;
		int replyStatus = 0;

		status = serviceStatistics->sendToCallback(serviceStatistics->sendToHandler, serviceStatistics->endpoint, data, &reply, &replyStatus);

		if (status == CELIX_SUCCESS && replyStatus == 0) {
			json_error_t error;
			json_t *js_reply = json_loads(reply, 0, &error);
			if (js_reply) {


				if (json_unpack(js_reply, "{s:f}", "statistic", statVal) != 0){
					status = CELIX_BUNDLE_EXCEPTION;
				}

			} else {
				printf("SERVICE_STATISTICS_PROXY: got error '%s' for '%s'\n", error.text, reply);
				status = CELIX_BUNDLE_EXCEPTION;
			}
			json_decref(js_reply);
		}
		else {
			status = replyStatus;
		}
		json_decref(root);

		free(data);
		free(reply);
	} else {
		printf("SERVICE_STATISTICS_PROXY: No endpoint information available\n");
		status = CELIX_BUNDLE_EXCEPTION;
	}

	return status;
}

celix_status_t serviceStatisticsProxy_getMeasurementUnit(service_statistics_type* serviceStatistics, char** mUnit){

	celix_status_t status = CELIX_SUCCESS;

	if (serviceStatistics->endpoint != NULL) {
		json_t *root;

		root = json_pack("{s:s, s:[]}", "m", "getMeasurementUnit", "a");

		char *data = json_dumps(root, 0);
		char *reply = NULL;
		int replyStatus = 0;

		status = serviceStatistics->sendToCallback(serviceStatistics->sendToHandler, serviceStatistics->endpoint, data, &reply, &replyStatus);

		if (status == CELIX_SUCCESS && replyStatus == 0) {
			json_error_t error;
			json_t *js_reply = json_loads(reply, 0, &error);
			if (js_reply) {

				char* um = NULL;

				if (json_unpack(js_reply, "{s:s}", "unitMeasurement", &um) != 0){
					status = CELIX_BUNDLE_EXCEPTION;
				}
				else{
					*mUnit=strdup(um);
				}
			} else {
				printf("SERVICE_STATISTICS_PROXY: got error '%s' for '%s'\n", error.text, reply);
				status = CELIX_BUNDLE_EXCEPTION;
			}
			json_decref(js_reply);
		}
		else {
			status = replyStatus;
		}
		json_decref(root);

		free(data);
		free(reply);
	} else {
		printf("SERVICE_STATISTICS_PROXY: No endpoint information available\n");
		status = CELIX_BUNDLE_EXCEPTION;
	}

	return status;
}


celix_status_t serviceStatisticsProxy_registerProxyService(void* proxyFactoryService, endpoint_description_pt endpointDescription, void* rsa, sendToHandle sendToCallback) {
	celix_status_t status = CELIX_SUCCESS;

	remote_proxy_factory_service_pt serviceStatisticsProxyFactoryService = (remote_proxy_factory_service_pt) proxyFactoryService;
	service_statistics_type* stat = NULL;
	struct service_statistics_service* statService = NULL;

	serviceStatisticsProxy_create(serviceStatisticsProxyFactoryService->context, &stat);
	statService = calloc(1, sizeof(*statService));
	statService->serviceStatistics = stat;
	statService->getServiceName = serviceStatisticsProxy_getServiceName;
	statService->getServiceType = serviceStatisticsProxy_getServiceType;
	statService->getStatistic = serviceStatisticsProxy_getStatistic;
	statService->getMeasurementUnit =  serviceStatisticsProxy_getMeasurementUnit;

	properties_pt srvcProps = properties_create();
	properties_set(srvcProps, (char *) "proxy.interface", (char *) INAETICS_DEMONSTRATOR_API_SERVICE_STATISTICS_SERVICE_NAME);
	properties_set(srvcProps, (char *) "endpoint.framework.uuid", (char *) endpointDescription->frameworkUUID);

	service_registration_pt proxyReg = NULL;

	serviceStatisticsProxy_setEndpointDescription(stat, endpointDescription);
	serviceStatisticsProxy_setHandler(stat, rsa);
	serviceStatisticsProxy_setCallback(stat, sendToCallback);

	if (bundleContext_registerService(serviceStatisticsProxyFactoryService->context, INAETICS_DEMONSTRATOR_API_SERVICE_STATISTICS_SERVICE_NAME, statService, srvcProps, &proxyReg) != CELIX_SUCCESS)
	{
		printf("SERVICE_STATISTICS_PROXY: error while registering statistics service\n");
	}

	hashMap_put(serviceStatisticsProxyFactoryService->proxy_registrations, endpointDescription, proxyReg);


	return status;
}


celix_status_t serviceStatisticsProxy_unregisterProxyService(void* proxyFactoryService, endpoint_description_pt endpointDescription) {
	celix_status_t status = CELIX_SUCCESS;

	remote_proxy_factory_service_pt serviceStatisticsProxyFactoryService = (remote_proxy_factory_service_pt) proxyFactoryService;
	service_registration_pt proxyReg = hashMap_get(serviceStatisticsProxyFactoryService->proxy_registrations, endpointDescription);

	if (proxyReg != NULL)
	{
		serviceRegistration_unregister(proxyReg);
	}

	return status;
}


celix_status_t serviceStatisticsProxy_setEndpointDescription(void *proxy, endpoint_description_pt endpoint) {
	celix_status_t status = CELIX_SUCCESS;

	service_statistics_type* stat = proxy;
	stat->endpoint = endpoint;

	return status;
}


celix_status_t serviceStatisticsProxy_setHandler(void *proxy, void *handler) {
	celix_status_t status = CELIX_SUCCESS;

	service_statistics_type* stat = proxy;
	stat->sendToHandler = handler;

	return status;
}


celix_status_t serviceStatisticsProxy_setCallback(void *proxy, sendToHandle callback) {
	celix_status_t status = CELIX_SUCCESS;

	service_statistics_type* stat = proxy;
	stat->sendToCallback = callback;

	return status;
}

