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
#include "arraylist_data_store_impl.h"

struct activator {
	service_registration_pt dataStoreRegistration;
	struct data_store_service* dataStoreService;
};

celix_status_t bundleActivator_create(bundle_context_pt context, void **userData) {
	celix_status_t status = CELIX_SUCCESS;

	*userData = calloc(1, sizeof(struct activator));

	if (*userData) {
		((struct activator *) *userData)->dataStoreService = NULL;
	} else {
		status = CELIX_ENOMEM;
	}
	return status;
}

celix_status_t bundleActivator_start(void * userData, bundle_context_pt context) {
	celix_status_t status = CELIX_SUCCESS;
	struct activator *activator = userData;

	struct data_store_service* dsService = NULL;

	status = dataStoreService_create(&dsService);

	if (status == CELIX_SUCCESS){
		properties_pt properties = NULL;

		activator->dataStoreService = dsService;

		properties = properties_create();
		properties_set(properties, (char *) OSGI_RSA_SERVICE_EXPORTED_INTERFACES, (char *) INAETICS_DEMONSTRATOR_API__DATA_STORE_SERVICE_NAME);

		status = bundleContext_registerService(context, INAETICS_DEMONSTRATOR_API__DATA_STORE_SERVICE_NAME,
				activator->dataStoreService, properties, &activator->dataStoreRegistration);

		printf("DATA_STORE: Service %s %s\n", INAETICS_DEMONSTRATOR_API__DATA_STORE_SERVICE_NAME,
				status == CELIX_SUCCESS ? "registered" : "registration failed");

	}

	return status;
}

celix_status_t bundleActivator_stop(void * userData, bundle_context_pt context) {
	celix_status_t status = CELIX_SUCCESS;
	struct activator *activator = userData;

	serviceRegistration_unregister(activator->dataStoreRegistration);
	status = dataStoreService_destroy(activator->dataStoreService);

	return status;
}

celix_status_t bundleActivator_destroy(void * userData, bundle_context_pt context) {
	celix_status_t status = CELIX_SUCCESS;
	struct activator *activator = userData;

	free(activator);

	return status;
}

