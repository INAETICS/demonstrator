/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.clusterinfo.stub;

import static java.util.Collections.singletonList;

import java.io.IOException;
import java.lang.management.ManagementFactory;
import java.lang.management.OperatingSystemMXBean;
import java.util.Collection;

import javax.management.MBeanServerConnection;

import org.inaetics.demonstrator.api.clusterinfo.ClusterInfoProvider;
import org.inaetics.demonstrator.api.clusterinfo.ContainerInfo;
import org.inaetics.demonstrator.api.clusterinfo.NodeInfo;

/**
 * 
 */
public class StubClusterInfoProvider implements ClusterInfoProvider {
    private static final String MXBEAN_NAME = ManagementFactory.OPERATING_SYSTEM_MXBEAN_NAME;

    private volatile OperatingSystemMXBean m_osBean;

    @Override
    public Collection<NodeInfo> getClusterInfo() {
        double cpuUsage = m_osBean.getSystemLoadAverage();
        double avgLoad = m_osBean.getSystemLoadAverage();

        Runtime runtime = Runtime.getRuntime();
        long usedMemory = runtime.totalMemory() - runtime.freeMemory();
        long hotMemory = 0;

        ContainerInfo conInfo = new ContainerInfo("default", cpuUsage, avgLoad, usedMemory, hotMemory);
        return singletonList(new NodeInfo("127.0.0.1", singletonList(conInfo)));
    }

    // Called by Felix DM.
    protected final void start() throws IOException {
        MBeanServerConnection mbsc = ManagementFactory.getPlatformMBeanServer();

        m_osBean = ManagementFactory.newPlatformMXBeanProxy(mbsc, MXBEAN_NAME, OperatingSystemMXBean.class);
    }

    protected final void stop() {
        m_osBean = null;
    }
}
