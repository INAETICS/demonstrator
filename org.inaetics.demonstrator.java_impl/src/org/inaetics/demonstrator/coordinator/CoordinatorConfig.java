/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.coordinator;

import static org.inaetics.demonstrator.coordinator.Activator.CONFIG_PID;

import java.util.Dictionary;

import org.osgi.framework.BundleContext;
import org.osgi.service.cm.ConfigurationException;

/**
 * Configuration class for the coordinator
 */
public class CoordinatorConfig {

	private static String KEY_K8S_MASTER_URI = CONFIG_PID + ".kubernetes_master_url";
	private static String KEY_QUEUE_LOW_BARRIER = CONFIG_PID + ".queue_low_barrier";
	private static String KEY_QUEUE_HIGH_BARRIER = CONFIG_PID + ".queue_high_barrier";
	private static String KEY_PROCESSOR_CONTROLLER_NAME = CONFIG_PID + ".processor_controller_name";
	private static String KEY_POLL_INTERVAL = CONFIG_PID + ".poll_interval";
	private static String KEY_MAX_NR_PROCESSORS = CONFIG_PID + ".max_number_processors";
	
	private String m_k8sMasterUrl;
	private int m_queueLowBarrier;
	private int m_queueHighBarrier;
	private String m_processorControllerName;
	private int m_pollInterval;
	private int m_maxNrProcessors;
	
    public CoordinatorConfig(BundleContext context, Dictionary<String, ?> properties) throws ConfigurationException {
    	m_k8sMasterUrl = ConfigUtils.getConfigStringValue(context, properties, KEY_K8S_MASTER_URI);
    	m_queueLowBarrier = ConfigUtils.getConfigIntValue(context, properties, KEY_QUEUE_LOW_BARRIER);
    	m_queueHighBarrier = ConfigUtils.getConfigIntValue(context, properties, KEY_QUEUE_HIGH_BARRIER);
    	m_processorControllerName = ConfigUtils.getConfigStringValue(context, properties, KEY_PROCESSOR_CONTROLLER_NAME);
    	m_pollInterval = ConfigUtils.getConfigIntValue(context, properties, KEY_POLL_INTERVAL);
    	m_maxNrProcessors = ConfigUtils.getConfigIntValue(context, properties, KEY_MAX_NR_PROCESSORS);
    }

	public String getK8sMasterUrl() {
		return m_k8sMasterUrl;
	}

	public int getQueueLowBarrier() {
		return m_queueLowBarrier;
	}

	public int getQueueHighBarrier() {
		return m_queueHighBarrier;
	}

	public String getProcessorControllerName() {
		return m_processorControllerName;
	}
	
	public int getPollInterval() {
		return m_pollInterval;
	}
	
	public int getMaxNrProcessors() {
		return m_maxNrProcessors;
	}

	@Override
	public String toString() {
		return "CoordinatorConfig [m_k8sMasterUrl=" + m_k8sMasterUrl
				+ ", m_queueLowBarrier=" + m_queueLowBarrier
				+ ", m_queueHighBarrier=" + m_queueHighBarrier
				+ ", m_processorControllerName=" + m_processorControllerName
				+ ", m_pollInterval=" + m_pollInterval + ", m_maxNrProcessors="
				+ m_maxNrProcessors + "]";
	}
	
	
}