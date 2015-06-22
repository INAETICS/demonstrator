/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.coordinator;

import java.util.Dictionary;
import java.util.Properties;

import org.apache.felix.dm.Component;
import org.apache.felix.dm.DependencyActivatorBase;
import org.apache.felix.dm.DependencyManager;
import org.inaetics.demonstrator.api.stats.StatsProvider;
import org.osgi.framework.BundleContext;
import org.osgi.framework.Constants;
import org.osgi.service.cm.ConfigurationException;
import org.osgi.service.cm.ManagedService;
import org.osgi.service.log.LogService;

public class Activator extends DependencyActivatorBase implements ManagedService {

	public static final String CONFIG_PID = "org.inaetics.demonstrator.coordinator";

    private volatile BundleContext m_context;
    private volatile DependencyManager m_manager;

    private volatile CoordinatorConfig m_config;

	private volatile Component m_coordinator;
	
    @Override
	public void init(BundleContext context, DependencyManager manager) throws Exception {

        m_context = context;
        m_manager = manager;
        
        boolean configOk = true;
        try {
        	m_config = new CoordinatorConfig(context, null);
        }
        catch(ConfigurationException e) {
        	configOk = false;
        }
        
        if (configOk) {
        	registerCoordinator();
        }

        registerConfigurationService();
        
	}
	
	@Override
	public void updated(Dictionary<String, ?> properties)
			throws ConfigurationException {

		unregisterCoordinator();

		m_config = new CoordinatorConfig(m_context, properties);
		
		registerCoordinator();
		
	}

	private void registerConfigurationService() {
		String[] ifaces = { ManagedService.class.getName() };

		Properties props = new Properties();
		props.put(Constants.SERVICE_PID, CONFIG_PID);

		m_manager.add(createComponent()
			.setInterface(ifaces, props)
			.setImplementation(this)
            .setAutoConfig(BundleContext.class, false)
            .setAutoConfig(DependencyManager.class, false)
            .setAutoConfig(Component.class, false)
		);
	}

	private void registerCoordinator() {
		String[] ifaces = { Object.class.getName() };

		Properties props = new Properties();

		SimpleQueueProcessorCoordinator simpleCoordinator = new SimpleQueueProcessorCoordinator(m_config);
		
		Component coordinator = createComponent()
			.setInterface(ifaces, props)
			.setImplementation(simpleCoordinator)
            .add(createServiceDependency().setService(StatsProvider.class, "(type=queue)").setRequired(true))
			.add(createServiceDependency().setService(LogService.class).setRequired(false));
		
		m_coordinator = coordinator;
		m_manager.add(m_coordinator);
		
	}
	
    private void unregisterCoordinator() {
        Component component = m_coordinator;
        m_coordinator = null;
        if (component != null) {
            m_manager.remove(component);
        }
    }
}
