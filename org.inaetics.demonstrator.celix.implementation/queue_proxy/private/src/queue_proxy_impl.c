#include <jansson.h>

#include <string.h>
#include <stddef.h>

#include "celix_errno.h"
#include "array_list.h"
#include "queue_proxy_impl.h"

#include "inaetics_demonstrator_api/sample_queue.h"

celix_status_t queueProxy_setEndpointDescription(void *proxy, endpoint_description_pt endpoint);
celix_status_t queueProxy_setHandler(void *proxy, void *handler);
celix_status_t queueProxy_setCallback(void *proxy, sendToHandle callback);

celix_status_t queueProxy_create(bundle_context_pt context, sample_queue_type **queue) {
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

// { "m": "" "a":["arg1", "arg2"] }
celix_status_t queueProxy_put(sample_queue_type* queue, struct sample workSample, bool *sampleTaken) {
	celix_status_t status = CELIX_SUCCESS;

	if (queue->endpoint != NULL) {
		json_t *root;

		root = json_pack("{s:s, s:[{s:I,s:f,s:f}]}", "m", "put(Lorg/inaetics/demonstrator/api/data/Sample;)Z", "a", "sampleTime", workSample.time, "value1", workSample.value1, "value2", workSample.value2);

		char *data = json_dumps(root, 0);
		char *reply = NULL;
		int replyStatus = 0;

		status = queue->sendToCallback(queue->sendToHandler, queue->endpoint, data, &reply, &replyStatus);

		if (status == CELIX_SUCCESS && replyStatus == 0) {
			json_error_t error;
			json_t *js_reply = json_loads(reply, JSON_DECODE_ANY, &error);

			if (js_reply) {
				json_unpack(js_reply, "b", sampleTaken);
			} else {
				printf("PROXY: got error '%s' for '%s'\n", error.text, reply);
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

celix_status_t queueProxy_putAll(sample_queue_type *queue, struct sample *samples, uint32_t size, uint32_t *samplesTaken)
{
	celix_status_t status = CELIX_SUCCESS;

	if (queue->endpoint != NULL) {
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

		status = queue->sendToCallback(queue->sendToHandler, queue->endpoint, data, &reply, &replyStatus);

		if (status == CELIX_SUCCESS && replyStatus == 0) {
			json_error_t error;
			json_t *js_reply = json_loads(reply, JSON_DECODE_ANY, &error);
			if (js_reply) {
				json_unpack(js_reply, "i", samplesTaken);
			} else {
				printf("PROXY: got error '%s' for '%s'\n", error.text, reply);
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

int queueProxy_take(sample_queue_type* queue, struct sample *sample) {
	celix_status_t status = CELIX_SUCCESS;

	if (queue->endpoint != NULL) {
		json_t *root;

		root = json_pack("{s:s, s:[]}", "m", "take()Lorg/inaetics/demonstrator/api/data/Sample;", "a");

		char *data = json_dumps(root, 0);
		char *reply = NULL;
		int replyStatus = 0;

		status = queue->sendToCallback(queue->sendToHandler, queue->endpoint, data, &reply, &replyStatus);

		if (status == CELIX_SUCCESS && replyStatus == 0) {
			json_error_t error;
			json_t *js_reply = json_loads(reply, JSON_DECODE_ANY, &error);
			if (js_reply) {
				json_t* js_sample = NULL;

				if (json_unpack(js_reply, "o", &js_sample) != 0) {
					status = CELIX_BUNDLE_EXCEPTION;
				}
				else if (json_is_null(js_sample)) {
					status = CELIX_BUNDLE_EXCEPTION;
				}
				else if (json_unpack(js_sample, "{s:i,s:f,s:f}", "sampleTime", &sample->time, "value1", &sample->value1, "value1", &sample->value2) != 0) {
					status = CELIX_BUNDLE_EXCEPTION;
				}
			} else {
				printf("PROXY: got error '%s' for '%s'\n", error.text, reply);
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

int queueProxy_takeAll(sample_queue_type* queue, uint32_t min, uint32_t max, struct sample **samples, uint32_t *samplesSize) {
	celix_status_t status = CELIX_SUCCESS;

	if (queue->endpoint != NULL) {
		uint32_t arrayCnt = 0;
		json_t *root;

		root = json_pack("{s:s, s:[ii]}", "m", "takeMinMax(II)Ljava/util/Collection;", "a", min, max);

		char *data = json_dumps(root, 0);
		char *reply = NULL;
		int replyStatus = 0;

		status = queue->sendToCallback(queue->sendToHandler, queue->endpoint, data, &reply, &replyStatus);

		if (status == CELIX_SUCCESS && replyStatus == 0) {
			json_error_t error;
			json_t *js_reply = json_loads(reply, JSON_DECODE_ANY, &error);
			if (js_reply) {
				json_t* array;

				if (json_unpack(js_reply, "o", &array) != 0) {
					status = CELIX_BUNDLE_EXCEPTION;
				}
				else if (json_is_array(array))
				{
					*samplesSize = json_array_size(array);
					for (arrayCnt = 0; arrayCnt < *samplesSize; arrayCnt++) {
						json_t* sample = json_array_get(array, arrayCnt);
						json_unpack(sample, "{s:I,s:f,s:f]}", "sampleTime", &samples[arrayCnt]->time, "value1", &samples[arrayCnt]->value1, "value2", &samples[arrayCnt]->value2);
					}
				}
				else
				{
					*samplesSize = 0;
				}
			} else {
				printf("PROXY: got error '%s' for '%s'\n", error.text, reply);
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

celix_status_t queueProxy_registerProxyService(void* proxyFactoryService, endpoint_description_pt endpointDescription, void* rsa, sendToHandle sendToCallback) {
	celix_status_t status = CELIX_SUCCESS;

	remote_proxy_factory_service_pt queueProxyFactoryService = (remote_proxy_factory_service_pt) proxyFactoryService;
	sample_queue_type* queue = NULL;
	struct sample_queue_service* queueService = NULL;

	queueProxy_create(queueProxyFactoryService->context, &queue);
	queueService = calloc(1, sizeof(*queueService));
	queueService->sampleQueue = queue;
	queueService->put = (void *) queueProxy_put;
	queueService->putAll = (void *) queueProxy_putAll;
	queueService->take = (void *) queueProxy_take;
	queueService->takeAll = (void *) queueProxy_takeAll;

	properties_pt srvcProps = properties_create();
	properties_set(srvcProps, (char *) "proxy.interface", (char *) INAETICS_DEMONSTRATOR_API__SAMPLE_QUEUE_SERVICE_NAME);
	properties_set(srvcProps, (char *) "endpoint.framework.uuid", (char *) endpointDescription->frameworkUUID);

	service_registration_pt proxyReg = NULL;

	queueProxy_setEndpointDescription(queue, endpointDescription);
	queueProxy_setHandler(queue, rsa);
	queueProxy_setCallback(queue, sendToCallback);

	if (bundleContext_registerService(queueProxyFactoryService->context, INAETICS_DEMONSTRATOR_API__SAMPLE_QUEUE_SERVICE_NAME, queueService, srvcProps, &proxyReg) != CELIX_SUCCESS)
			{
		printf("QUEUE_PROXY: error while registering queue service\n");
	}

	hashMap_put(queueProxyFactoryService->proxy_registrations, endpointDescription, proxyReg);

	return status;
}

celix_status_t queueProxy_unregisterProxyService(void* proxyFactoryService, endpoint_description_pt endpointDescription) {
	celix_status_t status = CELIX_SUCCESS;

	remote_proxy_factory_service_pt queueProxyFactoryService = (remote_proxy_factory_service_pt) proxyFactoryService;
	service_registration_pt proxyReg = hashMap_get(queueProxyFactoryService->proxy_registrations, endpointDescription);

	if (proxyReg != NULL)
			{
		serviceRegistration_unregister(proxyReg);
	}

	return status;
}

celix_status_t queueProxy_setEndpointDescription(void *proxy, endpoint_description_pt endpoint) {
	celix_status_t status = CELIX_SUCCESS;

	sample_queue_type* queue = proxy;
	queue->endpoint = endpoint;

	return status;
}

celix_status_t queueProxy_setHandler(void *proxy, void *handler) {
	celix_status_t status = CELIX_SUCCESS;

	sample_queue_type* queue = proxy;
	queue->sendToHandler = handler;

	return status;
}

celix_status_t queueProxy_setCallback(void *proxy, sendToHandle callback) {
	celix_status_t status = CELIX_SUCCESS;

	sample_queue_type* queue = proxy;
	queue->sendToCallback = callback;

	return status;
}

