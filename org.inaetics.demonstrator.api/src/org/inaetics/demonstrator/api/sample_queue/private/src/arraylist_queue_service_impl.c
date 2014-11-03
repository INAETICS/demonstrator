#include <stdlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "array_list.h"
#include "celix_errno.h"

#include "arraylist_queue_service_impl.h"


int queueService_put(sample_queue_type *sampleQueue, struct sample sample, bool *sampleTaken){

	celix_status_t status = CELIX_BUNDLE_EXCEPTION;


	pthread_mutex_lock(&sampleQueue->lock);

	struct sample* s=calloc(1,sizeof(struct sample));

	if(s!=NULL){

		memcpy(s,&sample,sizeof(struct sample));
		bool ret=arrayList_add(sampleQueue->queue, s);
		if(ret){
			printf("SAMPLE_QUEUE: Added sample {%llu | %f | %f }to queue\n",s->time,s->value1,s->value2);
		}

		*sampleTaken=ret;

	}
	else{
		status=CELIX_ENOMEM;
	}
	pthread_mutex_unlock(&sampleQueue->lock);

	return (int)status;
}

int queueService_putAll(sample_queue_type *sampleQueue, struct sample *samples, uint32_t size, uint32_t *samplesTaken){

	celix_status_t status = CELIX_SUCCESS;
	uint32_t i=0;
	uint32_t samples_added=0;

	pthread_mutex_lock(&sampleQueue->lock);

	printf("SAMPLE_QUEUE: Adding a burst of %u samples\n",size);

	for(;i<size;i++){

		struct sample* s=calloc(1,sizeof(struct sample));

		if(s!=NULL){

			memcpy(s,&samples[i],sizeof(struct sample));

			if(arrayList_add(sampleQueue->queue, s)){
				printf("\tSAMPLE_QUEUE: Added sample {%llu | %f | %f } to queue\n",s->time,s->value1,s->value2);
				samples_added++;
			}
		}
		else{
			status=CELIX_ENOMEM;
			break;
		}

	}

	printf("SAMPLE_QUEUE: End burst\n");
	pthread_mutex_unlock(&sampleQueue->lock);

	*samplesTaken=samples_added;

	if(*samplesTaken!=size){
		printf("SAMPLE_QUEUE: Could not add all the requested samples (requested:%u, added%u)\n",size,*samplesTaken);
		if(status==CELIX_SUCCESS) //Don't mask the ENOMEM
			status=CELIX_BUNDLE_EXCEPTION;
	}

	return (int)status;


}

int queueService_take(sample_queue_type *sampleQueue, struct sample *sample){

	printf("SAMPLE_QUEUE: Sorry, not yet implemented\n");
	return -1;

}
int queueService_takeAll(sample_queue_type *sampleQueue, uint32_t min, uint32_t max, struct sample **samples, uint32_t *samplesSize){
	printf("SAMPLE_QUEUE: Sorry, not yet implemented\n");
	return -1;
}
