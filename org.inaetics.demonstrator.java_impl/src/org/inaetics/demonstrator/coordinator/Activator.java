/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.coordinator;

import java.util.Properties;

import org.apache.felix.dm.DependencyActivatorBase;
import org.apache.felix.dm.DependencyManager;
import org.inaetics.demonstrator.api.stats.StatsProvider;
import org.osgi.framework.BundleContext;
import org.osgi.service.cm.ManagedService;
import org.osgi.service.log.LogService;

public class Activator extends DependencyActivatorBase {
	private static final String PID = "SimpleCoordinator";

	@Override
	public void init(BundleContext context, DependencyManager manager) throws Exception {
		String[] ifaces = {ManagedService.class.getName() };

		Properties props = new Properties();

		manager.add(createComponent()
			.setInterface(ifaces, props)
			.setImplementation(SimpleQueueProcessorCoordinator.class)
            .add(createServiceDependency().setService(StatsProvider.class).setCallbacks("add", "remove").setRequired(false))
			.add(createServiceDependency().setService(LogService.class).setRequired(false))
		);

	}
}
