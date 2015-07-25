/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.producer.burst;

import java.util.Properties;

import org.apache.felix.dm.DependencyActivatorBase;
import org.apache.felix.dm.DependencyManager;
import org.inaetics.demonstrator.api.producer.Producer;
import org.inaetics.demonstrator.api.queue.SampleQueue;
import org.inaetics.demonstrator.api.stats.StatsProvider;
import org.osgi.framework.BundleContext;
import org.osgi.service.log.LogService;
import org.osgi.service.remoteserviceadmin.RemoteConstants;

public class Activator extends DependencyActivatorBase {
	@Override
	public void init(BundleContext context, DependencyManager manager) throws Exception {
		Properties props = new Properties();
		props.put(RemoteConstants.SERVICE_EXPORTED_INTERFACES, Producer.class.getName());
        props.put("type", "burst");

		Producer service = new BurstSampleProducer();

		manager.add(createComponent()
			.setInterface(Producer.class.getName(), props)
			.setImplementation(service)
			.add(createServiceDependency().setService(SampleQueue.class).setRequired(true))
			.add(createServiceDependency().setService(LogService.class).setRequired(false))
		);

        /*
         * IMPORTANT: We do not want a direct coupling between the DataStore and StatsProvider,
         * hence we create a singleton service that is registered as two *different* services.
         */
        props = new Properties();
        props.put(RemoteConstants.SERVICE_EXPORTED_INTERFACES, StatsProvider.class.getName());
        props.put("type", "producer");

        manager.add(createComponent()
            .setInterface(StatsProvider.class.getName(), props)
            .setImplementation(service)
            .setCallbacks(null, null, null, null) // init/start/stop/destroy already called for the 1st component...
        );
	}
}
