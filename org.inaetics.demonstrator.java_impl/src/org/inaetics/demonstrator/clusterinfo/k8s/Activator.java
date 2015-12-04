/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.clusterinfo.k8s;

import java.util.Properties;

import org.amdatu.kubernetes.Kubernetes;
import org.apache.felix.dm.DependencyActivatorBase;
import org.apache.felix.dm.DependencyManager;
import org.inaetics.demonstrator.api.clusterinfo.ClusterInfoProvider;
import org.osgi.framework.BundleContext;
import org.osgi.service.log.LogService;
import org.osgi.service.remoteserviceadmin.RemoteConstants;

public class Activator extends DependencyActivatorBase {
    public static final String PID = "org.inaetics.demonstrator.clusterinfo";
    
    @Override
    public void init(BundleContext context, DependencyManager manager) throws Exception {
        String iface = ClusterInfoProvider.class.getName();

        Properties props = new Properties();
        props.put(RemoteConstants.SERVICE_EXPORTED_INTERFACES, iface);
        props.put("type", "memory");

        manager.add(createComponent()
            .setInterface(iface, props)
            .setImplementation(K8sClusterInfoProvider.class)
            .add(createConfigurationDependency().setPid(PID))
            .add(createServiceDependency().setService(Kubernetes.class).setRequired(true))
            .add(createServiceDependency().setService(LogService.class).setRequired(false)));
    }
}
