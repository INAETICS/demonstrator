/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.logsender;

import java.util.Dictionary;
import java.util.Hashtable;

import org.apache.felix.dm.DependencyActivatorBase;
import org.apache.felix.dm.DependencyManager;
import org.osgi.framework.BundleContext;
import org.osgi.framework.Constants;
import org.osgi.service.cm.ManagedService;
import org.osgi.service.log.LogReaderService;

public class Activator extends DependencyActivatorBase {
	
	@Override
	public void init(BundleContext context, DependencyManager manager) throws Exception {
		
		Dictionary<String, Object> properties = new Hashtable<>();
		properties.put(Constants.SERVICE_PID, LogsToTCP.PID);
		
		manager.add(createComponent()
			.setInterface(ManagedService.class.getName(), properties)
			.setImplementation(LogsToTCP.class)
			.add(createServiceDependency().setService(LogReaderService.class).setRequired(true))
		);
	}
}
