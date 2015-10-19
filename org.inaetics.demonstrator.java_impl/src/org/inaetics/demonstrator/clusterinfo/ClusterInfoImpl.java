/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.clusterinfo;

import java.util.List;
import java.util.Set;
import java.util.concurrent.CopyOnWriteArrayList;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

import org.amdatu.kubernetes.Kubernetes;
import org.inaetics.demonstrator.api.clusterinfo.ClusterInfo;
import org.inaetics.demonstrator.api.clusterinfo.ClusterNodeInfo;
import org.osgi.service.log.LogService;

public class ClusterInfoImpl implements ClusterInfo {

	private final ClusterInfoConfig m_config;
	private ClusterNodeQuerier m_clusterNodeQuerier = null;
	private List<ClusterNodeInfo> m_clusterInfo;
    private ScheduledExecutorService m_updateExecutor;

	private volatile LogService m_log;
	private volatile Kubernetes m_kubernetes;
	
	public ClusterInfoImpl(ClusterInfoConfig config) {
		m_config = config;
		m_clusterInfo = new CopyOnWriteArrayList<>();
	}

	protected void start() {
		//m_clusterNodeQuerier = new FleetClusterNodeQuerier(m_config, this);
		m_clusterNodeQuerier = new KubernetesClusterNodeQuerier(m_config, this, m_kubernetes);
		m_updateExecutor = Executors.newSingleThreadScheduledExecutor();
		Runnable updateTask = new Runnable() {
			@Override
			public void run() {
				updateClusterInfo();
			}
		};
		m_updateExecutor.scheduleAtFixedRate(updateTask, 0, m_config.getUpdatePeriod(), TimeUnit.SECONDS);
	}

	protected void stop() {
		m_updateExecutor.shutdownNow();
		m_updateExecutor = null;
	}
	
	private void updateClusterInfo() {
		m_clusterInfo.clear();
		
		Set<ClusterNodeInfo> clusterNodes = m_clusterNodeQuerier.getClusterNodes();
		for (ClusterNodeInfo clusterNode : clusterNodes) {
			DockerInfoUpdater.updateDockerContainerInfo(clusterNode, m_config);
			m_clusterInfo.add(clusterNode);
		}

		m_clusterInfo.sort(null);
		
	}
	
	@Override
	public List<ClusterNodeInfo> getClusterInfo() {
		return m_clusterInfo;
	}
	
	void log(String msg, Throwable t) {
		m_log.log(LogService.LOG_DEBUG, msg, t);
	}

	void error(String msg, Throwable t) {
		m_log.log(LogService.LOG_ERROR, msg, t);
	}
}
