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
package org.inaetics.demonstrator.api.data;

public class Result {

	private final long processingTime; //milliseconds since epoch time
	private final double result1;
	private final Sample sample;
	
	public Result(long processingTime, double result1, Sample sample) {
		super();
		this.processingTime = processingTime;
		this.result1 = result1;
		this.sample = sample;
	}

	public long getProcessingTime() {
		return processingTime;
	}

	public double getResult1() {
		return result1;
	}

	public Sample getSample() {
		return sample;
	}
}
