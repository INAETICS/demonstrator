/**
 * Licensed under Apache License v2. See LICENSE for more information.
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
