/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
#include <jansson.h>

#include <string.h>
#include <stddef.h>

#include "celix_errno.h"
#include "array_list.h"
#include "producer_proxy_impl.h"

#include "inaetics_demonstrator_api/producer.h"

#define TAG	"PRODUCER_PROXY"

celix_status_t producerProxy_create(bundle_context_pt context, producer_type_pt* producer) {
	celix_status_t status = CELIX_SUCCESS;
	*producer = calloc(1, sizeof(**producer));
	if (!*producer) {
		status = CELIX_ENOMEM;
	} else {
		(*producer)->context = context;
		(*producer)->endpoint = NULL;
		(*producer)->sendToCallback = NULL;
		(*producer)->sendToHandler = NULL;
	}

	return status;
}

celix_status_t producerProxy_destroy(producer_type_pt* producer) {
	celix_status_t status = CELIX_SUCCESS;

	free(*producer);
	*producer = NULL;

	return status;
}

int producerProxy_getMaxSampleRate(void* producer) {
	producer_type_pt producer_type = (producer_type_pt) producer;
	int maxSampleRate = -1;

	if (producer_type->endpoint != NULL) {
		celix_status_t status = CELIX_SUCCESS;
		json_t *root;

		root = json_pack("{s:s}", "m", "getMaxSampleRate()I");

		char *data = json_dumps(root, 0);
		char *reply = NULL;
		int replyStatus = 0;

		status = producer_type->sendToCallback(producer_type->sendToHandler, producer_type->endpoint, data, &reply, &replyStatus);

		if (status == CELIX_SUCCESS && replyStatus == 0) {
			json_error_t error;

			json_t *js_reply = json_loads(reply, JSON_DECODE_ANY, &error);

			if (!js_reply || json_unpack(js_reply, "{s:i}", "r", maxSampleRate) != 0) {
				printf("%s: put: got error '%s' for '%s'\n", TAG, error.text, reply);
				status = CELIX_BUNDLE_EXCEPTION;
			}
			json_decref(js_reply);
		}

		json_decref(root);

		free(data);
		free(reply);
	} else {
		printf("%s: No endpoint information available\n", TAG);
	}

	return maxSampleRate;
}

int producerProxy_getSampleRate(void* producer) {
	producer_type_pt producer_type = (producer_type_pt) producer;
	int sampleRate = -1;

	if (producer_type->endpoint != NULL) {
		celix_status_t status = CELIX_SUCCESS;
		json_t *root;

		root = json_pack("{s:s}", "m", "getSampleRate()I");

		char *data = json_dumps(root, 0);
		char *reply = NULL;
		int replyStatus = 0;

		status = producer_type->sendToCallback(producer_type->sendToHandler, producer_type->endpoint, data, &reply, &replyStatus);

		if (status == CELIX_SUCCESS && replyStatus == 0) {
			json_error_t error;

			json_t *js_reply = json_loads(reply, JSON_DECODE_ANY, &error);

			if (!js_reply || json_unpack(js_reply, "{s:i}", "r", sampleRate) != 0) {
				printf("%s: put: got error '%s' for '%s'\n", TAG, error.text, reply);
				status = CELIX_BUNDLE_EXCEPTION;
			}
			json_decref(js_reply);
		}

		json_decref(root);

		free(data);
		free(reply);
	} else {
		printf("%s: No endpoint information available\n", TAG);
	}

	return sampleRate;
}

void producerProxy_setSampleRate(void* producer, int sampleRate) {
	producer_type_pt producer_type = (producer_type_pt) producer;

	if (producer_type->endpoint != NULL) {
		json_t *root;

		root = json_pack("{s:s, s:i}", "m", "setSampleRate()I", "a", sampleRate);

		char *data = json_dumps(root, 0);
		char *reply = NULL;
		int replyStatus = 0;

		producer_type->sendToCallback(producer_type->sendToHandler, producer_type->endpoint, data, &reply, &replyStatus);

		json_decref(root);

		free(data);
	} else {
		printf("%s: No endpoint information available\n", TAG);
	}
}
