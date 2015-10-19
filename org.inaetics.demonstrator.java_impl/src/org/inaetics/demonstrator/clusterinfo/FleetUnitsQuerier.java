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
	private final ObjectMapper m_mapper;
	private final ClusterInfoConfig m_config;
	private final ClusterInfoImpl m_clusterinfo;

	public FleetUnitsQuerier(Set<FleetUnitInfo> list, ClusterInfoConfig config, ClusterInfoImpl clusterinfo) {
		m_config = config;
		m_etcdEndpoint = "http://" + config.getEtcdEndpoint();
		m_hostList = list;
		m_mapper = new ObjectMapper();
		m_clusterinfo = clusterinfo;
	}

	@Override
	public void run() {

		JsonNode root = null;

		try {
			root = m_mapper.readTree(new URL(m_etcdEndpoint + BASE_ETCD_DIR + FLEET_ETCD_DIR)).path("node").path("nodes");
		} catch (Exception e) {
			m_clusterinfo.error("error reading fleet informations from etcd", e);
		}

		if (root != null) {

			Iterator<JsonNode> iter = root.iterator();
			while (iter.hasNext()) {
				String f_unitKey = iter.next().path("key").textValue();

				JsonNode f_unitInfo = null;

				try {
					f_unitInfo = m_mapper.readTree(new URL(m_etcdEndpoint + BASE_ETCD_DIR + f_unitKey + "/object"))
							.path("node").path("value");

					JsonNode md = m_mapper.readTree(f_unitInfo.asText()).path(FLEET_METADATA_KEY);

					if (md.path(m_config.getFleetSelectionKey()).textValue().equals(m_config.getFleetSelectionValue())) {

						String ip = m_mapper.readTree(f_unitInfo.asText()).path(FLEET_PUBLICIP_KEY).textValue();

						m_hostList.add(new FleetUnitInfo(ip));
						m_clusterinfo.log("adding host " + ip, null);
					}

				} catch (Exception e) {
					m_clusterinfo.error("error reading fleet informations from etcd", e);
				}

			}

		}
	}

}