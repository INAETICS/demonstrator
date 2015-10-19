/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.clusterinfo;

import java.util.Properties;

import org.amdatu.kubernetes.Kubernetes;
import org.apache.felix.dm.DependencyActivatorBase;
import org.apache.felix.dm.DependencyManager;
import org.inaetics.demonstrator.api.clusterinfo.ClusterInfo;
import org.osgi.framework.BundleContext;
import org.osgi.service.log.LogService;
import org.osgi.service.remoteserviceadmin.RemoteConstants;

public class Activator extends DependencyActivatorBase{

	/*
	 * Needed only in case of ManagedService
	 * private static final String PID = "ClusterInfo";
	 */
	private ClusterInfoImpl service = null; 

	@Override
	public void init(BundleContext context, DependencyManager manager) throws Exception {
		String iface = ClusterInfo.class.getName();
		
		ClusterInfoConfig config = new ClusterInfoConfig();
		service = new ClusterInfoImpl(config);

		Properties props = new Properties();
		//props.put(Constants.SERVICE_PID, PID);
		props.put(RemoteConstants.SERVICE_EXPORTED_INTERFACES, iface);
		props.put("type", "memory");

		manager.add(createComponent()
				.setInterface(iface, props)
				.setImplementation(service)
				.add(createServiceDependency().setService(Kubernetes.class).setRequired(true))
				.add(createServiceDependency().setService(LogService.class).setRequired(false))
				);
	}

}
