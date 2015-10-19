/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.clusterinfo;

import java.util.Set;

import org.inaetics.demonstrator.api.clusterinfo.ClusterNodeInfo;

/**
 * Interface for getting inforamtion about node in the cluster
 */
public interface ClusterNodeQuerier {

	public Set<ClusterNodeInfo> getClusterNodes();

}
