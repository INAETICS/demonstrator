#include <jansson.h>

#include <string.h>
#include <stddef.h>

#include "celix_errno.h"
#include "array_list.h"
#include "stats_provider_proxy_impl.h"

#include "inaetics_demonstrator_api/stats_provider.h"


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

celix_status_t statsProviderProxy_destroy(stats_provider_type **stat)  {
    celix_status_t status = CELIX_SUCCESS;

    free(*stat);
    *stat = NULL;

    return status;
}

int statsProviderProxy_getName(void* provider, char** name) {

	celix_status_t status = CELIX_SUCCESS;
	stats_provider_type* statsProvider = (stats_provider_type*) provider;

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

				if (json_unpack(js_reply, "{s:s}", "r", &sn) != 0) {
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
		else if (replyStatus != 0) {
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

int statsProviderProxy_getType(void* provider, char** type) {

	celix_status_t status = CELIX_SUCCESS;
    stats_provider_type* statsProvider = (stats_provider_type*) provider;

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

				if (json_unpack(js_reply, "{s:s}", "r", &st) != 0) {
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
		else if (replyStatus != 0) {
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

int statsProviderProxy_getvalue(void* provider, double* statVal) {

	celix_status_t status = CELIX_SUCCESS;
    stats_provider_type* statsProvider = (stats_provider_type*) provider;

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

				if (json_unpack(js_reply, "{s:f}", "r", statVal) != 0) {
					status = CELIX_BUNDLE_EXCEPTION;
				}

			} else {
				printf("STATS_PROVIDER_PROXY: got error '%s' for '%s'\n", error.text, reply);
				status = CELIX_BUNDLE_EXCEPTION;
			}
			json_decref(js_reply);
		}
		else if (replyStatus != 0) {
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

int statsProviderProxy_getMeasurementUnitUnit(void* provider, char** mUnit) {

	celix_status_t status = CELIX_SUCCESS;
    stats_provider_type* statsProvider = (stats_provider_type*) provider;

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

				if (json_unpack(js_reply, "{s:s}", "r", &um) != 0) {
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
		else if (replyStatus != 0) {
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
