/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.api.data;

public class Result {
	private long processingTime; // milliseconds since epoch time
	private double result1;
	private Sample sample;
	
	/**
	 * Constructor needed for JSON (de)serialization. 
	 */
	public Result() {
		// Nop
	}

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
	
	public void setProcessingTime(long processingTime) {
		this.processingTime = processingTime;
	}
	
	public void setResult1(double result1) {
		this.result1 = result1;
	}
	
	public void setSample(Sample sample) {
		this.sample = sample;
	}

	@Override
	public String toString() {
		return String.format("Result[%d, %s => %.3f]", processingTime, sample, result1);
	}
}
