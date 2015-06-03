/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
#include <jansson.h>

#include <string.h>
#include <stddef.h>

#include "celix_errno.h"
#include "array_list.h"
#include "data_store_proxy_impl.h"

#include "inaetics_demonstrator_api/data_store.h"
#include "inaetics_demonstrator_api/sample_queue.h"


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

celix_status_t dataStoreProxy_destroy(data_store_type **data_store)  {
    celix_status_t status = CELIX_SUCCESS;

    free(*data_store);
    *data_store = NULL;

    return status;
}

int dataStoreProxy_store(void* store, struct result workResult, bool *resultStored) {

	celix_status_t status = CELIX_SUCCESS;
	int result = 0;
    data_store_type* dataStore = (data_store_type*) store;

	if (dataStore->endpoint != NULL) {

		json_t* sample = json_pack("{s:I,s:f,s:f}", "sampleTime", workResult.sample.time, "value1", workResult.sample.value1, "value2", workResult.sample.value2);
		json_t* result = json_pack("[{s:I,s:f,s:O}]", "processingTime", workResult.time, "result1", workResult.value1, "sample", sample);
		json_t *root = json_pack("{s:s, s:O}", "m", "store(Lorg/inaetics/demonstrator/api/data/Result;)V", "a", result);

		char *data = json_dumps(root, 0);
		char *reply = NULL;
		int replyStatus = 0;

		status = dataStore->sendToCallback(dataStore->sendToHandler, dataStore->endpoint, data, &reply, &replyStatus);

		json_decref(root);

		free(data);
		free(reply);
	} else {
		printf("DATA_STORE_PROXY: No endpoint information available\n");
		status = CELIX_BUNDLE_EXCEPTION;
	}

	return (status == CELIX_SUCCESS) ? result : (int) status;
}

int dataStoreProxy_storeAll(void* store, struct result *results, uint32_t size, uint32_t *storedResults)
{
	celix_status_t status = CELIX_SUCCESS;
	int result = 0;
	data_store_type* dataStore = (data_store_type*) store;

	if (dataStore->endpoint != NULL) {
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

		status = dataStore->sendToCallback(dataStore->sendToHandler, dataStore->endpoint, data, &reply, &replyStatus);

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
