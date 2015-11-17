/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.api.clusterinfo;

import java.math.RoundingMode;
import java.text.DecimalFormat;

public class DockerContainerInfo {

	private final String m_containerName;
	private final double m_cpuUsage;
	private final double m_avgLoad;
	private final long m_usedMem;
	private final long m_hotMem;

	private final static double BYTE_PER_MB = (double) (1024 * 1024);

	public DockerContainerInfo(String containerName, double cpuUsage, double avgLoad, long usedMem, long hotMem) {

		DecimalFormat df = new DecimalFormat("#.####");
		df.setRoundingMode(RoundingMode.CEILING);

		m_containerName = containerName;
		m_cpuUsage = new Double(df.format(cpuUsage));
		m_avgLoad = new Double(df.format(avgLoad));
		m_usedMem = usedMem;
		m_hotMem = hotMem;
	}

	public String getName() {
		return m_containerName;
	}

	public double getCpuUsage() {
		return m_cpuUsage;
	}

	public double getAvgLoad() {
		return m_avgLoad;
	}

	public long getUsedMem() {
		return m_usedMem;
	}

	public long getHotMem() {
		return m_hotMem;
	}

	@Override
	public String toString() {
		String s = m_containerName + ": ";
		s += String.format("%10.5f", m_cpuUsage);
		s += String.format("%10.5f", m_avgLoad);
		s += String.format("%10.1f", (double) (((double) m_usedMem) / BYTE_PER_MB));
		s += String.format("%10.1f", (double) (((double) m_hotMem) / BYTE_PER_MB));

		return s;
	}

}
