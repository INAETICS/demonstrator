/*
 * Copyright (c) 2015 INAETICS, <www.inaetics.org>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
