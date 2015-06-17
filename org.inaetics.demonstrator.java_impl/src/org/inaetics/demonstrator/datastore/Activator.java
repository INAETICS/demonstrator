/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.datastore;

import java.util.Properties;

import org.apache.felix.dm.DependencyActivatorBase;
import org.apache.felix.dm.DependencyManager;
import org.inaetics.demonstrator.api.datastore.DataStore;
import org.inaetics.demonstrator.api.stats.StatsProvider;
import org.osgi.framework.BundleContext;
import org.osgi.framework.Constants;
import org.osgi.service.cm.ManagedService;
import org.osgi.service.log.LogService;
import org.osgi.service.remoteserviceadmin.RemoteConstants;

public class Activator extends DependencyActivatorBase {
	private static final String PID = "InMemoryDataStore";

	@Override
	public void init(BundleContext context, DependencyManager manager) throws Exception {
		String[] ifaces = { DataStore.class.getName(), ManagedService.class.getName() };
		// See comment below...
		InMemoryDataStore service = new InMemoryDataStore();

		Properties props = new Properties();
		props.put(Constants.SERVICE_PID, PID);
		props.put(RemoteConstants.SERVICE_EXPORTED_INTERFACES, ifaces[0]);
        props.put("type", "memory");

		manager.add(createComponent()
			.setInterface(ifaces, props)
			.setImplementation(service)
			.add(createServiceDependency().setService(LogService.class).setRequired(false))
		);

        /*
         * IMPORTANT: We do not want a direct coupling between the DataStore and StatsProvider,
         * hence we create a singleton service that is registered as two *different* services.
         */
		props = new Properties();
        props.put(RemoteConstants.SERVICE_EXPORTED_INTERFACES, StatsProvider.class.getName());
        props.put("type", "datastore");

        manager.add(createComponent()
            .setInterface(StatsProvider.class.getName(), props)
            .setImplementation(service)
            .setCallbacks(null, null, null, null) // init/start/stop/destroy already called for the 1st component...
        );
	}
}
