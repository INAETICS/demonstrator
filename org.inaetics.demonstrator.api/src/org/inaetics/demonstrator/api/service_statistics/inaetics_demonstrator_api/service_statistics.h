/*
 * Copyright (c) 2014 INAETICS, <www.inaetics.org>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef __INAETICS_DEMONSTRATOR_API__SERVICE_STATISTICS_H_
#define __INAETICS_DEMONSTRATOR_API__SERVICE_STATISTICS_H_

#include <stdint.h>

#define INAETICS_DEMONSTRATOR_API_SERVICE_STATISTICS_SERVICE_NAME "org.inaetics.demonstrator.api.servstat.ServiceStatistics"

typedef struct service_statistics service_statistics_type; //ADT

struct service_statistics_service {
	
    service_statistics_type *serviceStatistics;

	int (*getServiceName)(service_statistics_type *serviceStatistics, char **name);      //Pretty print unique name of the monitored service, e.g. "Celix Based Queue (serviceid)"
	
    int (*getServiceType)(service_statistics_type *serviceStatistics, char **type);      //Type of the monitored service, e.g. queue or processor
	
    int (*getStatistic)(service_statistics_type *serviceStatistics, double *statVal);  //Actual value for the monitored statistic, e.g. queue precentage of total size for queue/datastore or throughput for processor/producer
	
    int (*getMeasurementUnit)(service_statistics_type *serviceStatistics, char **mUnit); //Measurement unit for the monitored statistic, e.g. % for queue size or sample/sec for processor/producer

};

#endif
