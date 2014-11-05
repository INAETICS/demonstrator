#include <stdarg.h>

#include "producer_impl.h"


static void msg(int lvl, char *fmsg, ...) {
    if (lvl <= VERBOSE) {
		char msg[512];
		va_list listPointer;
		va_start(listPointer, fmsg);
		vsprintf(msg, fmsg, listPointer);

    	printf("[%d] : %s\n", lvl, msg);
    }
}


static unsigned int rand_range(unsigned int min, unsigned int max)
{
	struct timespec ts_rand;
	clock_gettime(CLOCK_REALTIME, &ts_rand);
	srand(ts_rand.tv_nsec);
	double scaled = (double) (((double) rand()) / ((double) RAND_MAX));

	return (max - min + 1) * scaled + min;
}

static void fillSample(struct sample* s) {

	struct localSample ls;

	struct timeval tv;
	gettimeofday(&tv, NULL );
	ls.time = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	srand(ls.time);
	ls.value1 = (double) rand();
	ls.value2 = (double) rand();

	memcpy(s, &ls, sizeof(struct sample));

}

static void shutdownProducerThread(void) {
	msg(0, "PRODUCER: Producing Thread is shutting down...");
	pthread_exit(NULL );
}



void *produceSamples(void *handle) {
	struct activator *activator = handle;

	struct sample smpl;

	while (activator->running) {
		struct timespec ts;
		int i;
		int rc = 0;

		clock_gettime(CLOCK_REALTIME, &ts);
		ts.tv_sec += WAIT_TIME_SECONDS;

		pthread_mutex_lock(&activator->queueLock);

		/* block, till a queue is available */
		while ((arrayList_size(activator->queueServices) == 0) && (rc != ETIMEDOUT) && (activator->running)) {
			rc = pthread_cond_timedwait(&activator->queueAvailable, &activator->queueLock, &ts);
		}

		if (rc == 0) {
			for (i = 0; i < arrayList_size(activator->queueServices) && (activator->running); i++) {
				struct sample_queue_service* qService = arrayList_get(activator->queueServices, i);
				struct timespec ts_start;
				struct timespec ts_end;

				int burst_len = 0;
				int singleSampleCnt = 0;
				int burstSampleCnt = 0;


				clock_gettime(CLOCK_REALTIME, &ts_start);

				// send single samples per second
				for (ts_end = ts_start; (singleSampleCnt < SINGLE_SAMPLES_PER_SEC) && (ts_start.tv_sec == ts_end.tv_sec) && (activator->running); ) {
					bool ret = false;

					memset(&smpl, 0, sizeof(struct sample));
					fillSample(&smpl);

					qService->put(qService->sampleQueue, smpl, &ret);

					if (ret == true) {
						msg(3, "PRODUCER: Sample {Time:%llu | V1=%f | V2=%f} stored.", smpl.time, smpl.value1,
														smpl.value2);
						singleSampleCnt++;
					}
					else {
						msg(2, "PRODUCER: Could not store sample.");
					}

					clock_gettime(CLOCK_REALTIME, &ts_end);
				}

				while(ts_start.tv_sec >= ts_end.tv_sec) {
					clock_gettime(CLOCK_REALTIME, &ts_end);
				}

				clock_gettime(CLOCK_REALTIME, &ts_start);

				for (ts_end = ts_start; (burstSampleCnt < BURST_SAMPLES_PER_SEC) && (ts_start.tv_sec == ts_end.tv_sec) && (activator->running);) {
					burst_len = rand_range(MIN_BURST_LEN, MAX_BURST_LEN);
					uint32_t burst_samples_stored = 0;

					struct sample burst[burst_len];
					memset(burst, 0, burst_len * sizeof(struct sample));
					int j = 0;

					msg(3, "PRODUCER: Preparing burst of %u samples", burst_len);

					for (; j < burst_len; j++) {
						fillSample(&burst[j]);
						msg(3, "\tPRODUCER: Prepared sample {Time:%llu | V1=%f | V2=%f}", burst[j].time, burst[j].value1,
								burst[j].value2);
					}

					qService->putAll(qService->sampleQueue, burst, burst_len, &burst_samples_stored);
					burstSampleCnt+= burst_samples_stored;

					clock_gettime(CLOCK_REALTIME, &ts_end);
				}

				msg(1, "PRODUCER: %d single samples // %d burst samples sent.", singleSampleCnt, burstSampleCnt);

				while(ts_start.tv_sec >= ts_end.tv_sec) {
					clock_gettime(CLOCK_REALTIME, &ts_end);
				}
			}
		}

		pthread_mutex_unlock(&activator->queueLock);
	}

	shutdownProducerThread();
	return NULL ;
}
