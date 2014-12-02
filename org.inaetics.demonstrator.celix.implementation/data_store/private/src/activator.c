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
#include "inaetics_demonstrator_api/stats_provider.h"

#include "arraylist_data_store_impl.h"


struct activator {
	bundle_context_pt context;
	data_store_type *dataStore;

	service_registration_pt dataStoreRegistration;
	struct data_store_service* dataStoreService;

	service_registration_pt dataStoreStatsRegistration;
	struct stats_provider_service* dataStoreStatsService;
};

static celix_status_t bundleActivator_createDataStoreService(struct activator *act);
static celix_status_t bundleActivator_createStatsService(struct activator *act);

static celix_status_t bundleActivator_registerDataStoreService(struct activator *act);
static celix_status_t bundleActivator_registerStatsService(struct activator *act);


celix_status_t bundleActivator_create(bundle_context_pt context, void **userData) {
	celix_status_t status = CELIX_SUCCESS;

	*userData = calloc(1, sizeof(struct activator));

	if (*userData) {
		((struct activator *) *userData)->context = context;
		((struct activator *) *userData)->dataStore = NULL;
		((struct activator *) *userData)->dataStoreService = NULL;
		((struct activator *) *userData)->dataStoreRegistration = NULL;
		((struct activator *) *userData)->dataStoreStatsService = NULL;
		((struct activator *) *userData)->dataStoreStatsRegistration = NULL;
	} else {
		status = CELIX_ENOMEM;
	}
	return status;
}

celix_status_t bundleActivator_start(void * userData, bundle_context_pt context) {
	celix_status_t status = CELIX_SUCCESS;
	struct activator *activator = userData;

	//Creating data store Component

	//Setting up name for data store.
	char *name = NULL;
	char *uuid = NULL;
	char uuidName[128];
	memset(uuidName,0,128);

	bundleContext_getProperty(context, "inaetics.demonstrator.datastore.name", &name);
	bundleContext_getProperty(context, OSGI_FRAMEWORK_FRAMEWORK_UUID, &uuid);

	if(name!=NULL){
		snprintf(uuidName,128,"Data Store %s", name);
	}
	else if (name == NULL && uuid != NULL) {
		snprintf(uuidName,128,"Data Store %.8s", uuid);
	} else {
		snprintf(uuidName,128,"Data Store (unknown ID)");
	}


	status = dataStore_create(uuidName, &activator->dataStore);

	if(status==CELIX_SUCCESS) {

		//Creating service structs for services provided by the component
		bundleActivator_createDataStoreService(activator);
		bundleActivator_createStatsService(activator);

		if (activator->dataStoreService != NULL) {
			bundleActivator_registerDataStoreService(activator);
		}
		if (activator->dataStoreStatsService != NULL) {
			bundleActivator_registerStatsService(activator);
		}

		if (	activator->dataStoreService == NULL || activator->dataStoreStatsService == NULL ||
				activator->dataStoreRegistration == NULL || activator->dataStoreStatsRegistration == NULL) {
			printf("DATA STORE: Error creating/registering services\n");
			status = CELIX_BUNDLE_EXCEPTION;
		}
		else{
			printf("DATA STORE: Created datastore with name %s\n",uuidName);
		}
	}
	return status;
}

static celix_status_t bundleActivator_createDataStoreService(struct activator *act) {
	celix_status_t status = CELIX_SUCCESS;

	act->dataStoreService = calloc(1, sizeof(*act->dataStoreService));

	if (act->dataStoreService != NULL) {
		act->dataStoreService->dataStore = act->dataStore;
		act->dataStoreService->store= (void *)dataStore_store;
		act->dataStoreService->storeAll = (void *)dataStore_storeAll;
		act->dataStoreService->findResultsBetween = NULL;
	} else {
		status = CELIX_ENOMEM;
	}

	return status;
}

static celix_status_t bundleActivator_createStatsService(struct activator *act) {
	celix_status_t status = CELIX_SUCCESS;

	act->dataStoreStatsService = calloc(1, sizeof(*act->dataStoreStatsService));

	if (act->dataStoreStatsService != NULL) {
		act->dataStoreStatsService->statsProvider = act->dataStore;
		act->dataStoreStatsService->getName = (void *)dataStore_getUtilizationStatsName;
		act->dataStoreStatsService->getType = (void *)dataStore_getUtilizationStatsType;
		act->dataStoreStatsService->getMeasurementUnit = (void *)dataStore_getUtilizationStatsMeasurementUnit;
		act->dataStoreStatsService->getValue = (void *)dataStore_getUtilizationStatsValue;
	} else {
		status = CELIX_ENOMEM;
	}

	return status;
}

static celix_status_t bundleActivator_registerDataStoreService(struct activator *act) {
	celix_status_t status = CELIX_SUCCESS;
	properties_pt props = NULL;
	props = properties_create();
	properties_set(props, (char *) OSGI_RSA_SERVICE_EXPORTED_INTERFACES, (char *) INAETICS_DEMONSTRATOR_API__DATA_STORE_SERVICE_NAME);

	status = bundleContext_registerService(act->context, (char *)INAETICS_DEMONSTRATOR_API__DATA_STORE_SERVICE_NAME,
			act->dataStoreService, props, &act->dataStoreRegistration);

	return status;
}

static celix_status_t bundleActivator_registerStatsService(struct activator *act) {
	celix_status_t status = CELIX_SUCCESS;
	properties_pt props = NULL;
	props = properties_create();
	properties_set(props, (char *) OSGI_RSA_SERVICE_EXPORTED_INTERFACES, (char *) INAETICS_DEMONSTRATOR_API__STATS_PROVIDER_SERVICE_NAME);

	status = bundleContext_registerService(act->context, (char *)INAETICS_DEMONSTRATOR_API__STATS_PROVIDER_SERVICE_NAME,
			act->dataStoreStatsService, props, &act->dataStoreStatsRegistration);

	return status;
}


celix_status_t bundleActivator_stop(void * userData, bundle_context_pt context) {
	celix_status_t status = CELIX_SUCCESS;
	struct activator *activator = userData;

	serviceRegistration_unregister(activator->dataStoreStatsRegistration);
	serviceRegistration_unregister(activator->dataStoreRegistration);

	free(activator->dataStoreService);
	free(activator->dataStoreStatsService);

	dataStore_destroy(activator->dataStore);

	return status;
}

celix_status_t bundleActivator_destroy(void * userData, bundle_context_pt context) {
	celix_status_t status = CELIX_SUCCESS;
	struct activator *activator = userData;

	free(activator);

	return status;
}

