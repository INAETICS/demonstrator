/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.coordinator;

import java.util.Dictionary;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

import org.apache.felix.dm.Component;
import org.inaetics.demonstrator.api.stats.StatsProvider;
import org.inaetics.demonstrator.coordinator.k8s.KubernetesClient;
import org.inaetics.demonstrator.coordinator.k8s.ReplicationController;
import org.osgi.service.cm.ConfigurationException;
import org.osgi.service.cm.ManagedService;
import org.osgi.service.log.LogService;

public class SimpleQueueProcessorCoordinator implements ManagedService {

	private volatile StatsProvider m_queueStatsProvider;
	private volatile LogService m_log;

	private final ScheduledExecutorService m_executor;
	private volatile Future<?> m_pollFuture;

	private static final String K8S_URI = "http://172.17.8.20:10260";
	private KubernetesClient m_kubernetesClient;
	private static final int QUEUE_LOW_BARRIER = 40;
	private static final int QUEUE_HIGH_BARRIER = 60;
	private static final String PROCESSOR_CONTROLLER_NAME = "inaetics-processor-controller";
	
	public SimpleQueueProcessorCoordinator() {
		m_executor = Executors.newSingleThreadScheduledExecutor();
		m_kubernetesClient = new KubernetesClient(K8S_URI);
	}

	@Override
	public void updated(Dictionary<String, ?> properties)
			throws ConfigurationException {
		
		// TODO get k8s url, rc name, min/max count, schedule time etc. from configuration
		
	}

	protected final void start(Component comp) {
		m_pollFuture = m_executor.scheduleAtFixedRate(new Runnable() {
			@Override
			public void run() {
				checkQueue();
			}
		}, 1, 20, TimeUnit.SECONDS);
	
	}

	protected final void stop(Component comp) {
		try {
			if (m_pollFuture != null) {
				m_pollFuture.cancel(true);
				m_pollFuture = null;
			}
		} finally {
			m_executor.shutdownNow();
		}
	}

	private void checkQueue() {
		m_log.log(LogService.LOG_INFO, "polling queue...");
		if (m_queueStatsProvider != null) {
			double value = m_queueStatsProvider.getValue();
			if (value < QUEUE_LOW_BARRIER) {
				m_log.log(LogService.LOG_INFO, "queue under low barrier, decreasing nr of processors");
				decreaseProcessorCount();
			}
			else if (value > QUEUE_HIGH_BARRIER) {
				m_log.log(LogService.LOG_INFO, "queue ove high barrier, increasing nr of processors");
				increaseProcessorCount();
			}
		}
	}
	
	private void increaseProcessorCount() {
		ReplicationController processorController = m_kubernetesClient.getReplicationController(PROCESSOR_CONTROLLER_NAME);
		if (processorController != null) {
			processorController.increaseReplicaCount(5);
			m_kubernetesClient.updateReplicationController(processorController);
		}
	}

	private void decreaseProcessorCount() {
		ReplicationController processorController = m_kubernetesClient.getReplicationController(PROCESSOR_CONTROLLER_NAME);
		if (processorController != null) {
			processorController.decreaseReplicaCount(1);
			m_kubernetesClient.updateReplicationController(processorController);
		}
	}

}
