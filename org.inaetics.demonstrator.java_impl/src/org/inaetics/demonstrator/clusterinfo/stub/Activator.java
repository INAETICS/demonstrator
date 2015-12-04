/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.clusterinfo.stub;

import java.util.Properties;

import org.apache.felix.dm.DependencyActivatorBase;
import org.apache.felix.dm.DependencyManager;
import org.inaetics.demonstrator.api.clusterinfo.ClusterInfoProvider;
import org.osgi.framework.BundleContext;
import org.osgi.service.log.LogService;
import org.osgi.service.remoteserviceadmin.RemoteConstants;

public class Activator extends DependencyActivatorBase {
    @Override
    public void init(BundleContext context, DependencyManager manager) throws Exception {
        String iface = ClusterInfoProvider.class.getName();

        Properties props = new Properties();
        props.put(RemoteConstants.SERVICE_EXPORTED_INTERFACES, iface);
        props.put("type", "memory");

        manager.add(createComponent()
            .setInterface(iface, props)
            .setImplementation(StubClusterInfoProvider.class)
            .add(createServiceDependency().setService(LogService.class).setRequired(false)));
    }
}
