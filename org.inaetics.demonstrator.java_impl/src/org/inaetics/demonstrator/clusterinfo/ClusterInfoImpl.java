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
import org.inaetics.demonstrator.api.clusterinfo.ClusterNodeInfo;
import org.osgi.service.log.LogService;

public class ClusterInfoImpl implements ClusterInfo {

	private Set<ClusterNodeInfo> m_clusterNodes;
	private ClusterNodeQuerier m_clusterNodeQuerier = null;
	private final Timer timer;
	private final ClusterInfoConfig m_config;

	private volatile LogService m_log;
	
	public ClusterInfoImpl(ClusterInfoConfig config) {
		m_config = config;
		m_clusterNodes = new CopyOnWriteArraySet<ClusterNodeInfo>();
		m_clusterNodeQuerier = new FleetClusterNodeQuerier(config, this);
		timer = new Timer(true);
	}

	protected void start() {
		TimerTask task = new TimerTask() {
			@Override
			public void run() {
				updateClusterNodes();
			}
		};
		timer.scheduleAtFixedRate(task, 0, m_config.getUpdatePeriod() * 1000);
	}

	protected void stop() {
		timer.cancel();
	}
	
	synchronized private void updateClusterNodes() {
		m_clusterNodes = m_clusterNodeQuerier.getClusterNodes();
	}

	@Override
	synchronized public List<ClusterNodeInfo> getClusterInfo() {
		List<ClusterNodeInfo> cl_info = new ArrayList<ClusterNodeInfo>();

		for (ClusterNodeInfo f_info : m_clusterNodes) {
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
