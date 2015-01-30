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

public class Sample {

	private long sampleTime; // Milliseconds since epoch time
	private double value1;
	private double value2;

	/**
	 * Constructor needed for JSON (de)serialization.
	 */
	public Sample() {
		// Nop
	}

	public Sample(long sampleTime, double value1, double value2) {
		this.sampleTime = sampleTime;
		this.value1 = value1;
		this.value2 = value2;
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj) {
			return true;
		}
		if (obj == null || getClass() != obj.getClass())
			return false;

		Sample other = (Sample) obj;
		if (sampleTime != other.sampleTime) {
			return false;
		}
		if (Double.doubleToLongBits(value1) != Double.doubleToLongBits(other.value1)) {
			return false;
		}
		if (Double.doubleToLongBits(value2) != Double.doubleToLongBits(other.value2)) {
			return false;
		}
		return true;
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

	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + (int) (sampleTime ^ (sampleTime >>> 32));
		long temp;
		temp = Double.doubleToLongBits(value1);
		result = prime * result + (int) (temp ^ (temp >>> 32));
		temp = Double.doubleToLongBits(value2);
		result = prime * result + (int) (temp ^ (temp >>> 32));
		return result;
	}

	public void setSampleTime(long sampleTime) {
		this.sampleTime = sampleTime;
	}

	public void setValue1(double value1) {
		this.value1 = value1;
	}

	public void setValue2(double value2) {
		this.value2 = value2;
	}

	@Override
	public String toString() {
		return String.format("Sample[%d, <%.3f;%.3f>]", sampleTime, value1, value2);
	}
}
