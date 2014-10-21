/*
 * activator.c
 *
 *  Created on: 18 Sep 2014
 *      Author: abroekhuis
 */

#include <stdlib.h>
#include <unistd.h>

#include <sys/time.h>

#include <bundle_activator.h>
#include <service_tracker.h>
#include <constants.h>

#include <array_list.h>
#include <pthread.h>

#include "inaetics_demonstrator_api/sample_queue.h"
#include "queue_service_impl.h"


struct activator {
	sample_queue_type* queueHandler;

	service_registration_pt queueRegistration;

	struct sample_queue_service* queueService;

};



celix_status_t bundleActivator_create(bundle_context_pt context, void **userData) {
	celix_status_t status = CELIX_SUCCESS;

	*userData = calloc (1, sizeof(struct activator));

	if (*userData) {
		((struct activator *) *userData)->queueService = NULL;
		((struct activator *) *userData)->queueHandler = NULL;

	} else {
		status = CELIX_ENOMEM;
	}
	return status;
}


celix_status_t bundleActivator_start(void * userData, bundle_context_pt context) {
	celix_status_t status = CELIX_SUCCESS;
	struct activator *activator = userData;
	struct sample_queue_service* qService = calloc(1, sizeof(struct sample_queue_service));

	sample_queue_type* qHandler = calloc(1, sizeof(struct sample_queue));


	if (qService && qHandler)
	{

	    if (pthread_mutex_init(&qHandler->lock, NULL) != 0)
	    {
	        printf("SAMPLE_QUEUE: Mutex init failed\n");
	    }

		arrayList_create(&(qHandler->queue));

		qService->sampleQueue = qHandler;
		qService->put = queueService_put;
		qService->putAll = queueService_putAll;
		qService->take = queueService_take;
		qService->takeAll = queueService_takeAll;


		activator->queueHandler = qHandler;
		activator->queueService = qService;

		status = bundleContext_registerService(context, INAETICS_DEMONSTATOR_API__SAMPLE_QUEUE_SERVICE_NAME, activator->queueService, NULL,  &activator->queueRegistration);

		printf("SAMPLE_QUEUE: Service %s %s\n", INAETICS_DEMONSTATOR_API__SAMPLE_QUEUE_SERVICE_NAME,status==CELIX_SUCCESS?"registered":"registration failed");


	}

	return status;
}

celix_status_t bundleActivator_stop(void * userData, bundle_context_pt context) {
	celix_status_t status = CELIX_SUCCESS;
	struct activator *activator = userData;

	serviceRegistration_unregister(activator->queueRegistration);

	return status;
}

celix_status_t bundleActivator_destroy(void * userData, bundle_context_pt context) {
	celix_status_t status = CELIX_SUCCESS;
	struct activator *activator = userData;

	arrayList_destroy(activator->queueHandler->queue);
	pthread_mutex_destroy(&(activator->queueHandler->lock));

	free(activator->queueService);
	free(activator->queueHandler);

	free(activator);

	return status;
}

