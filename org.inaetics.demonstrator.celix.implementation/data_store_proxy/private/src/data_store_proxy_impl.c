#include <jansson.h>

#include <string.h>
#include <stddef.h>

#include "celix_errno.h"
#include "array_list.h"
#include "data_store_proxy_impl.h"

#include "inaetics_demonstrator_api/data_store.h"
#include "inaetics_demonstrator_api/sample_queue.h"

celix_status_t dataStoreProxy_setEndpointDescription(void *proxy, endpoint_description_pt endpoint);
celix_status_t dataStoreProxy_setHandler(void *proxy, void *handler);
celix_status_t dataStoreProxy_setCallback(void *proxy, sendToHandle callback);

celix_status_t dataStoreProxy_create(bundle_context_pt context, data_store_type **data_store) {
	celix_status_t status = CELIX_SUCCESS;
	*data_store = calloc(1, sizeof(**data_store));
	if (!*data_store) {
		status = CELIX_ENOMEM;
	} else {
		(*data_store)->context = context;
		(*data_store)->endpoint = NULL;
		(*data_store)->sendToCallback = NULL;
		(*data_store)->sendToHandler = NULL;
	}

	return status;
}

celix_status_t dataStoreProxy_store(data_store_type* data_store, struct result workResult, bool *resultStored) {

	celix_status_t status = CELIX_SUCCESS;
	int result = 0;

	if (data_store->endpoint != NULL) {

		json_t* sample = json_pack("{s:I,s:f,s:f}", "sampleTime", workResult.sample.time, "value1", workResult.sample.value1, "value2", workResult.sample.value2);
		json_t* result = json_pack("[{s:I,s:f,s:O}]", "processingTime", workResult.time, "result1", workResult.value1, "sample", sample);
		json_t *root = json_pack("{s:s, s:O}", "m", "store(Lorg/inaetics/demonstrator/api/data/Result;)V", "a", result);

		char *data = json_dumps(root, 0);
		char *reply = NULL;
		int replyStatus = 0;

		status = data_store->sendToCallback(data_store->sendToHandler, data_store->endpoint, data, &reply, &replyStatus);

		json_decref(root);

		free(data);
		free(reply);
	} else {
		printf("DATA_STORE_PROXY: No endpoint information available\n");
		status = CELIX_BUNDLE_EXCEPTION;
	}

	return (status == CELIX_SUCCESS) ? result : (int) status;
}

celix_status_t dataStoreProxy_storeAll(data_store_type *store, struct result *results, uint32_t size, uint32_t *storedResults)
{
	celix_status_t status = CELIX_SUCCESS;
	int result = 0;

	if (store->endpoint != NULL) {
		uint32_t arrayCnt = 0;
		json_t *root;
		json_t *array = json_array();

		for (arrayCnt = 0; arrayCnt < size; arrayCnt++) {
			json_t *element = json_pack("{s:I,s:f,s:{s:I,s:f,s:f}}", "processingTime", results[arrayCnt].time, "result1", results[arrayCnt].value1, "sample", "sampleTime",
					results[arrayCnt].sample.time, "value1", results[arrayCnt].sample.value1, "value2", results[arrayCnt].sample.value2);
			json_array_append_new(array, element);
		}

		root = json_pack("{s:s, s:[O]}", "m", "storeAll(Ljava/util/Collection;)V", "a", array);

		char *data = json_dumps(root, 0);
		char *reply = NULL;
		int replyStatus = 0;

		status = store->sendToCallback(store->sendToHandler, store->endpoint, data, &reply, &replyStatus);

		if (status == CELIX_SUCCESS && replyStatus == 0) {
			json_error_t error;
			json_t *js_reply = json_loads(reply, 0, &error);
			if (js_reply) {
				json_unpack(js_reply, "{s:i, s:i}", "r", &result, "d", storedResults);

				if (result != 0)
					status = result;
			} else {
				printf("DATA_STORE_PROXY: got error '%s' for '%s'\n", error.text, reply);
				status = CELIX_BUNDLE_EXCEPTION;
			}
			json_decref(js_reply);
		}

		json_decref(array);
		json_decref(root);

		free(data);
		free(reply);
	} else {
		printf("DATA_STORE_PROXY: No endpoint information available\n");
		status = CELIX_BUNDLE_EXCEPTION;
	}

	return (status == CELIX_SUCCESS) ? result : (int) status;
}

celix_status_t dataStoreProxy_registerProxyService(void* proxyFactoryService, endpoint_description_pt endpointDescription, void* rsa, sendToHandle sendToCallback) {
	celix_status_t status = CELIX_SUCCESS;

	remote_proxy_factory_service_pt queueProxyFactoryService = (remote_proxy_factory_service_pt) proxyFactoryService;
	data_store_type* store = NULL;
	struct data_store_service* storeService = NULL;

	dataStoreProxy_create(queueProxyFactoryService->context, &store);
	storeService = calloc(1, sizeof(*storeService));
	storeService->dataStore = store;
	storeService->store = (void *)dataStoreProxy_store;
	storeService->storeAll = (void *)dataStoreProxy_storeAll;

	properties_pt srvcProps = properties_create();
	properties_set(srvcProps, (char *) "proxy.interface", (char *) INAETICS_DEMONSTRATOR_API__DATA_STORE_SERVICE_NAME);
	properties_set(srvcProps, (char *) "endpoint.framework.uuid", (char *) endpointDescription->frameworkUUID);

	service_registration_pt proxyReg = NULL;

	dataStoreProxy_setEndpointDescription(store, endpointDescription);
	dataStoreProxy_setHandler(store, rsa);
	dataStoreProxy_setCallback(store, sendToCallback);

	if (bundleContext_registerService(queueProxyFactoryService->context, INAETICS_DEMONSTRATOR_API__DATA_STORE_SERVICE_NAME, storeService, srvcProps, &proxyReg) != CELIX_SUCCESS)
	{
		printf("DATA_STORE_PROXY: error while registering queue service\n");
	}

	hashMap_put(queueProxyFactoryService->proxy_registrations, endpointDescription, proxyReg);

	return status;
}

celix_status_t dataStoreProxy_unregisterProxyService(void* proxyFactoryService, endpoint_description_pt endpointDescription) {
	celix_status_t status = CELIX_SUCCESS;

	remote_proxy_factory_service_pt queueProxyFactoryService = (remote_proxy_factory_service_pt) proxyFactoryService;
	service_registration_pt proxyReg = hashMap_get(queueProxyFactoryService->proxy_registrations, endpointDescription);

	if (proxyReg != NULL)
	{
		serviceRegistration_unregister(proxyReg);
	}

	return status;
}

celix_status_t dataStoreProxy_setEndpointDescription(void *proxy, endpoint_description_pt endpoint) {
	celix_status_t status = CELIX_SUCCESS;

	data_store_type * store = proxy;
	store->endpoint = endpoint;

	return status;
}

celix_status_t dataStoreProxy_setHandler(void *proxy, void *handler) {
	celix_status_t status = CELIX_SUCCESS;

	data_store_type * store = proxy;
	store->sendToHandler = handler;

	return status;
}

celix_status_t dataStoreProxy_setCallback(void *proxy, sendToHandle callback) {
	celix_status_t status = CELIX_SUCCESS;

	data_store_type * store = proxy;
	store->sendToCallback = callback;

	return status;
}

