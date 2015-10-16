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

public class ClusterInfoImpl implements ClusterInfo {

	private static final int DEFAULT_UNITS_UPDATE_PERIOD = 30;
	private static final int DEFAULT_QUERIER_UPDATE_PERIOD = 10;

	private Set<FleetUnitInfo> m_fleetUnits;
	private TimerTask m_fleetUnitsQuerier = null;
	private final Timer timer;
	private final int m_unitsUpdatePeriod;
	private final int m_querierUpdatePeriod;

	public ClusterInfoImpl() {
		m_fleetUnits = new CopyOnWriteArraySet<FleetUnitInfo>();
		m_fleetUnitsQuerier = new FleetUnitsQuerier(m_fleetUnits, DEFAULT_UNITS_UPDATE_PERIOD);
		m_unitsUpdatePeriod = DEFAULT_UNITS_UPDATE_PERIOD;
		m_querierUpdatePeriod = DEFAULT_QUERIER_UPDATE_PERIOD;
		timer = new Timer(true);
	}

	public void start() {
		timer.scheduleAtFixedRate(m_fleetUnitsQuerier, 0, m_querierUpdatePeriod * 1000);
	}

	public void stop() {
		timer.cancel();
	}

	@Override
	public List<FleetUnitInfo> getClusterInfo() {
		List<FleetUnitInfo> cl_info = new ArrayList<FleetUnitInfo>();

		for (FleetUnitInfo f_info : m_fleetUnits) {
			DockerInfoUpdater.updateDockerContainerInfo(f_info);
			cl_info.add(f_info);
		}

		cl_info.sort(null);

		return cl_info;
	}

}
