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
#ifndef INAETICS_DEMONSTRATOR_API__PRODUCER_H_
#define INAETICS_DEMONSTRATOR_API__PRODUCER_H_

#define INAETICS_DEMONSTRATOR_API__PRODUCER_SERVICE_NAME "org.inaetics.demonstrator.api.producer.Producer"

struct producer_service {
	void *producer;

	int (*getMaxSampleRate)(void *producer);
	int (*getSampleRate)(void *producer);
	int (*setSampleRate)(void *producer, int rate);
};

typedef struct producer_service* producer_service_pt;


#endif /* INAETICS_DEMONSTRATOR_API__PRODUCER_H_ */
