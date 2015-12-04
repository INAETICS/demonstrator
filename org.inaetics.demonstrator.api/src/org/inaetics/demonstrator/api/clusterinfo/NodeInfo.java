/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.api.clusterinfo;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class NodeInfo implements Comparable<NodeInfo> {
	private final String m_ipAddress;
	private final List<ContainerInfo> m_containerList;

	public NodeInfo(String ip) {
	    this(ip, Collections.emptyList());
	}

    public NodeInfo(String ip, List<ContainerInfo> containerInfos) {
        m_ipAddress = ip;
        m_containerList = new ArrayList<ContainerInfo>(containerInfos);
    }

	public String getIpAddress() {
		return m_ipAddress;
	}

	public List<ContainerInfo> getContainerList() {
		return m_containerList;
	}

	@Override
	public int compareTo(NodeInfo f) {
		return m_ipAddress.compareTo(f.getIpAddress());
	}

	@Override
	public boolean equals(Object o) {
	    if (o == this) {
	        return true;
	    }
		if (o == null || !(o instanceof NodeInfo)) {
			return false;
		}

		NodeInfo f = (NodeInfo) o;
		return m_ipAddress.equals(f.getIpAddress());
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
