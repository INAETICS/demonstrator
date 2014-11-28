#include <stdlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>

#include "array_list.h"
#include "celix_errno.h"

#include "arraylist_data_store_statistics_impl.h"


static void msg(int lvl, char *fmsg, ...) {
	if (lvl <= VERBOSE) {
		char msg[512];
		va_list listPointer;
		va_start(listPointer, fmsg);
		vsprintf(msg, fmsg, listPointer);

		printf("[%d] : %s\n", lvl, msg);
	}
}

celix_status_t dataStoreStatService_create(struct service_statistics_service** dsStatService, struct data_store_service* dsService, char* fwUUID){

	celix_status_t status = CELIX_ENOMEM;
	service_statistics_type *dsStats =  NULL;

	char buf[512];

	*dsStatService = calloc(1, sizeof(struct service_statistics_service));
	dsStats = calloc(1, sizeof(struct service_statistics));

	if ((*dsStatService != NULL) && (dsStats!= NULL)) {

		memset(buf,0,512);
		snprintf(buf,512,"%s %s",DS_STATS_NAME_PREFIX,fwUUID);
		dsStats->service_stats_name=strdup(buf);

		dsStats->service_stats_type=strdup(DS_STATS_TYPE);

		dsStats->service_stats_measurementUnit=strdup(DS_STATS_UNIT);

		dsStats->dsService=dsService;

		(*dsStatService)->serviceStatistics = dsStats;
		(*dsStatService)->getServiceName =  dataStoreStatService_getServiceStatName;
		(*dsStatService)->getServiceType = dataStoreStatService_getServiceStatType;
		(*dsStatService)->getStatistic = dataStoreStatService_getServiceStatVal;
		(*dsStatService)->getMeasurementUnit =  dataStoreStatService_getServiceStatUnit;

		status = CELIX_SUCCESS;
	}

	return status;


}

celix_status_t dataStoreStatService_destroy(struct service_statistics_service* dsStatService){

	celix_status_t status = CELIX_SUCCESS;

	if(dsStatService!=NULL){

		service_statistics_type* dsStatHandler =  dsStatService->serviceStatistics;

		if(dsStatHandler!=NULL){

			if(dsStatHandler->service_stats_name!=NULL){
				free(dsStatHandler->service_stats_name);
				dsStatHandler->service_stats_name=NULL;
			}

			if(dsStatHandler->service_stats_type!=NULL){
				free(dsStatHandler->service_stats_type);
				dsStatHandler->service_stats_type = NULL;
			}

			if(dsStatHandler->service_stats_measurementUnit!=NULL){
				free(dsStatHandler->service_stats_measurementUnit);
				dsStatHandler->service_stats_measurementUnit = NULL;
			}

			dsStatHandler->dsService = NULL;

		}

		free(dsStatService->serviceStatistics);
		dsStatService->serviceStatistics = NULL;

		free(dsStatService);

	}

	return status;


}

int dataStoreStatService_getServiceStatName(service_statistics_type *serviceStatistics, char **name){

	celix_status_t status = CELIX_SUCCESS;

	if (serviceStatistics->service_stats_name != NULL) {
		*name=strdup(serviceStatistics->service_stats_name);
		//msg(0, "DATASTORE_STAT: Got getName request, answer is %s",*name);
	}
	else {
		msg(0, "DATASTORE_STAT: getName denied because service is removed");
		status = CELIX_ILLEGAL_STATE;
	}

	return (int) status;

}

int dataStoreStatService_getServiceStatType(service_statistics_type *serviceStatistics, char **type){

	celix_status_t status = CELIX_SUCCESS;

	if (serviceStatistics->service_stats_type != NULL) {
		*type=strdup(serviceStatistics->service_stats_type);
		//msg(0, "DATASTORE_STAT: Got getType request, answer is %s",*type);
	}
	else {
		msg(0, "DATASTORE_STAT: getType denied because service is removed");
		status = CELIX_ILLEGAL_STATE;
	}

	return (int) status;

}


int dataStoreStatService_getServiceStatUnit(service_statistics_type *serviceStatistics, char **mUnit){
	celix_status_t status = CELIX_SUCCESS;

	if (serviceStatistics->service_stats_measurementUnit != NULL) {
		*mUnit=strdup(serviceStatistics->service_stats_measurementUnit);
		//msg(0, "DATASTORE_STAT: Got getMUnit request, answer is %s",*mUnit);
	}
	else {
		msg(0, "DATASTORE_STAT: getStatUnit denied because service is removed");
		status = CELIX_ILLEGAL_STATE;
	}

	return (int) status;

}


int dataStoreStatService_getServiceStatVal(service_statistics_type *serviceStatistics, double* statVal){

	celix_status_t status = CELIX_SUCCESS;

	if (serviceStatistics!=NULL && serviceStatistics->dsService!=NULL) {
		status=serviceStatistics->dsService->getLoad(serviceStatistics->dsService->dataStore,statVal);
		//msg(0, "DATASTORE_STAT: Got getStatistic request, answer is %f",*statVal);
	}
	else {
		msg(0, "DATASTORE_STAT: getStatVal denied because service is removed");
		status = CELIX_ILLEGAL_STATE;
	}

	return (int) status;

}

