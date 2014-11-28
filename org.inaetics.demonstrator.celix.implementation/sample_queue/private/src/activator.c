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
#include <remote_constants.h>
#include <constants.h>

#include <array_list.h>
#include <pthread.h>

#include "inaetics_demonstrator_api/sample_queue.h"
#include "inaetics_demonstrator_api/service_statistics.h"

#include "arraylist_queue_service_impl.h"
#include "arraylist_queue_service_statistics_impl.h"

struct activator {
	service_registration_pt queueRegistration;
	struct sample_queue_service* queueService;

	service_registration_pt queueStatRegistration;
	struct service_statistics_service* queueStatService;
};

celix_status_t bundleActivator_create(bundle_context_pt context, void **userData) {
	celix_status_t status = CELIX_SUCCESS;

	*userData = calloc(1, sizeof(struct activator));

	if (*userData) {
		((struct activator *) *userData)->queueService = NULL;
		((struct activator *) *userData)->queueStatService = NULL;

	} else {
		status = CELIX_ENOMEM;
	}

	return status;
}

celix_status_t bundleActivator_start(void * userData, bundle_context_pt context) {
	celix_status_t status = CELIX_SUCCESS;
	struct activator *activator = userData;

	struct sample_queue_service* qService = NULL;
	struct service_statistics_service* qStatService = NULL;

	status = queueService_create(&qService);

	if(status==CELIX_SUCCESS){

		char* fwUUID = NULL;
		bundleContext_getProperty(context,OSGI_FRAMEWORK_FRAMEWORK_UUID,&fwUUID);

		status=queueStatService_create(&qStatService,qService,fwUUID);

		if (status == CELIX_SUCCESS){
			properties_pt queueProperties = NULL;

			activator->queueService = qService;

			queueProperties = properties_create();
			properties_set(queueProperties, (char *) OSGI_RSA_SERVICE_EXPORTED_INTERFACES, (char *) INAETICS_DEMONSTRATOR_API__SAMPLE_QUEUE_SERVICE_NAME);

			status = bundleContext_registerService(context, INAETICS_DEMONSTRATOR_API__SAMPLE_QUEUE_SERVICE_NAME,
					activator->queueService, queueProperties, &activator->queueRegistration);

			printf("SAMPLE_QUEUE: Service %s %s\n", INAETICS_DEMONSTRATOR_API__SAMPLE_QUEUE_SERVICE_NAME,
					status == CELIX_SUCCESS ? "registered" : "registration failed");

			if(status==CELIX_SUCCESS){
				properties_pt queueStatProperties = NULL;

				activator->queueStatService= qStatService;


				queueStatProperties = properties_create();
				properties_set(queueStatProperties, (char *) OSGI_RSA_SERVICE_EXPORTED_INTERFACES, (char *) INAETICS_DEMONSTRATOR_API_SERVICE_STATISTICS_SERVICE_NAME);

				status = bundleContext_registerService(context, INAETICS_DEMONSTRATOR_API_SERVICE_STATISTICS_SERVICE_NAME,
						activator->queueStatService, queueStatProperties, &activator->queueStatRegistration);

				printf("SAMPLE_QUEUE: Service %s (fwUUID=%s) %s\n", INAETICS_DEMONSTRATOR_API_SERVICE_STATISTICS_SERVICE_NAME,fwUUID,
						status == CELIX_SUCCESS ? "registered" : "registration failed");

			}

		}
		else{
			printf("SAMPLE_QUEUE: Cannot create Queue Statistics Service\n");
		}
	}
	else{
		printf("SAMPLE_QUEUE: Cannot create Queue Service\n");
	}

	return status;
}

celix_status_t bundleActivator_stop(void * userData, bundle_context_pt context) {

	celix_status_t status = CELIX_SUCCESS;
	struct activator *activator = userData;

	serviceRegistration_unregister(activator->queueStatRegistration);
	serviceRegistration_unregister(activator->queueRegistration);

	queueStatService_destroy(activator->queueStatService);
	queueService_destroy(activator->queueService) ;

	return status;
}

celix_status_t bundleActivator_destroy(void * userData, bundle_context_pt context) {
	celix_status_t status = CELIX_SUCCESS;
	struct activator *activator = userData;

	free(activator);

	return status;
}

