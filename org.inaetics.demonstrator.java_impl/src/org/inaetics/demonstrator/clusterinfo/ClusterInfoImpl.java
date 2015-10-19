/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.clusterinfo;

import java.util.ArrayList;
import java.util.List;
import java.util.Set;
import java.util.Timer;
import java.util.TimerTask;
import java.util.concurrent.CopyOnWriteArraySet;

import org.inaetics.demonstrator.api.clusterinfo.ClusterInfo;
import org.inaetics.demonstrator.api.clusterinfo.FleetUnitInfo;
import org.osgi.service.log.LogService;

public class ClusterInfoImpl implements ClusterInfo {

	private Set<FleetUnitInfo> m_fleetUnits;
	private TimerTask m_fleetUnitsQuerier = null;
	private final Timer timer;
	private final ClusterInfoConfig m_config;

	private volatile LogService m_log;
	
	public ClusterInfoImpl(ClusterInfoConfig config) {
		m_config = config;
		m_fleetUnits = new CopyOnWriteArraySet<FleetUnitInfo>();
		m_fleetUnitsQuerier = new FleetUnitsQuerier(m_fleetUnits, config, this);
		timer = new Timer(true);
	}

	protected void start() {
		timer.scheduleAtFixedRate(m_fleetUnitsQuerier, 0, m_config.getUpdatePeriod() * 1000);
	}

	protected void stop() {
		timer.cancel();
	}

	@Override
	public List<FleetUnitInfo> getClusterInfo() {
		List<FleetUnitInfo> cl_info = new ArrayList<FleetUnitInfo>();

		for (FleetUnitInfo f_info : m_fleetUnits) {
			DockerInfoUpdater.updateDockerContainerInfo(f_info, m_config);
			cl_info.add(f_info);
		}

		cl_info.sort(null);

		return cl_info;
	}
	
	void log(String msg, Throwable t) {
		m_log.log(LogService.LOG_DEBUG, msg);
	}

	void error(String msg, Throwable t) {
		m_log.log(LogService.LOG_ERROR, msg);
	}
}
