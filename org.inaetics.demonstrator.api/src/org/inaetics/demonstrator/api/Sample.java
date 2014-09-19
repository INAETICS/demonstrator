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
package org.inaetics.demonstrator.api;

public class Sample {

	private final long sampleTime; //Milliseconds since epoch time
	private final double value1;
	private final double value2;
	
	public Sample(long sampleTime, double value1, double value2) {
		super();
		this.sampleTime = sampleTime;
		this.value1 = value1;
		this.value2 = value2;
	}
	
	public long getSampleTime() {
		return sampleTime;
	}

	public double getValue1() {
		return value1;
	}

	public double getValue2() {
		return value2;
	}
}
