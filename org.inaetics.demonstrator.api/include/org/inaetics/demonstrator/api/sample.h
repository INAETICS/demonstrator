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
#ifndef __ORG_INAETICS_DEMONSTATOR_API__SAMPLE_H_
#define __ORG_INAETICS_DEMONSTATOR_API__SAMPLE_H_

#include <stdint.h>

struct org_inaetics_demonstator_api__sample {
	const uint64_t time; //milliseconds since unix epoch
	const double value1;
	const double value2;
};

#endif
