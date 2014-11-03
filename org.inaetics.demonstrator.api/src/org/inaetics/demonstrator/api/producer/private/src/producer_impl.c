#include "producer_impl.h"

static unsigned int rand_range(unsigned int min, unsigned int max)
{
	srand(time(NULL));
    double scaled = (double)(((double)rand())/((double)RAND_MAX));

    return (max - min +1)*scaled + min;
}



static void fillSample(struct sample* s){

	struct localSample ls;

	struct timeval tv;
	gettimeofday(&tv,NULL);
	ls.time=(tv.tv_sec*1000)+(tv.tv_usec/1000);
	srand(ls.time);
	ls.value1=(double)rand();
	ls.value2=(double)rand();

	memcpy(s,&ls,sizeof(struct sample));

}


void *produceSamples(void *handle) {
	struct activator *activator = handle;

	struct sample smpl;

	while (activator->running) {
		int i;
		for (i = 0; i < arrayList_size(activator->queueServices); i++) {
			struct sample_queue_service* qService = arrayList_get(activator->queueServices, i);


			int putNum=0;
			for(;putNum<SINGLE_PUT_NUMBER;putNum++){
				memset(&smpl,0, sizeof(struct sample));

				fillSample(&smpl);

				bool ret=false;
				qService->put(qService->sampleQueue,smpl,&ret);

				if (ret==true)
				{
					printf("PRODUCER: Sample {Time:%llu | V1=%f | V2=%f} stored.\n",smpl.time,smpl.value1, smpl.value2);
				}
				else
				{
					printf("PRODUCER: Could not store sample.\n");
				}

				sleep(SAMPLE_GEN_PERIOD);

			}

			sleep(5*SAMPLE_GEN_PERIOD);

			int burst_len=rand_range(MIN_BURST_LEN,MAX_BURST_LEN);

			struct sample burst[burst_len];
			memset(burst,0,burst_len*sizeof(struct sample));
			int j=0;
			printf("PRODUCER: Preparing burst of %u samples\n",burst_len);
			for(;j<burst_len;j++){
				fillSample(&burst[j]);
				printf("\tPRODUCER: Prepared sample {Time:%llu | V1=%f | V2=%f} \n",burst[j].time,burst[j].value1, burst[j].value2);
				usleep(500000);
			}


			uint32_t burst_samples_stored=0;
			qService->putAll(qService->sampleQueue,burst,burst_len,&burst_samples_stored);

			sleep(5*SAMPLE_GEN_PERIOD);


		}
	}

	pthread_exit(NULL);
	return NULL;
}
