/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.clusterinfo;

import java.util.Dictionary;
import java.util.Properties;

import org.amdatu.kubernetes.Kubernetes;
import org.apache.felix.dm.Component;
import org.apache.felix.dm.DependencyActivatorBase;
import org.apache.felix.dm.DependencyManager;
import org.inaetics.demonstrator.api.clusterinfo.ClusterInfo;
import org.osgi.framework.BundleContext;
import org.osgi.framework.Constants;
import org.osgi.service.cm.ConfigurationException;
import org.osgi.service.cm.ManagedService;
import org.osgi.service.log.LogService;
import org.osgi.service.remoteserviceadmin.RemoteConstants;

public class Activator extends DependencyActivatorBase implements ManagedService {

    public static final String PID = "org.inaetics.demonstrator.clusterinfo";
	private DependencyManager m_manager;
	
	private volatile Component m_component;
	
	@Override
	public void init(BundleContext context, DependencyManager manager) throws Exception {
		m_manager = manager;
		
		Properties props = new Properties();
		props.put(Constants.SERVICE_PID, PID);
		manager.add(createComponent()
				.setInterface(ManagedService.class.getName(), props)
				.setImplementation(this)
		);
	}
	
	@Override
	public void updated(Dictionary<String, ?> properties) throws ConfigurationException {
		try {
			unregisterClusterInfo();
			ClusterInfoConfig config = new ClusterInfoConfig(properties);
			registerClusterInfo(config);
		}
		catch (Exception e) {
			throw new ConfigurationException(null, "error reading config", e);
		}		
	}

	private void registerClusterInfo(ClusterInfoConfig config) {
		String iface = ClusterInfo.class.getName();

		Properties props = new Properties();
		props.put(RemoteConstants.SERVICE_EXPORTED_INTERFACES, iface);
		props.put("type", "memory");

		m_component = createComponent()
				.setInterface(iface, props)
				.setImplementation(new ClusterInfoImpl(config))
				.add(createServiceDependency().setService(Kubernetes.class).setRequired(true))
				.add(createServiceDependency().setService(LogService.class).setRequired(false))
		;
		m_manager.add(m_component);
	}
	
	private void unregisterClusterInfo() {
		if (m_component != null) {
			m_manager.remove(m_component);
			m_component = null;
		}
	}

}
