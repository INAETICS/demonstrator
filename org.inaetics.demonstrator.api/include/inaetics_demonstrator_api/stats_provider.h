/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
#ifndef INAETICS_DEMONSTRATOR_API__STATS_PROVIDER_H_
#define INAETICS_DEMONSTRATOR_API__STATS_PROVIDER_H_

#define INAETICS_DEMONSTRATOR_API__STATS_PROVIDER_SERVICE_NAME "org.inaetics.demonstrator.api.stats.StatsProvider"

struct stats_provider_service {
	void *statsProvider;

	int (*getName)(void *statsProvider, char **name);
	int (*getType)(void *statsProvider, char **type);
	int (*getMeasurementUnit)(void *statsProvider, char **measurementUnit);
	int (*getValue)(void *statsProvider, double *value);

};


#endif /* INAETICS_DEMONSTRATOR_API__STATS_PROVIDER_H_ */
