#include "processor_impl.h"


void *processSamples(void *handle) {
	struct activator *activator = handle;

	while (activator->running) {

		int i;

		for (i = 0; i < arrayList_size(activator->queueServices); i++) {
			int j, numOfRecvSamples;
			struct sample *recvSamples[MAX_BURST_LEN];
			struct sample_queue_service* qService = arrayList_get(activator->queueServices, i);

			for (j=0; j < MAX_BURST_LEN; j++)
				recvSamples[j] = calloc(1,sizeof(struct sample));

			printf("PROCESSOR: TakeAll (min: %d, max: %d)\n", MIN_BURST_LEN, MAX_BURST_LEN);

			if ( qService->takeAll(qService->sampleQueue, MIN_BURST_LEN, MAX_BURST_LEN , &recvSamples[0], &numOfRecvSamples) == 0) {
				printf("PROCESSOR:  %u samples received\n", numOfRecvSamples);

				for (j = 0; j < numOfRecvSamples; j++) {
					printf("\tPROCESSOR: ReceivedSample (%d/%d)  {Time:%llu | V1=%f | V2=%f} \n", j, numOfRecvSamples, recvSamples[j]->time, recvSamples[j]->value1, recvSamples[j]->value2);
				}
			}
			else  {
				printf("PROCESSOR: Could not take all samples.\n");
			}

			for (j=0; j < MAX_BURST_LEN; j++)
				free(recvSamples[j]);

			for(j = 0; j < 10; j++)
			{
				struct sample *recvSample = calloc(1,sizeof(struct sample));

				if ( qService->take(qService->sampleQueue, recvSample) == 0) {
					printf("\tPROCESSOR: ReceivedSample {Time:%llu | V1=%f | V2=%f} \n", recvSample->time, recvSample->value1, recvSample->value2);
				}
				else {
					printf("PROCESSOR: Could not take a single sample.\n");
				}

				free(recvSample);
			}

		}
	}

	pthread_exit(NULL);

	return NULL ;
}
