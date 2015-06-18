#include <stdarg.h>
#include <pthread.h>
#include <utils.h>
#include <unistd.h>

#include "statistic_tracker_impl.h"

struct statistic_tracker {
	hash_map_pt statServices;
	pthread_rwlock_t statLock;

	bool running;
};

unsigned int threadHash(void * thread) {
	return *((unsigned int*) thread);
}

int threadEqual(void* thread1, void* thread2) {
	return pthread_equal(*((pthread_t*) thread1), *((pthread_t*) thread2));
}

static void msg(int lvl, char *fmsg, ...) {
	if (lvl <= VERBOSE) {
		char msg[512];
		va_list listPointer;
		va_start(listPointer, fmsg);
		vsprintf(msg, fmsg, listPointer);

		printf("[%d] : %s\n", lvl, msg);
	}
}

void* statPoller(void* handle) {

	statistic_tracker_pt statTracker = (statistic_tracker_pt) handle;
	celix_status_t status = CELIX_SUCCESS;

	statTracker->running = true;

	while (statTracker->running && status == CELIX_SUCCESS) {

		struct stats_provider_service* statService = NULL;

		char* name = NULL;
		char* type = NULL;
		double statVal = 0.0f;
		char* mUnit = NULL;

		pthread_rwlock_rdlock(&statTracker->statLock);
		pthread_t self = pthread_self();
		statService = (struct stats_provider_service*) hashMap_get(statTracker->statServices, &self);

		if (statService != NULL) {
			statService->getName(statService->statsProvider, &name);
			statService->getType(statService->statsProvider, &type);
			statService->getValue(statService->statsProvider, &statVal);
			statService->getMeasurementUnit(statService->statsProvider, &mUnit);

			msg(1, "STAT_TRACKER: Statistic for %s (type %s): %f %s ", name, type, statVal, mUnit);
		} else {
			status = CELIX_BUNDLE_EXCEPTION;
		}
		pthread_rwlock_unlock(&statTracker->statLock);

		if (mUnit != NULL) {
			free(mUnit);
		}
		if (type != NULL) {
			free(type);
		}
		if (name != NULL) {
			free(name);
		}

		sleep(WAIT_TIME_SECONDS);
	}

	return NULL;

}

celix_status_t statistic_tracker_create(statistic_tracker_pt* statTracker) {
	celix_status_t status = CELIX_SUCCESS;

	statistic_tracker_pt lclStatTracker = calloc(1, sizeof(*lclStatTracker));

	if (lclStatTracker != NULL) {
		lclStatTracker->running = false;
		pthread_rwlock_init(&lclStatTracker->statLock, NULL);
		lclStatTracker->statServices = hashMap_create(threadHash, NULL, threadEqual, NULL);

		(*statTracker) = lclStatTracker;
	} else {
		status = CELIX_ENOMEM;
	}

	return status;
}

celix_status_t statistic_tracker_stop(statistic_tracker_pt statTracker) {

	celix_status_t status = CELIX_SUCCESS;

	printf("STAT_TRACKER: Stopping StatTracker.\n");
	statTracker->running = false;

	return status;
}

celix_status_t statistic_tracker_destroy(statistic_tracker_pt statTracker) {
	celix_status_t status = CELIX_SUCCESS;

	pthread_rwlock_wrlock(&statTracker->statLock);
	hashMap_destroy(statTracker->statServices, false, false);
	pthread_rwlock_unlock(&statTracker->statLock);

	pthread_rwlock_destroy(&statTracker->statLock);

	return status;
}

celix_status_t statistic_tracker_statServiceAdded(void *handle, service_reference_pt reference, void *service) {

	struct stats_provider_service* statService = (struct stats_provider_service*) service;
	statistic_tracker_pt statTracker = (statistic_tracker_pt) handle;
	pthread_t* thread_pt = calloc(1, sizeof(*thread_pt));
	char* name = NULL;

	pthread_rwlock_wrlock(&statTracker->statLock);
	pthread_create(thread_pt, NULL, statPoller, statTracker);
	hashMap_put(statTracker->statServices, thread_pt, service);
	pthread_rwlock_unlock(&statTracker->statLock);

	statService->getName(statService->statsProvider, &name);

	msg(1, "STAT_TRACKER: Service %s Added (handled by thread %lu)", name, (unsigned long) *thread_pt);

	return CELIX_SUCCESS;
}

celix_status_t statistic_tracker_statServiceRemoved(void *handle, service_reference_pt reference, void *service) {

	celix_status_t status = CELIX_BUNDLE_EXCEPTION;
	statistic_tracker_pt statTracker = (statistic_tracker_pt) handle;

	pthread_t* thread_pt = NULL;

	pthread_rwlock_wrlock(&statTracker->statLock);

	hash_map_iterator_pt iter = hashMapIterator_create(statTracker->statServices);

	while (hashMapIterator_hasNext(iter) && thread_pt == NULL) {
		hash_map_entry_pt entry = hashMapIterator_nextEntry(iter);

		if (service == hashMapEntry_getValue(entry)) {
			struct stats_provider_service* statService = (struct stats_provider_service*) service;
			char* name = NULL;

			statService->getName(statService->statsProvider, &name);

			thread_pt = hashMapEntry_getKey(entry);

			msg(1, "STAT_TRACKER: Service %s Removed. ", name);
			hashMap_remove(statTracker->statServices, thread_pt);
		}
	}

	hashMapIterator_destroy(iter);

	pthread_rwlock_unlock(&statTracker->statLock);

	if (thread_pt != NULL) {
		pthread_join(*thread_pt, NULL);
		msg(1, "STAT_TRACKER: Service Removed. Thread %lu stopped.", (unsigned long) *thread_pt);
		free(thread_pt);
		status = CELIX_SUCCESS;
	}

	return status;
}
