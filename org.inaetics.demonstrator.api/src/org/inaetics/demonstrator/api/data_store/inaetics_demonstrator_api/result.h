/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
#ifndef __INAETICS_DEMONSTRATOR_API__RESULT_H_
#define __INAETICS_DEMONSTRATOR_API__RESULT_H_

#include <stdint.h>
#include "inaetics_demonstrator_api/sample.h"

struct result {
	uint64_t time; //milliseconds since unix epoch
	double value1;
	struct sample sample;
};

#endif
