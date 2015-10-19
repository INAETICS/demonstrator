/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.api.clusterinfo;

import java.util.ArrayList;
import java.util.List;

public class ClusterNodeInfo implements Comparable<ClusterNodeInfo> {

	private final String m_ipAddress;
	private List<DockerContainerInfo> m_containerList;

	public ClusterNodeInfo(String ip) {
		m_ipAddress = ip;
		m_containerList = new ArrayList<DockerContainerInfo>();
	}

	public String getIpAddress() {
		return m_ipAddress;
	}

	public List<DockerContainerInfo> getContainerList() {
		return m_containerList;
	}

	@Override
	public int compareTo(ClusterNodeInfo f) {
		return this.m_ipAddress.compareTo(f.getIpAddress());
	}

	@Override
	public boolean equals(Object o) {

		if (o == null)
			return false;
		if (o == this)
			return true;
		if (!(o instanceof ClusterNodeInfo))
			return false;
		ClusterNodeInfo f = (ClusterNodeInfo) o;

		return (this.m_ipAddress.equals(f.getIpAddress()));

	}

	@Override
	public int hashCode() {
		return m_ipAddress.hashCode();
	}

	@Override
	public String toString() {
		return "[ " + m_ipAddress + " -> " + m_containerList + " ] ";
	}

}
