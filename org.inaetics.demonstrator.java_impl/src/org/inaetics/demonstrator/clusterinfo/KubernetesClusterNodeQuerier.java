/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.clusterinfo;

import java.util.HashSet;
import java.util.Set;

import org.amdatu.kubernetes.Kubernetes;
import org.inaetics.demonstrator.api.clusterinfo.ClusterNodeInfo;

import io.fabric8.kubernetes.api.model.NodeList;

/**
 * Use Kubernetes to get information about the cluster nodes
 */
public class KubernetesClusterNodeQuerier implements ClusterNodeQuerier {
	
//	private ClusterInfoConfig m_config;
	private ClusterInfoImpl m_clusterinfo;
	private Kubernetes m_kubernetes;

	public KubernetesClusterNodeQuerier(ClusterInfoConfig config, ClusterInfoImpl clusterinfo, Kubernetes kubernetes) {
//		m_config = config;
		m_clusterinfo = clusterinfo;
		m_kubernetes = kubernetes;
	}

	@Override
	public Set<ClusterNodeInfo> getClusterNodes() {

		NodeList nodeList = m_kubernetes.listNodes().toBlocking().first();
		m_clusterinfo.log("found " + nodeList.getItems().size() + " cluster nodes", null);
		Set<ClusterNodeInfo> clusterNodes = new HashSet<>();
		nodeList.getItems().stream().forEach(node -> clusterNodes.add(
				new ClusterNodeInfo(
						node.getStatus().getAddresses().stream().filter(address -> address.getType().equals("LegacyHostIP")).findFirst().get().getAddress())));
		return clusterNodes;
	}

}
