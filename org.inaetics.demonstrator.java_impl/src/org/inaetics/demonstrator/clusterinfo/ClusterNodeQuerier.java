package org.inaetics.demonstrator.clusterinfo;

import java.util.Set;

import org.inaetics.demonstrator.api.clusterinfo.ClusterNodeInfo;

public interface ClusterNodeQuerier {

	public Set<ClusterNodeInfo> getClusterNodes();

}
