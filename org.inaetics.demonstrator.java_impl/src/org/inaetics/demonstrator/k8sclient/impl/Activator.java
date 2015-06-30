/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.k8sclient.impl;

import java.util.Dictionary;
import java.util.Properties;

import org.apache.felix.dm.Component;
import org.apache.felix.dm.DependencyActivatorBase;
import org.apache.felix.dm.DependencyManager;
import org.inaetics.demonstrator.k8sclient.KubernetesClient;
import org.osgi.framework.BundleContext;
import org.osgi.framework.Constants;
import org.osgi.service.cm.ConfigurationException;
import org.osgi.service.cm.ManagedService;
import org.osgi.service.log.LogService;

public class Activator extends DependencyActivatorBase implements ManagedService {

	public static final String CONFIG_PID = "org.inaetics.demonstrator.kubernetesclient";

    private volatile BundleContext m_context;
    private volatile DependencyManager m_manager;

    private volatile KubernetesConfig m_config;

	private volatile Component m_k8sclient;
	
    @Override
	public void init(BundleContext context, DependencyManager manager) throws Exception {

        m_context = context;
        m_manager = manager;
        
        boolean configOk = true;
        try {
        	m_config = new KubernetesConfig(context, null);
        }
        catch(ConfigurationException e) {
        	configOk = false;
        }
        
        if (configOk) {
        	registerK8sClient();
        }

        registerConfigurationService();
        
	}
	
	@Override
	public void updated(Dictionary<String, ?> properties)
			throws ConfigurationException {

		unregisterK8sClient();

		m_config = new KubernetesConfig(m_context, properties);
		
		registerK8sClient();
		
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

	private void registerK8sClient() {
		String[] ifaces = { KubernetesClient.class.getName() };

		Properties props = new Properties();

		KubernetesClient k8sClient = new KubernetesClientImpl(m_config);
		
		Component coordinator = createComponent()
			.setInterface(ifaces, props)
			.setImplementation(k8sClient)
			.add(createServiceDependency().setService(LogService.class).setRequired(false));
		
		m_k8sclient = coordinator;
		m_manager.add(m_k8sclient);
		
	}
	
    private void unregisterK8sClient() {
        Component component = m_k8sclient;
        m_k8sclient = null;
        if (component != null) {
            m_manager.remove(component);
        }
    }
}
