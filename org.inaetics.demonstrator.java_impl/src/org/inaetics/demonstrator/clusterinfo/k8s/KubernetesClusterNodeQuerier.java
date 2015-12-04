/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.clusterinfo.k8s;

import java.io.IOException;
import java.util.HashSet;
import java.util.Set;

import org.amdatu.kubernetes.Kubernetes;

import io.fabric8.kubernetes.api.model.NodeList;

/**
 * Use Kubernetes to get information about the cluster nodes
 */
public class KubernetesClusterNodeQuerier implements ClusterNodeQuerier {
    private final Kubernetes m_kubernetes;

    public KubernetesClusterNodeQuerier(Kubernetes kubernetes) {
        m_kubernetes = kubernetes;
    }

    @Override
    public Set<String> getClusterNodeIPs() throws IOException {
        Set<String> result = new HashSet<>();
        
        NodeList nodeList = m_kubernetes.listNodes().toBlocking().first();
        nodeList.getItems().stream()
            .forEach(node -> result.add(node.getStatus().getAddresses().stream()
                .filter(address -> address.getType().equals("LegacyHostIP")).findFirst().get().getAddress()));
        return result;
    }

}
