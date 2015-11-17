/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.coordinator.autoscaler;

import java.util.Properties;

import org.apache.felix.dm.DependencyActivatorBase;
import org.apache.felix.dm.DependencyManager;
import org.inaetics.demonstrator.api.coordinator.CoordinatorService;
import org.inaetics.demonstrator.api.stats.StatsProvider;
import org.osgi.framework.BundleContext;
import org.osgi.framework.Constants;
import org.osgi.service.cm.ManagedService;
import org.osgi.service.log.LogService;

public class Activator extends DependencyActivatorBase {
    private static final String PID = "org.inaetics.demonstrator.coordinator.autoscaler.simple";

    @Override
    public void init(BundleContext context, DependencyManager manager) throws Exception {
        Properties props = new Properties();
        props.put(Constants.SERVICE_PID, PID);

        manager.add(createComponent()
        	.setInterface(ManagedService.class.getName(), props)
            .setImplementation(AutoScalerImpl.class)
            .add(createServiceDependency().setService(CoordinatorService.class).setRequired(true))
            .add(createServiceDependency().setService(StatsProvider.class, "(type=queue)").setRequired(true))
            .add(createServiceDependency().setService(LogService.class).setRequired(false)));
    }
}
