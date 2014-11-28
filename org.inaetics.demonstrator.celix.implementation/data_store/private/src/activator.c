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

#include "inaetics_demonstrator_api/data_store.h"
#include "inaetics_demonstrator_api/service_statistics.h"

#include "arraylist_data_store_impl.h"
#include "arraylist_data_store_statistics_impl.h"


struct activator {
	service_registration_pt dataStoreRegistration;
	struct data_store_service* dataStoreService;

	service_registration_pt dataStoreStatRegistration;
	struct service_statistics_service* dataStoreStatService;
};

celix_status_t bundleActivator_create(bundle_context_pt context, void **userData) {
	celix_status_t status = CELIX_SUCCESS;

	*userData = calloc(1, sizeof(struct activator));

	if (*userData) {
		((struct activator *) *userData)->dataStoreService = NULL;
		((struct activator *) *userData)->dataStoreStatService = NULL;
	} else {
		status = CELIX_ENOMEM;
	}
	return status;
}

celix_status_t bundleActivator_start(void * userData, bundle_context_pt context) {
	celix_status_t status = CELIX_SUCCESS;
	struct activator *activator = userData;

	struct data_store_service* dsService = NULL;
	struct service_statistics_service* dsStatService = NULL;

	status = dataStoreService_create(&dsService);

	if(status==CELIX_SUCCESS){

		char* fwUUID = NULL;
		bundleContext_getProperty(context,OSGI_FRAMEWORK_FRAMEWORK_UUID,&fwUUID);

		status=dataStoreStatService_create(&dsStatService,dsService,fwUUID);

		if (status == CELIX_SUCCESS){
			properties_pt dsProperties = NULL;

			activator->dataStoreService = dsService;

			dsProperties = properties_create();
			properties_set(dsProperties, (char *) OSGI_RSA_SERVICE_EXPORTED_INTERFACES, (char *) INAETICS_DEMONSTRATOR_API__DATA_STORE_SERVICE_NAME);

			status = bundleContext_registerService(context, INAETICS_DEMONSTRATOR_API__DATA_STORE_SERVICE_NAME,
					activator->dataStoreService, dsProperties, &activator->dataStoreRegistration);

			printf("DATA_STORE: Service %s %s\n", INAETICS_DEMONSTRATOR_API__DATA_STORE_SERVICE_NAME,
					status == CELIX_SUCCESS ? "registered" : "registration failed");

			if(status==CELIX_SUCCESS){
				properties_pt dsStatProperties = NULL;

				activator->dataStoreStatService = dsStatService;


				dsStatProperties = properties_create();
				properties_set(dsStatProperties, (char *) OSGI_RSA_SERVICE_EXPORTED_INTERFACES, (char *) INAETICS_DEMONSTRATOR_API_SERVICE_STATISTICS_SERVICE_NAME);

				status = bundleContext_registerService(context, INAETICS_DEMONSTRATOR_API_SERVICE_STATISTICS_SERVICE_NAME,
						activator->dataStoreStatService, dsStatProperties, &activator->dataStoreStatRegistration);

				printf("DATA_STORE: Service %s (fwUUID=%s) %s\n", INAETICS_DEMONSTRATOR_API_SERVICE_STATISTICS_SERVICE_NAME,fwUUID,
						status == CELIX_SUCCESS ? "registered" : "registration failed");

			}

		}
		else{
			printf("DATA_STORE: Cannot create DataStore Statistics Service\n");
		}
	}
	else{
		printf("DATA_STORE: Cannot create DataStore Service\n");
	}

	return status;
}

celix_status_t bundleActivator_stop(void * userData, bundle_context_pt context) {
	celix_status_t status = CELIX_SUCCESS;
	struct activator *activator = userData;

	serviceRegistration_unregister(activator->dataStoreStatRegistration);
	serviceRegistration_unregister(activator->dataStoreRegistration);

	dataStoreStatService_destroy(activator->dataStoreStatService);
	dataStoreService_destroy(activator->dataStoreService);

	return status;
}

celix_status_t bundleActivator_destroy(void * userData, bundle_context_pt context) {
	celix_status_t status = CELIX_SUCCESS;
	struct activator *activator = userData;

	free(activator);

	return status;
}

