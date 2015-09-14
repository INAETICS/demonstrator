/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.api.stats;

import java.util.ArrayList;
import java.util.List;

public class FleetUnitInfo implements Comparable<FleetUnitInfo>{

	private final static int DEFAULT_UPDATE_PERIOD = 30; /* Stats is an array of 60 JSON objects */

	private final String m_ipAddress;
	private final int m_updatePeriod;
	private List<DockerContainerInfo> m_containerList;
	
	public String getIpAddress() {
		return m_ipAddress;
	}
	
	public int getUpdatePeriod() {
		return m_updatePeriod;
	}

	public List<DockerContainerInfo> getContainerList() {
		return m_containerList;
	}

	public FleetUnitInfo(String ip){
		m_ipAddress=ip;
		m_updatePeriod=DEFAULT_UPDATE_PERIOD;
		m_containerList=new ArrayList<DockerContainerInfo>();
	}
	
	public FleetUnitInfo(String ip, int period){
		m_ipAddress=ip;
		m_updatePeriod=period;
		m_containerList=new ArrayList<DockerContainerInfo>();
	}
		
	@Override
	public int compareTo(FleetUnitInfo f) {
		return this.m_ipAddress.compareTo(f.getIpAddress());
	}
	
	@Override
	public boolean equals(Object o){

		if (o == null) return false;
		if (o == this) return true;
		if (!(o instanceof FleetUnitInfo))return false;
		FleetUnitInfo f = (FleetUnitInfo)o;

		return (this.m_ipAddress.equals(f.getIpAddress()));

	}

	@Override
	public int hashCode(){
		return m_ipAddress.hashCode();
	}

	@Override
	public String toString(){
		return "[ " + m_ipAddress + " -> " + m_containerList + " ] ";
	}

}
