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
	private final long processingTime; // milliseconds since epoch time
	private final double result1;
	private final Sample sample;

	public Result(long processingTime, double result1, Sample sample) {
		this.processingTime = processingTime;
		this.result1 = result1;
		this.sample = sample;
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj) {
			return true;
		}
		if (obj == null || getClass() != obj.getClass()) {
			return false;
		}

		Result other = (Result) obj;
		if (processingTime != other.processingTime) {
			return false;
		}
		if (Double.doubleToLongBits(result1) != Double.doubleToLongBits(other.result1)) {
			return false;
		}
		if (sample == null) {
			if (other.sample != null) {
				return false;
			}
		} else if (!sample.equals(other.sample)) {
			return false;
		}
		return true;
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

	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + (int) (processingTime ^ (processingTime >>> 32));
		long temp;
		temp = Double.doubleToLongBits(result1);
		result = prime * result + (int) (temp ^ (temp >>> 32));
		result = prime * result + ((sample == null) ? 0 : sample.hashCode());
		return result;
	}
	
	@Override
	public String toString() {
		return String.format("Result[%d, %s => %.3f]", processingTime, sample, result1);
	}
}
