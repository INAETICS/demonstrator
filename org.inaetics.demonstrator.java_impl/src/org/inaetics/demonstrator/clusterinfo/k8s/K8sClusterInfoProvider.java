/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.clusterinfo.k8s;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Dictionary;
import java.util.List;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;

import org.amdatu.kubernetes.Kubernetes;
import org.inaetics.demonstrator.api.clusterinfo.ClusterInfoProvider;
import org.inaetics.demonstrator.api.clusterinfo.ContainerInfo;
import org.inaetics.demonstrator.api.clusterinfo.NodeInfo;
import org.osgi.service.cm.ConfigurationException;
import org.osgi.service.cm.ManagedService;
import org.osgi.service.log.LogService;

/**
 * Kubernetes specific implementation of a {@link ClusterInfoProvider}.
 */
public class K8sClusterInfoProvider implements ManagedService, ClusterInfoProvider {
    // Managed by Felix DM...
    private volatile LogService m_log;
    private volatile Kubernetes m_kubernetes;
    // Locally managed...
    private volatile ClusterInfoConfig m_config;
    private volatile List<NodeInfo> m_clusterInfo;

    private volatile ExecutorService m_updateExecutor;
    private volatile Future<?> m_future;

    public K8sClusterInfoProvider() {
        m_clusterInfo = new ArrayList<>();
    }

    @Override
    public List<NodeInfo> getClusterInfo() {
        return new ArrayList<>(m_clusterInfo);
    }

    @Override
    public void updated(Dictionary<String, ?> properties) throws ConfigurationException {
        m_config = new ClusterInfoConfig(properties);
    }

    // Called by Felix DM.
    protected final void start() {
        Runnable updateTask = new Runnable() {
            @Override
            public void run() {
                boolean running = true;
                do {
                    // Ensure we've got the same reference while running this loop-instance...
                    ClusterInfoConfig config = m_config;

                    try {
                        TimeUnit.SECONDS.sleep(config.getUpdatePeriod());
                    } catch (InterruptedException e) {
                        Thread.currentThread().interrupt();
                        // Stop gathering information...
                        running = false;
                    }

                    try {
                        m_clusterInfo = gatherClusterInfo(new KubernetesClusterNodeQuerier(m_kubernetes), config.getCadvisorPort());
                    } catch (IOException e) {
                        m_log.log(LogService.LOG_WARNING, "Failed to gather cluster information!", e);
                    }
                } while (running && !Thread.currentThread().isInterrupted());
            }
        };

        m_updateExecutor = Executors.newSingleThreadExecutor();
        m_future = m_updateExecutor.submit(updateTask);
    }

    // Called by Felix DM.
    protected final void stop() {
        if (m_future != null) {
            m_future.cancel(true /* mayInterruptIfRunning */);
            m_future = null;
        }
        m_updateExecutor.shutdownNow();
        m_updateExecutor = null;
    }

    private List<NodeInfo> gatherClusterInfo(ClusterNodeQuerier querier, int cAdvisorPort) throws IOException {
        List<NodeInfo> clusterInfo = new ArrayList<>();

        for (String clusterNodeIP : querier.getClusterNodeIPs()) {
            List<ContainerInfo> containerInfo = DockerHelper.getContainerInfo(clusterNodeIP, cAdvisorPort);
            clusterInfo.add(new NodeInfo(clusterNodeIP, containerInfo));
        }
        clusterInfo.sort(null);

        return clusterInfo;
    }
}
