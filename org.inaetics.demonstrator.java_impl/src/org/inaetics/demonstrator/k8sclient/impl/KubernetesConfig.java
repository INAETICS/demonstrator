/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.k8sclient.impl;

import static org.inaetics.demonstrator.k8sclient.impl.Activator.CONFIG_PID;

import java.util.Dictionary;

import org.osgi.framework.BundleContext;
import org.osgi.service.cm.ConfigurationException;

/**
 * Configuration class for the coordinator
 */
public class KubernetesConfig {

	public static String KEY_K8S_MASTER_URI = CONFIG_PID + ".master_url";
	
	private String m_k8sMasterUrl;
	
    public KubernetesConfig(BundleContext context, Dictionary<String, ?> properties) throws ConfigurationException {
    	m_k8sMasterUrl = ConfigUtils.getConfigStringValue(context, properties, KEY_K8S_MASTER_URI);
    }

	public String getK8sMasterUrl() {
		return m_k8sMasterUrl;
	}

	@Override
	public String toString() {
		return "CoordinatorConfig [m_k8sMasterUrl=" + m_k8sMasterUrl + "]";
	}
	
}