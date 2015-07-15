/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.coordinator;

import io.fabric8.kubernetes.api.model.ReplicationController;

import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

import org.amdatu.kubernetes.Kubernetes;
import org.apache.felix.dm.Component;
import org.inaetics.demonstrator.api.stats.StatsProvider;
import org.osgi.service.log.LogService;

import rx.Observable;
import rx.Observer;

public class SimpleQueueProcessorCoordinator {

	private volatile StatsProvider m_queueStatsProvider;
	private volatile LogService m_log;

	private volatile ScheduledExecutorService m_executor;
	private volatile Future<?> m_pollFuture;

	private volatile Kubernetes m_kubernetesClient;
	private static final String K8S_NAMESPACE = "default";
	
	private volatile CoordinatorConfig m_config;
	
	public SimpleQueueProcessorCoordinator(CoordinatorConfig config) {
		m_config = config;
		m_executor = Executors.newSingleThreadScheduledExecutor();
	}

	protected final void start(Component comp) {
		
		m_log.log(LogService.LOG_INFO, "using config: " + m_config);

		m_pollFuture = m_executor.scheduleAtFixedRate(new Runnable() {
			@Override
			public void run() {
				checkQueue();
			}
		}, 1, m_config.getPollInterval(), TimeUnit.SECONDS);
	}

	protected final void stop(Component comp) {
		try {
			if (m_pollFuture != null) {
				m_pollFuture.cancel(true);
				m_pollFuture = null;
			}
		} finally {
			m_executor.shutdownNow();
			m_executor = null;
		}
	}

	private void checkQueue() {
		if (m_queueStatsProvider != null) {
			double value = m_queueStatsProvider.getValue();
			m_log.log(LogService.LOG_INFO, "queue utilzation: " + value);
			if (value < m_config.getQueueLowBarrier()) {
				m_log.log(LogService.LOG_INFO, "queue under low barrier, decreasing nr of processors");
				decreaseProcessorCount();
			}
			else if (value > m_config.getQueueHighBarrier()) {
				m_log.log(LogService.LOG_INFO, "queue over high barrier, increasing nr of processors");
				increaseProcessorCount();
			}
		}
	}
	
	private void increaseProcessorCount() {
		
		Observer<ReplicationController> rcObserver = new ReplicationControllerObserver() {

			@Override
			public void onCompleted() {
				if (celixReplicas <= felixReplicas) {
					celixReplicas = Math.min(++celixReplicas, m_config.getMaxNrProcessors() / 2);
					m_log.log(LogService.LOG_INFO, "scaling celix processors to " + celixReplicas);
					m_kubernetesClient.scale(K8S_NAMESPACE, m_config.getCelixProcessorControllerName(), celixReplicas).subscribe();
				}
				else {
					felixReplicas = Math.min(++felixReplicas, m_config.getMaxNrProcessors() / 2);
					m_log.log(LogService.LOG_INFO, "scaling felix processors to " + felixReplicas);
					m_kubernetesClient.scale(K8S_NAMESPACE, m_config.getFelixProcessorControllerName(), felixReplicas).subscribe();
				}
			}
			
		};
		
		modifyReplicationControllers(rcObserver);
		
	}

	private void decreaseProcessorCount() {
		
		Observer<ReplicationController> rcObserver = new ReplicationControllerObserver() {

			@Override
			public void onCompleted() {
				m_log.log(LogService.LOG_DEBUG, "onCompleted");
				if (celixReplicas > felixReplicas) {
					celixReplicas = Math.max(--celixReplicas, 1);
					m_log.log(LogService.LOG_INFO, "scaling celix processors to " + celixReplicas);
					m_kubernetesClient.scale(K8S_NAMESPACE, m_config.getCelixProcessorControllerName(), celixReplicas).subscribe();
				}
				else {
					felixReplicas = Math.max(--felixReplicas, 0);
					m_log.log(LogService.LOG_INFO, "scaling felix processors to " + felixReplicas);
					m_kubernetesClient.scale(K8S_NAMESPACE, m_config.getFelixProcessorControllerName(), felixReplicas).subscribe();
				}
			}
			
		};
		
		modifyReplicationControllers(rcObserver);
		
	}

	private void modifyReplicationControllers(Observer<ReplicationController> replicationControllerObserver) {
		
		Observable<ReplicationController> celixObservable = m_kubernetesClient.getReplicationController(
				K8S_NAMESPACE, m_config.getCelixProcessorControllerName());
		Observable<ReplicationController> felixObservable = m_kubernetesClient.getReplicationController(
				K8S_NAMESPACE, m_config.getFelixProcessorControllerName());
		
		Observable<ReplicationController> rcObservable = celixObservable.concatWith(felixObservable);
		rcObservable.subscribe(replicationControllerObserver);
		
	}
	
	abstract class ReplicationControllerObserver implements Observer<ReplicationController> {

		protected Integer celixReplicas = null;
		protected Integer felixReplicas = null;
		
		@Override
		public void onError(Throwable e) {
			m_log.log(LogService.LOG_ERROR, "error updating replication controllers", e);
		}

		@Override
		public void onNext(ReplicationController rc) {
			//m_log.log(LogService.LOG_DEBUG, "onNext:" + rc);
			Integer replicas = rc.getStatus().getReplicas();
			if (rc.getMetadata().getName().equals(m_config.getCelixProcessorControllerName())) {
				celixReplicas = replicas;
			}
			else if (rc.getMetadata().getName().equals(m_config.getFelixProcessorControllerName())) {
				felixReplicas = replicas;
			}
		}

	}
	
}
