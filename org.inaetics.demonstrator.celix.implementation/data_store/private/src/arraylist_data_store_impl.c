/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "array_list.h"
#include "celix_errno.h"

#include "arraylist_data_store_impl.h"

#define MAX_STORE_SIZE       	100000

#define WAIT_TIME_SECONDS       2
#define VERBOSE					3

#define UTILIZATION_NAME_POSTFIX 		" Statistics"
#define UTILIZATION_TYPE 				"(utilization)"
#define UTILIZATION_MEASUREMENT_UNIT	"%"

struct data_store {
	char *name;
	char *utilizationStatsName;
	pthread_mutex_t lock;
	pthread_cond_t listEmpty;
	array_list_pt store;
	volatile long currentStoreSize;
	long max_store_size;
};

static void msg(int lvl, char *fmsg, ...) {
	if (lvl <= VERBOSE) {
		char msg[512];
		va_list listPointer;
		va_start(listPointer, fmsg);
		vsprintf(msg, fmsg, listPointer);

		printf("[%d] : %s\n", lvl, msg);
	}
}

celix_status_t dataStore_create(char* name, data_store_type** result){

	celix_status_t status = CELIX_ENOMEM;
	data_store_type* dataStore = calloc(1, sizeof(struct data_store));

	if (dataStore != NULL) {
		dataStore->name = strdup(name);
		dataStore->utilizationStatsName = calloc(1, strlen(name) + strlen(UTILIZATION_NAME_POSTFIX) + 1);
	}

	if (dataStore != NULL && dataStore->name !=NULL && dataStore->utilizationStatsName !=NULL) {

		sprintf(dataStore->utilizationStatsName,"%s%s",dataStore->name,(char*)UTILIZATION_NAME_POSTFIX);

		pthread_mutex_init(&((dataStore)->lock), NULL);
		pthread_cond_init(&(dataStore)->listEmpty, NULL);

		arrayList_create(&((dataStore)->store));

		dataStore->currentStoreSize = 0;
		dataStore->max_store_size = MAX_STORE_SIZE;

		*result=dataStore;

		status = CELIX_SUCCESS;

	}

	return status;

}

celix_status_t dataStore_destroy(data_store_type* dataStore){

	celix_status_t status = CELIX_SUCCESS;


	pthread_mutex_lock(&(dataStore->lock));

	/* Empty the queue */
	array_list_iterator_pt iter = arrayListIterator_create(dataStore->store);

	while (arrayListIterator_hasNext(iter)) {
		free(arrayListIterator_next(iter));
	}

	arrayListIterator_destroy(iter);

	/* Destroy the queue */

	arrayList_destroy(dataStore->store);
	dataStore->store = NULL;

	pthread_mutex_unlock(&(dataStore->lock));
	pthread_mutex_destroy(&(dataStore->lock));

	free(dataStore->utilizationStatsName);
	free(dataStore->name);
	free(dataStore);

	return status;

}

static bool dataStoreService_isStoreFull(array_list_pt store)
{
	return !((MAX_STORE_SIZE == 0) || (arrayList_size(store) < MAX_STORE_SIZE));
}

int dataStore_store(data_store_type *dataStore, struct result result, bool *resultStored) {

	celix_status_t status = CELIX_SUCCESS;

	pthread_mutex_lock(&dataStore->lock);
	if (dataStore->store != NULL) {

		if (!dataStoreService_isStoreFull(dataStore->store))
		{
			struct result* s = calloc(1, sizeof(struct result));

			if (s != NULL) {

				memcpy(s, &result, sizeof(struct result));
				bool ret = arrayList_add(dataStore->store, s);
				if (ret) {
					msg(3, "DATA_STORE: stored result {%llu | %f | [ %llu # %f # %f ] }to queue", s->time, s->value1, s->sample.time, s->sample.value1, s->sample.value2);
					dataStore->currentStoreSize += 1;
					pthread_cond_signal(&dataStore->listEmpty);
				}
				else {
					free(s);
				}

				*resultStored = ret;

			}
			else {
				status = CELIX_ENOMEM;
			}
		}

	}
	else {
		msg(0, "DATA_STORE: store denied because service is removed");
		status = CELIX_ILLEGAL_STATE;
	}
	pthread_mutex_unlock(&dataStore->lock);

	return (int) status;
}

int dataStore_storeAll(data_store_type *dataStore, struct result *results, uint32_t size, uint32_t *storedResult) {

	celix_status_t status = CELIX_SUCCESS;
	uint32_t i = 0;
	uint32_t results_added = 0;

	pthread_mutex_lock(&dataStore->lock);

	if (dataStore->store != NULL) {

		msg(3, "DATA_STORE: Adding a burst of %u results", size);

		for (; (i < size) && (!dataStoreService_isStoreFull(dataStore->store)); i++) {

			struct result* s = calloc(1, sizeof(struct result));

			if (s != NULL) {

				memcpy(s, &results[i], sizeof(struct result));

				if (arrayList_add(dataStore->store, s)) {
					msg(3, "\tDATA_STORE: stored result {%llu | %f | [ %llu # %f # %f ] }to queue", s->time, s->value1, s->sample.time, s->sample.value1, s->sample.value2);
					results_added++;
					dataStore->currentStoreSize += 1;
					pthread_cond_signal(&dataStore->listEmpty);
				}
			}
			else {
				status = CELIX_ENOMEM;
				break;
			}

		}

		msg(3, "DATA_STORE: End burst");

		*storedResult = results_added;

		if (*storedResult != size) {
			msg(2, "DATA_STORE: Could not add all the requested results (requested:%u, added%u)", size, *storedResult);
			if (status == CELIX_SUCCESS) //Don't mask the ENOMEM
				status = CELIX_BUNDLE_EXCEPTION;
		}
	}
	else {
		msg(0, "DATA_STORE: storeAll denied because service is removed");
		status = CELIX_ILLEGAL_STATE;
	}

	pthread_mutex_unlock(&dataStore->lock);

	return (int) status;

}

int dataStore_getUtilizationStatsName(data_store_type *dataStore, char **name){
	celix_status_t status = CELIX_SUCCESS;

	if (dataStore->utilizationStatsName != NULL) {
		(*name) = strdup(dataStore->utilizationStatsName);
	}
	else {
		msg(0, "DATASTORE_STAT: getName denied because service is removed");
		status = CELIX_ILLEGAL_STATE;
	}

	return (int) status;
}

int dataStore_getUtilizationStatsType(data_store_type *dataStore, char **type){
	(*type) = strdup((char*)UTILIZATION_TYPE);
	return (int)CELIX_SUCCESS;
}

int dataStore_getUtilizationStatsValue(data_store_type *dataStore, double* statVal){
	//Note; read only access to maxstoreSize and currentstoreSize with no special need for precise value -> no synchronization needed.
	(*statVal)=(double)((((double)dataStore->currentStoreSize)/((double)dataStore->max_store_size))*100.0f);
	return (int)CELIX_SUCCESS;
}

int dataStore_getUtilizationStatsMeasurementUnit(data_store_type *dataStore, char **mUnit){
	(*mUnit) = strdup((char*) UTILIZATION_MEASUREMENT_UNIT);
	return (int)CELIX_SUCCESS;
}
