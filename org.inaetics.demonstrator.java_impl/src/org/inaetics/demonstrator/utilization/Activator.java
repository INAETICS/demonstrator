/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.utilization;

import java.util.Properties;

import org.apache.felix.dm.DependencyActivatorBase;
import org.apache.felix.dm.DependencyManager;
import org.inaetics.demonstrator.api.producer.Producer;
import org.inaetics.demonstrator.api.stats.StatsProvider;
import org.osgi.framework.BundleContext;
import org.osgi.service.log.LogService;
import org.osgi.service.remoteserviceadmin.RemoteConstants;

/**
 * Bundle activator.
 */
public class Activator extends DependencyActivatorBase {

    @Override
    public void init(BundleContext context, DependencyManager dm) throws Exception {
        Properties props;

        /*
         * IMPORTANT: We do not want a direct coupling between the DataStore and StatsProvider,
         * hence we create a singleton service that is registered as two *different* services.
         */
        props = new Properties();
        props.put(RemoteConstants.SERVICE_EXPORTED_INTERFACES, StatsProvider.class.getName());
        props.put("type", "other");

        dm.add(createComponent()
            .setInterface(StatsProvider.class.getName(), props)
            .setImplementation(UtilitizationAggegrator.class)
            .add(createServiceDependency().setService(Producer.class).setCallbacks("addProducer", "removeProducer"))
            .add(createServiceDependency().setService(LogService.class).setRequired(false))
        );
    }
}
