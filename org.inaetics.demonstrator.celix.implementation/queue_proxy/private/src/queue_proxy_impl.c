/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
#include <jansson.h>

#include <string.h>
#include <stddef.h>

#include "celix_errno.h"
#include "array_list.h"
#include "queue_proxy_impl.h"

#include "inaetics_demonstrator_api/sample_queue.h"



celix_status_t queueProxy_create(bundle_context_pt context, sample_queue_type **queue)  {
	celix_status_t status = CELIX_SUCCESS;
	*queue = calloc(1, sizeof(**queue));
	if (!*queue) {
		status = CELIX_ENOMEM;
	} else {
		(*queue)->context = context;
		(*queue)->endpoint = NULL;
		(*queue)->sendToCallback = NULL;
		(*queue)->sendToHandler = NULL;
	}

	return status;
}


celix_status_t queueProxy_destroy(sample_queue_type **queue)  {
	celix_status_t status = CELIX_SUCCESS;

	free(*queue);
	*queue = NULL;

	return status;
}

// { "m": "" "a":["arg1", "arg2"] }
int queueProxy_put(void* queue, struct sample workSample, bool *sampleTaken) {
	celix_status_t status = CELIX_SUCCESS;
	sample_queue_type* sampleQueue =  (sample_queue_type*) queue;

	if (sampleQueue->endpoint != NULL) {
		json_t *root;

		root = json_pack("{s:s, s:[{s:I,s:f,s:f}]}", "m", "put(Lorg/inaetics/demonstrator/api/data/Sample;)Z", "a", "sampleTime", workSample.time, "value1", workSample.value1, "value2", workSample.value2);

		char *data = json_dumps(root, 0);
		char *reply = NULL;
		int replyStatus = 0;

		status = sampleQueue->sendToCallback(sampleQueue->sendToHandler, sampleQueue->endpoint, data, &reply, &replyStatus);

		if (status == CELIX_SUCCESS && replyStatus == 0) {
			json_error_t error;

			json_t *js_reply = json_loads(reply, JSON_DECODE_ANY, &error);

			if (!js_reply || json_unpack(js_reply, "{s:b}", "r", sampleTaken) != 0) {
				printf("QUEUE_PROXY: put: got error '%s' for '%s'\n", error.text, reply);
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
		printf("QUEUE_PROXY: No endpoint information available\n");
		status = CELIX_BUNDLE_EXCEPTION;
	}

	return status;
}

int queueProxy_putAll(void *queue, struct sample *samples, uint32_t size, uint32_t *samplesTaken)
{
	celix_status_t status = CELIX_SUCCESS;
    sample_queue_type* sampleQueue =  (sample_queue_type*) queue;

	if (sampleQueue->endpoint != NULL) {
		uint32_t arrayCnt = 0;
		json_t *root;
		json_t *array = json_array();

		for (arrayCnt = 0; arrayCnt < size; arrayCnt++) {
			json_t *element = json_pack("{s:I,s:f,s:f}", "sampleTime", samples[arrayCnt].time, "value1", samples[arrayCnt].value1, "value2", samples[arrayCnt].value2);
			json_array_append_new(array, element);
		}

		root = json_pack("{s:s, s:[O]}", "m", "putAll(Ljava/util/Collection;)I", "a", array);

		char *data = json_dumps(root, 0);
		char *reply = NULL;
		int replyStatus = 0;

		status = sampleQueue->sendToCallback(sampleQueue->sendToHandler, sampleQueue->endpoint, data, &reply, &replyStatus);

		if (status == CELIX_SUCCESS && replyStatus == 0) {
			json_error_t error;

			json_t *js_reply = json_loads(reply, JSON_DECODE_ANY, &error);
			if (!js_reply  || json_unpack(js_reply, "{s:i}", "r",  samplesTaken) != 0) {
				printf("QUEUE_PROXY: putAll: got error '%s' for '%s'\n", error.text, reply);
				status = CELIX_BUNDLE_EXCEPTION;
			}
			json_decref(js_reply);
		}
		else {
			status = replyStatus;
		}

		json_decref(array);
		json_decref(root);

		free(data);
		free(reply);
	} else {
		printf("QUEUE_PROXY: No endpoint information available\n");
		status = CELIX_BUNDLE_EXCEPTION;
	}

	return status;
}

int queueProxy_take(void* queue, struct sample *sample) {
	celix_status_t status = CELIX_SUCCESS;
	sample_queue_type* sampleQueue =  (sample_queue_type*) queue;

	if (sampleQueue->endpoint != NULL) {
		json_t *root;

		root = json_pack("{s:s, s:[]}", "m", "take()Lorg/inaetics/demonstrator/api/data/Sample;", "a");

		char *data = json_dumps(root, JSON_DECODE_ANY);
		char *reply = NULL;
		int replyStatus = 0;

		status = sampleQueue->sendToCallback(sampleQueue->sendToHandler, sampleQueue->endpoint, data, &reply, &replyStatus);

		if (status == CELIX_SUCCESS && replyStatus == 0) {
			json_error_t error;
			json_t *js_reply = json_loads(reply, JSON_DECODE_ANY, &error);
			if (js_reply) {
				json_t* js_sample = NULL;

				if (json_unpack(js_reply, "{s:o}", "r", &js_sample) != 0) {
					status = CELIX_BUNDLE_EXCEPTION;
				}
				else if (json_is_null(js_sample)) {
					status = CELIX_BUNDLE_EXCEPTION;
				}
				else if (json_unpack(js_sample, "{s:I,s:f,s:f}", "sampleTime", &sample->time, "value1", &sample->value1, "value1", &sample->value2) != 0) {
	                printf("QUEUE_PROXY: take: got error while json_unpack for '%s'\n", json_dumps(js_sample, JSON_DECODE_ANY));
					status = CELIX_BUNDLE_EXCEPTION;
				}

			} else {
				printf("QUEUE_PROXY: take: got error '%s' for '%s'\n", error.text, reply);
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
		printf("QUEUE_PROXY: No endpoint information available\n");
		status = CELIX_BUNDLE_EXCEPTION;
	}

	return status;
}

int queueProxy_takeAll(void* queue, uint32_t min, uint32_t max, struct sample **samples, uint32_t *samplesSize) {
	celix_status_t status = CELIX_SUCCESS;
    sample_queue_type* sampleQueue =  (sample_queue_type*) queue;

	if (sampleQueue->endpoint != NULL) {
		uint32_t arrayCnt = 0;
		json_t *root;

		root = json_pack("{s:s, s:[ii]}", "m", "takeMinMax(II)Ljava/util/Collection;", "a", min, max);

		char *data = json_dumps(root, 0);
		char *reply = NULL;
		int replyStatus = 0;

		status = sampleQueue->sendToCallback(sampleQueue->sendToHandler, sampleQueue->endpoint, data, &reply, &replyStatus);

		if (status == CELIX_SUCCESS && replyStatus == 0) {
			json_error_t error;
			json_t *js_reply = json_loads(reply, JSON_DECODE_ANY, &error);
			if (js_reply) {
				json_t* array;


				if (json_unpack(js_reply, "{s:o}", "r", &array) != 0) {
                    printf("PROXY: unpack to array failed!\n");
					status = CELIX_BUNDLE_EXCEPTION;
				}
				else if (json_is_array(array))
				{
					*samplesSize = json_array_size(array);

					for (arrayCnt = 0; arrayCnt < *samplesSize; arrayCnt++) {
						json_t* sample = json_array_get(array, arrayCnt);
						if(json_unpack(sample, "{s:I,s:f,s:f}", "sampleTime", &samples[arrayCnt]->time, "value1", &samples[arrayCnt]->value1, "value2", &samples[arrayCnt]->value2) != 0) {
		                    printf("PROXY: error while unpacking array element %d : %s\n", arrayCnt, json_dumps(sample, JSON_DECODE_ANY));
						}
					}
				}
				else
				{
				    printf("QUEUEPROXY: takeAll: received value is no array! (reply: %s)\n", reply);
					*samplesSize = 0;
				}
			} else {
				printf("QUEUEPROXY: takeAll: got error '%s' for '%s'\n", error.text, reply);
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
		printf("QUEUE_PROXY: No endpoint information available\n");
		status = CELIX_BUNDLE_EXCEPTION;
	}

	return status;
}

