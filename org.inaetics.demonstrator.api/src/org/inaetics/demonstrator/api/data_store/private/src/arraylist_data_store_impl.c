#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "array_list.h"
#include "celix_errno.h"

#include "arraylist_data_store_impl.h"

static void msg(int lvl, char *fmsg, ...) {
	if (lvl <= VERBOSE) {
		char msg[512];
		va_list listPointer;
		va_start(listPointer, fmsg);
		vsprintf(msg, fmsg, listPointer);

		printf("[%d] : %s\n", lvl, msg);
	}
}

celix_status_t dataStoreService_create(struct data_store_service** dsService, data_store_type** dsHandler) {

	celix_status_t status = CELIX_ENOMEM;

	*dsService = calloc(1, sizeof(struct data_store_service));

	*dsHandler = calloc(1, sizeof(struct data_store));

	if ((*dsService != NULL) && (*dsHandler != NULL)) {

		pthread_mutex_init(&((*dsHandler)->lock), NULL);
		pthread_cond_init(&(*dsHandler)->listEmpty, NULL);

		arrayList_create(&((*dsHandler)->store));

		(*dsService)->dataStore = *dsHandler;
		(*dsService)->store = dataStoreService_store;
		(*dsService)->storeAll = dataStoreService_storeAll;
		(*dsService)->findResultsBetween = NULL;

		status = CELIX_SUCCESS;

	}

	return status;

}

celix_status_t dataStoreService_destroy(struct data_store_service* dsService, data_store_type* dsHandler) {

	celix_status_t status = CELIX_SUCCESS;

	pthread_mutex_lock(&(dsHandler->lock));

	/* Empty the queue */
	array_list_iterator_pt iter = arrayListIterator_create(dsHandler->store);

	while (arrayListIterator_hasNext(iter)) {
		free(arrayListIterator_next(iter));
	}

	arrayListIterator_destroy(iter);

	/* Destroy the queue */

	arrayList_destroy(dsHandler->store);
	dsHandler->store = NULL;

	pthread_mutex_unlock(&(dsHandler->lock));

	pthread_mutex_destroy(&(dsHandler->lock));

	free(dsService);
	free(dsHandler);

	return status;

}

static bool dataStoreService_isStoreFull(array_list_pt store)
{
	return ! ((MAX_STORE_SIZE == 0) || (arrayList_size(store) < MAX_STORE_SIZE));
}

int dataStoreService_store(data_store_type *dataStore, struct result result, bool *resultStored) {

	celix_status_t status = CELIX_SUCCESS;

	if (dataStore->store != NULL) {
		pthread_mutex_lock(&dataStore->lock);

		if (!dataStoreService_isStoreFull(dataStore->store))
		{
			struct result* s = calloc(1, sizeof(struct result));

			if (s != NULL) {

				memcpy(s, &result, sizeof(struct result));
				bool ret = arrayList_add(dataStore->store, s);
				if (ret) {
					msg(3, "DATA_STORE: stored result {%llu | %f | [ %llu # %f # %f ] }to queue", s->time, s->value1, s->sample.time, s->sample.value1, s->sample.value2);
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
		pthread_mutex_unlock(&dataStore->lock);
	}
	else {
		msg(0, "DATA_STORE: store denied because service is removed");
		status = CELIX_ILLEGAL_STATE;
	}

	return (int) status;
}



int dataStoreService_storeAll(data_store_type *dataStore, struct result *results, uint32_t size, uint32_t *storedResult) {

	celix_status_t status = CELIX_SUCCESS;
	uint32_t i = 0;
	uint32_t results_added = 0;

	if (dataStore->store != NULL) {
		pthread_mutex_lock(&dataStore->lock);

		msg(3, "DATA_STORE: Adding a burst of %u results", size);

		for (; (i < size) && (!dataStoreService_isStoreFull(dataStore->store)); i++) {

			struct result* s = calloc(1, sizeof(struct result));

			if (s != NULL) {

				memcpy(s, &results[i], sizeof(struct result));

				if (arrayList_add(dataStore->store, s)) {
					msg(3, "\tDATA_STORE: stored result {%llu | %f | [ %llu # %f # %f ] }to queue", s->time, s->value1, s->sample.time, s->sample.value1, s->sample.value2);
					results_added++;
					pthread_cond_signal(&dataStore->listEmpty);
				}
			}
			else {
				status = CELIX_ENOMEM;
				break;
			}

		}

		msg(3, "DATA_STORE: End burst");
		pthread_mutex_unlock(&dataStore->lock);

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

	return (int) status;

}
