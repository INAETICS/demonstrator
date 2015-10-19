/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.clusterinfo;

import java.util.Dictionary;

/**
 * Configuration values for cluster info
 */
public class ClusterInfoConfig {
	
	private static final String KEY_CADVISOR_PORT = "cAdvisorPort";	
	private static final String KEY_ETCD_ENDPOINT = "etcdEndpoint";	
	private static final String KEY_FLEET_SELECTION_KEY = "fleetSelectionKey";	
	private static final String KEY_FLEET_SELECTION_VALUE = "fleetSelectionValue";	
	private static final String KEY_UPDATE_PERIOD = "updatePeriod";	

	private final static int DEFAULT_CADVISOR_PORT = 4194;
	private final static String DEFAULT_ETCD_ENDPOINT = "172.17.8.20:2379";
    private final static String DEFAULT_FLEET_SELECTION_KEY = "role";
    private final static String DEFAULT_FLEET_SELECTION_VALUE = "cluster";
	private final static int DEFAULT_UPDATE_PERIOD = 30;

	private final int m_cAdvisorPort;
	private final String m_etcdEndpoint;
	private final String m_fleetSelectionKey;
	private final String m_fleetSelectionValue;
	private final int m_updatePeriod;
	
	public ClusterInfoConfig(Dictionary<String, ?> properties) {
		m_cAdvisorPort = getIntValue(properties, KEY_CADVISOR_PORT, DEFAULT_CADVISOR_PORT);
		m_etcdEndpoint = getStringValue(properties, KEY_ETCD_ENDPOINT, DEFAULT_ETCD_ENDPOINT);
		m_fleetSelectionKey = getStringValue(properties, KEY_FLEET_SELECTION_KEY, DEFAULT_FLEET_SELECTION_KEY);
		m_fleetSelectionValue = getStringValue(properties, KEY_FLEET_SELECTION_VALUE, DEFAULT_FLEET_SELECTION_VALUE);
		m_updatePeriod = getIntValue(properties, KEY_UPDATE_PERIOD, DEFAULT_UPDATE_PERIOD);
	}
	
	private String getStringValue(Dictionary<String, ?> properties, String key, String defaultValue) {
		Object value = properties.get(key);
		if (value != null) {
			return value.toString();
		}
		return defaultValue;
	}

	private int getIntValue(Dictionary<String, ?> properties, String key, int defaultValue) {
		Object value = properties.get(key);
		if (value != null) {
			try {
				return Integer.parseInt(value.toString());
			}
			catch (Exception e) {
			}
		}
		return defaultValue;
	}

	public int getCadvisorPort() {
		return m_cAdvisorPort;
	}

	public String getEtcdEndpoint() {
		return m_etcdEndpoint;
	}

	public String getFleetSelectionKey() {
		return m_fleetSelectionKey;
	}

	public String getFleetSelectionValue() {
		return m_fleetSelectionValue;
	}
	
	public int getUpdatePeriod() {
		return m_updatePeriod;
	}
}
