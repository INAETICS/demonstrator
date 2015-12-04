/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.clusterinfo.k8s;

import java.io.IOException;
import java.net.URL;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Set;

import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;

public class FleetClusterNodeQuerier implements ClusterNodeQuerier {
    private final static String BASE_ETCD_DIR = "/v2/keys";
    private final static String FLEET_ETCD_DIR = "/_coreos.com/fleet/machines";
    private final static String FLEET_PUBLICIP_KEY = "PublicIP";
    private final static String FLEET_METADATA_KEY = "Metadata";

    private final String m_etcdEndpoint;
    private final ObjectMapper m_mapper;
    private final ClusterInfoConfig m_config;

    public FleetClusterNodeQuerier(ClusterInfoConfig config) {
        m_config = config;
        m_etcdEndpoint = "http://" + config.getEtcdEndpoint();
        m_mapper = new ObjectMapper();
    }

    @Override
    public Set<String> getClusterNodeIPs() throws IOException {
        Set<String> clusterNodes = new HashSet<>();

        JsonNode root = m_mapper.readTree(new URL(m_etcdEndpoint + BASE_ETCD_DIR + FLEET_ETCD_DIR)).path("node").path("nodes");
        if (root != null) {
            Iterator<JsonNode> iter = root.iterator();
            while (iter.hasNext()) {
                String f_unitKey = iter.next().path("key").textValue();

                JsonNode f_unitInfo = m_mapper.readTree(new URL(m_etcdEndpoint + BASE_ETCD_DIR + f_unitKey + "/object"))
                    .path("node").path("value");

                JsonNode md = m_mapper.readTree(f_unitInfo.asText()).path(FLEET_METADATA_KEY);

                if (md.path(m_config.getFleetSelectionKey()).textValue().equals(m_config.getFleetSelectionValue())) {
                    String ip = m_mapper.readTree(f_unitInfo.asText()).path(FLEET_PUBLICIP_KEY).textValue();

                    clusterNodes.add(ip);
                }
            }
        }

        return clusterNodes;
    }

}