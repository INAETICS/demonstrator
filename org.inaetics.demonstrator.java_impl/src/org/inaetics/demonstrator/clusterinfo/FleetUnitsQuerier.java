/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.clusterinfo;

import java.net.URL;
import java.util.Iterator;
import java.util.Set;
import java.util.TimerTask;

import org.inaetics.demonstrator.api.clusterinfo.FleetUnitInfo;

import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;

public class FleetUnitsQuerier extends TimerTask {

	private final static String BASE_ETCD_DIR = "/v2/keys";
	private final static String FLEET_ETCD_DIR = "/_coreos.com/fleet/machines";
	private final static String FLEET_PUBLICIP_KEY = "PublicIP";
	private final static String FLEET_METADATA_KEY = "Metadata";

	private final String m_etcdEndpoint;
	private Set<FleetUnitInfo> m_hostList;
	private final int m_updatePeriod;
	private final ObjectMapper mapper;
	private final ClusterInfoConfig m_config;

	public FleetUnitsQuerier(Set<FleetUnitInfo> list, int updatePeriod, ClusterInfoConfig config) {
		m_config = config;
		m_etcdEndpoint = "http://" + config.getEtcdEndpoint();
		m_hostList = list;
		m_updatePeriod = updatePeriod;
		mapper = new ObjectMapper();
	}

	@Override
	public void run() {

		JsonNode root = null;

		try {
			root = mapper.readTree(new URL(m_etcdEndpoint + BASE_ETCD_DIR + FLEET_ETCD_DIR)).path("node").path("nodes");
		} catch (Exception e) {
			e.printStackTrace();
		}

		if (root != null) {

			Iterator<JsonNode> iter = root.iterator();
			while (iter.hasNext()) {
				String f_unitKey = iter.next().path("key").textValue();

				JsonNode f_unitInfo = null;

				try {
					f_unitInfo = mapper.readTree(new URL(m_etcdEndpoint + BASE_ETCD_DIR + f_unitKey + "/object"))
							.path("node").path("value");

					JsonNode md = mapper.readTree(f_unitInfo.asText()).path(FLEET_METADATA_KEY);

					if (md.path(m_config.getFleetSelectionKey()).textValue().equals(m_config.getFleetSelectionValue())) {

						String ip = mapper.readTree(f_unitInfo.asText()).path(FLEET_PUBLICIP_KEY).textValue();

						m_hostList.add(new FleetUnitInfo(ip, m_updatePeriod));

					}

				} catch (Exception e) {
					e.printStackTrace();
				}

			}

			// System.out.println(m_hostList);

		}
	}

}