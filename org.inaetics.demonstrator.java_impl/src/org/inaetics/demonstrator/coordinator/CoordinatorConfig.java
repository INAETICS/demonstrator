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

	private static String KEY_QUEUE_LOW_BARRIER = CONFIG_PID + ".queue_low_barrier";
	private static String KEY_QUEUE_HIGH_BARRIER = CONFIG_PID + ".queue_high_barrier";
	private static String KEY_FELIX_PROCESSOR_CONTROLLER_NAME = CONFIG_PID + ".felix_processor_controller_name";
	private static String KEY_CELIX_PROCESSOR_CONTROLLER_NAME = CONFIG_PID + ".celix_processor_controller_name";
	private static String KEY_POLL_INTERVAL = CONFIG_PID + ".poll_interval";
	private static String KEY_MAX_NR_PROCESSORS = CONFIG_PID + ".max_number_processors";
	
	private int m_queueLowBarrier;
	private int m_queueHighBarrier;
	private String m_felixProcessorControllerName;
	private String m_celixProcessorControllerName;
	private int m_pollInterval;
	private int m_maxNrProcessors;
	
    public CoordinatorConfig(BundleContext context, Dictionary<String, ?> properties) throws ConfigurationException {
    	m_queueLowBarrier = ConfigUtils.getConfigIntValue(context, properties, KEY_QUEUE_LOW_BARRIER);
    	m_queueHighBarrier = ConfigUtils.getConfigIntValue(context, properties, KEY_QUEUE_HIGH_BARRIER);
    	m_felixProcessorControllerName = ConfigUtils.getConfigStringValue(context, properties, KEY_FELIX_PROCESSOR_CONTROLLER_NAME);
    	m_celixProcessorControllerName = ConfigUtils.getConfigStringValue(context, properties, KEY_CELIX_PROCESSOR_CONTROLLER_NAME);
    	m_pollInterval = ConfigUtils.getConfigIntValue(context, properties, KEY_POLL_INTERVAL);
    	m_maxNrProcessors = ConfigUtils.getConfigIntValue(context, properties, KEY_MAX_NR_PROCESSORS);
    }

	public int getQueueLowBarrier() {
		return m_queueLowBarrier;
	}

	public int getQueueHighBarrier() {
		return m_queueHighBarrier;
	}

	public String getFelixProcessorControllerName() {
		return m_felixProcessorControllerName;
	}
	
	public String getCelixProcessorControllerName() {
		return m_celixProcessorControllerName;
	}

	public int getPollInterval() {
		return m_pollInterval;
	}
	
	public int getMaxNrProcessors() {
		return m_maxNrProcessors;
	}

	@Override
	public String toString() {
		return "CoordinatorConfig [m_queueLowBarrier=" + m_queueLowBarrier
				+ ", m_queueHighBarrier=" + m_queueHighBarrier
				+ ", m_felixProcessorControllerName=" + m_felixProcessorControllerName
				+ ", m_celixProcessorControllerName=" + m_celixProcessorControllerName
				+ ", m_pollInterval=" + m_pollInterval + ", m_maxNrProcessors="
				+ m_maxNrProcessors + "]";
	}
	
	
}