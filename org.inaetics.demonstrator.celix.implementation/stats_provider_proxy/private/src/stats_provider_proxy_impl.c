#include <jansson.h>

#include <string.h>
#include <stddef.h>

#include "celix_errno.h"
#include "array_list.h"
#include "stats_provider_proxy_impl.h"

#include "inaetics_demonstrator_api/stats_provider.h"

celix_status_t statsProviderProxy_setEndpointDescription(void *proxy, endpoint_description_pt endpoint);
celix_status_t statsProviderProxy_setHandler(void *proxy, void *handler);
celix_status_t statsProviderProxy_setCallback(void *proxy, sendToHandle callback);

celix_status_t statsProviderProxy_create(bundle_context_pt context, stats_provider_type **stat) {
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

celix_status_t statsProviderProxy_getName(stats_provider_type* statsProvider, char** name) {

	celix_status_t status = CELIX_SUCCESS;

	if (statsProvider->endpoint != NULL) {
		json_t *root;

		root = json_pack("{s:s, s:[]}", "m", "getName()Ljava/lang/String;", "a");

		char *data = json_dumps(root, 0);
		char *reply = NULL;
		int replyStatus = 0;

		status = statsProvider->sendToCallback(statsProvider->sendToHandler, statsProvider->endpoint, data, &reply, &replyStatus);

		if (status == CELIX_SUCCESS && replyStatus == 0) {
			json_error_t error;
			json_t *js_reply = json_loads(reply, JSON_DECODE_ANY, &error);
			if (js_reply) {

				char* sn = NULL;

				if (json_unpack(js_reply, "s", &sn) != 0) {
					status = CELIX_BUNDLE_EXCEPTION;
				}
				else {
					*name = strdup(sn);
				}
			} else {
				printf("STATS_PROVIDER_PROXY: got error '%s' for '%s'\n", error.text, reply);
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
		printf("STATS_PROVIDER_PROXY: No endpoint information available\n");
		status = CELIX_BUNDLE_EXCEPTION;
	}

	return status;
}

celix_status_t statsProviderProxy_getType(stats_provider_type* statsProvider, char** type) {

	celix_status_t status = CELIX_SUCCESS;

	if (statsProvider->endpoint != NULL) {
		json_t *root;

		root = json_pack("{s:s, s:[]}", "m", "getType()Ljava/lang/String;", "a");

		char *data = json_dumps(root, 0);
		char *reply = NULL;
		int replyStatus = 0;

		status = statsProvider->sendToCallback(statsProvider->sendToHandler, statsProvider->endpoint, data, &reply, &replyStatus);

		if (status == CELIX_SUCCESS && replyStatus == 0) {
			json_error_t error;
			json_t *js_reply = json_loads(reply, JSON_DECODE_ANY, &error);
			if (js_reply) {

				char* st = NULL;

				if (json_unpack(js_reply, "s", &st) != 0) {
					status = CELIX_BUNDLE_EXCEPTION;
				}
				else {
					*type = strdup(st);
				}
			} else {
				printf("STATS_PROVIDER_PROXY: got error '%s' for '%s'\n", error.text, reply);
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
		printf("STATS_PROVIDER_PROXY: No endpoint information available\n");
		status = CELIX_BUNDLE_EXCEPTION;
	}

	return status;
}

celix_status_t statsProviderProxy_getvalue(stats_provider_type* statsProvider, double* statVal) {

	celix_status_t status = CELIX_SUCCESS;

	if (statsProvider->endpoint != NULL) {
		json_t *root;

		root = json_pack("{s:s, s:[]}", "m", "getValue()D", "a");

		char *data = json_dumps(root, 0);
		char *reply = NULL;
		int replyStatus = 0;

		status = statsProvider->sendToCallback(statsProvider->sendToHandler, statsProvider->endpoint, data, &reply, &replyStatus);

		if (status == CELIX_SUCCESS && replyStatus == 0) {
			json_error_t error;
			json_t *js_reply = json_loads(reply, JSON_DECODE_ANY, &error);
			if (js_reply) {

				if (json_unpack(js_reply, "f", statVal) != 0) {
					status = CELIX_BUNDLE_EXCEPTION;
				}

			} else {
				printf("STATS_PROVIDER_PROXY: got error '%s' for '%s'\n", error.text, reply);
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
		printf("STATS_PROVIDER_PROXY: No endpoint information available\n");
		status = CELIX_BUNDLE_EXCEPTION;
	}

	return status;
}

celix_status_t statsProviderProxy_getMeasurementUnitUnit(stats_provider_type* statsProvider, char** mUnit) {

	celix_status_t status = CELIX_SUCCESS;

	if (statsProvider->endpoint != NULL) {
		json_t *root;

		root = json_pack("{s:s, s:[]}", "m", "getMeasurementUnit()Ljava/lang/String;", "a");

		char *data = json_dumps(root, 0);
		char *reply = NULL;
		int replyStatus = 0;

		status = statsProvider->sendToCallback(statsProvider->sendToHandler, statsProvider->endpoint, data, &reply, &replyStatus);

		if (status == CELIX_SUCCESS && replyStatus == 0) {
			json_error_t error;
			json_t *js_reply = json_loads(reply, JSON_DECODE_ANY, &error);
			if (js_reply) {

				char* um = NULL;

				if (json_unpack(js_reply, "s", &um) != 0) {
					status = CELIX_BUNDLE_EXCEPTION;
				}
				else {
					*mUnit = strdup(um);
				}
			} else {
				printf("STATS_PROVIDER_PROXY: got error '%s' for '%s'\n", error.text, reply);
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
		printf("STATS_PROVIDER_PROXY: No endpoint information available\n");
		status = CELIX_BUNDLE_EXCEPTION;
	}

	return status;
}

celix_status_t statsProviderProxy_registerProxyService(void* proxyFactoryService, endpoint_description_pt endpointDescription, void* rsa, sendToHandle sendToCallback) {
	celix_status_t status = CELIX_SUCCESS;

	remote_proxy_factory_service_pt statsProviderProxyFactoryService = (remote_proxy_factory_service_pt) proxyFactoryService;
	stats_provider_type* stat = NULL;
	struct stats_provider_service* statService = NULL;

	statsProviderProxy_create(statsProviderProxyFactoryService->context, &stat);
	statService = calloc(1, sizeof(*statService));
	statService->statsProvider = stat;
	statService->getName = (void*) statsProviderProxy_getName;
	statService->getType = (void*) statsProviderProxy_getType;
	statService->getValue = (void*) statsProviderProxy_getvalue;
	statService->getMeasurementUnit = (void*) statsProviderProxy_getMeasurementUnitUnit;

	properties_pt srvcProps = properties_create();
	properties_set(srvcProps, (char *) "proxy.interface", (char *) INAETICS_DEMONSTRATOR_API__STATS_PROVIDER_SERVICE_NAME);
	properties_set(srvcProps, (char *) "endpoint.framework.uuid", (char *) endpointDescription->frameworkUUID);

	service_registration_pt proxyReg = NULL;

	statsProviderProxy_setEndpointDescription(stat, endpointDescription);
	statsProviderProxy_setHandler(stat, rsa);
	statsProviderProxy_setCallback(stat, sendToCallback);

	if (bundleContext_registerService(statsProviderProxyFactoryService->context, INAETICS_DEMONSTRATOR_API__STATS_PROVIDER_SERVICE_NAME, statService, srvcProps, &proxyReg) != CELIX_SUCCESS) {
		printf("STATS_PROVIDER_PROXY: error while registering statistics service\n");
	}

	hashMap_put(statsProviderProxyFactoryService->proxy_registrations, endpointDescription, proxyReg);

	return status;
}

celix_status_t statsProviderProxy_unregisterProxyService(void* proxyFactoryService, endpoint_description_pt endpointDescription) {
	celix_status_t status = CELIX_SUCCESS;

	remote_proxy_factory_service_pt statsProviderProxyFactoryService = (remote_proxy_factory_service_pt) proxyFactoryService;
	service_registration_pt proxyReg = hashMap_get(statsProviderProxyFactoryService->proxy_registrations, endpointDescription);

	if (proxyReg != NULL) {
		serviceRegistration_unregister(proxyReg);
	}

	return status;
}

celix_status_t statsProviderProxy_setEndpointDescription(void *proxy, endpoint_description_pt endpoint) {
	celix_status_t status = CELIX_SUCCESS;

	stats_provider_type* stat = proxy;
	stat->endpoint = endpoint;

	return status;
}

celix_status_t statsProviderProxy_setHandler(void *proxy, void *handler) {
	celix_status_t status = CELIX_SUCCESS;

	stats_provider_type* stat = proxy;
	stat->sendToHandler = handler;

	return status;
}

celix_status_t statsProviderProxy_setCallback(void *proxy, sendToHandle callback) {
	celix_status_t status = CELIX_SUCCESS;

	stats_provider_type* stat = proxy;
	stat->sendToCallback = callback;

	return status;
}

