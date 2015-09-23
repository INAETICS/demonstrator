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
#ifndef __INAETICS_DEMONSTRATOR_API__RESULT_H_
#define __INAETICS_DEMONSTRATOR_API__RESULT_H_

#include <stdint.h>
#include "inaetics_demonstrator_api/sample.h"

struct result {
	uint64_t processingTime; //milliseconds since unix epoch
	double result1;
	struct sample sample;
};

#endif
