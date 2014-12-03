#include <stdlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>

#include "array_list.h"
#include "celix_errno.h"

#include "arraylist_queue_service_statistics_impl.h"

static void msg(int lvl, char *fmsg, ...) {
	if (lvl <= VERBOSE) {
		char msg[512];
		va_list listPointer;
		va_start(listPointer, fmsg);
		vsprintf(msg, fmsg, listPointer);

		printf("[%d] : %s\n", lvl, msg);
	}
}

celix_status_t queueStatService_create(struct service_statistics_service** qStatService, struct sample_queue_service* qService, char* fwUUID) {

	celix_status_t status = CELIX_ENOMEM;
	service_statistics_type *qStats = NULL;

	char buf[512];

	*qStatService = calloc(1, sizeof(struct service_statistics_service));
	qStats = calloc(1, sizeof(struct service_statistics));

	if ((*qStatService != NULL) && (qStats != NULL)) {

		memset(buf, 0, 512);
		snprintf(buf, 512, "%s %s", QUEUE_STATS_NAME_PREFIX, fwUUID);
		qStats->service_stats_name = strdup(buf);

		qStats->service_stats_type = strdup(QUEUE_STATS_TYPE);

		qStats->service_stats_measurementUnit = strdup(QUEUE_STATS_UNIT);

		qStats->qService = qService;

		(*qStatService)->serviceStatistics = qStats;
		(*qStatService)->getServiceName = queueStatService_getServiceStatName;
		(*qStatService)->getServiceType = queueStatService_getServiceStatType;
		(*qStatService)->getStatistic = queueStatService_getServiceStatVal;
		(*qStatService)->getMeasurementUnit = queueStatService_getServiceStatUnit;

		status = CELIX_SUCCESS;
	}

	return status;

}

celix_status_t queueStatService_destroy(struct service_statistics_service* qStatService) {

	celix_status_t status = CELIX_SUCCESS;

	if (qStatService != NULL) {

		service_statistics_type* qStatHandler = qStatService->serviceStatistics;

		if (qStatHandler != NULL) {

			if (qStatHandler->service_stats_name != NULL) {
				free(qStatHandler->service_stats_name);
				qStatHandler->service_stats_name = NULL;
			}

			if (qStatHandler->service_stats_type != NULL) {
				free(qStatHandler->service_stats_type);
				qStatHandler->service_stats_type = NULL;
			}

			if (qStatHandler->service_stats_measurementUnit != NULL) {
				free(qStatHandler->service_stats_measurementUnit);
				qStatHandler->service_stats_measurementUnit = NULL;
			}

			qStatHandler->qService = NULL;

		}

		free(qStatService->serviceStatistics);
		qStatService->serviceStatistics = NULL;

		free(qStatService);

	}

	return status;

}

int queueStatService_getServiceStatName(service_statistics_type *serviceStatistics, char **name) {

	celix_status_t status = CELIX_SUCCESS;

	if (serviceStatistics->service_stats_name != NULL) {
		*name = strdup(serviceStatistics->service_stats_name);
		//msg(0, "SAMPLE_QUEUE_STAT: Got getName request, answer is %s",*name);
	}
	else {
		msg(0, "SAMPLE_QUEUE_STAT: getName denied because service is removed");
		status = CELIX_ILLEGAL_STATE;
	}

	return (int) status;

}

int queueStatService_getServiceStatType(service_statistics_type *serviceStatistics, char **type) {

	celix_status_t status = CELIX_SUCCESS;

	if (serviceStatistics->service_stats_type != NULL) {
		*type = strdup(serviceStatistics->service_stats_type);
		//msg(0, "SAMPLE_QUEUE_STAT: Got getType request, answer is %s",*type);
	}
	else {
		msg(0, "SAMPLE_QUEUE_STAT: getType denied because service is removed");
		status = CELIX_ILLEGAL_STATE;
	}

	return (int) status;

}

int queueStatService_getServiceStatUnit(service_statistics_type *serviceStatistics, char **mUnit) {
	celix_status_t status = CELIX_SUCCESS;

	if (serviceStatistics->service_stats_measurementUnit != NULL) {
		*mUnit = strdup(serviceStatistics->service_stats_measurementUnit);
		//msg(0, "SAMPLE_QUEUE_STAT: Got getMUnit request, answer is %s",*mUnit);
	}
	else {
		msg(0, "SAMPLE_QUEUE_STAT: getStatUnit denied because service is removed");
		status = CELIX_ILLEGAL_STATE;
	}

	return (int) status;

}

int queueStatService_getServiceStatVal(service_statistics_type *serviceStatistics, double* statVal) {

	celix_status_t status = CELIX_SUCCESS;

	if (serviceStatistics->qService != NULL && serviceStatistics->qService != NULL) {
		status = serviceStatistics->qService->getLoad(serviceStatistics->qService->sampleQueue, statVal);
		//msg(0, "SAMPLE_QUEUE_STAT: Got getStatistic request, answer is %f",*statVal);
	}
	else {
		msg(0, "SAMPLE_QUEUE_STAT: getStatVal denied because service is removed");
		status = CELIX_ILLEGAL_STATE;
	}

	return (int) status;

}

