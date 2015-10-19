package org.inaetics.demonstrator.clusterinfo;

public class ClusterInfoConfig {
	
	private final static int DEFAULT_CADVISOR_PORT = 4194;
	private final static String DEFAULT_ETCD_ENDPOINT = "172.17.8.20:2379";
    private final static String DEFAULT_FLEET_SELECTION_KEY = "role";
    private final static String DEFAULT_FLEET_SELECTION_VALUE = "cluster";
	private final static int DEFAULT_UPDATE_PERIOD = 10;

	public int getCadvisorPort() {
		return DEFAULT_CADVISOR_PORT;
	}

	public String getEtcdEndpoint() {
		return DEFAULT_ETCD_ENDPOINT;
	}

	public String getFleetSelectionKey() {
		return DEFAULT_FLEET_SELECTION_KEY;
	}

	public String getFleetSelectionValue() {
		return DEFAULT_FLEET_SELECTION_VALUE;
	}
	
	public int getUpdatePeriod() {
		return DEFAULT_UPDATE_PERIOD;
	}
}
