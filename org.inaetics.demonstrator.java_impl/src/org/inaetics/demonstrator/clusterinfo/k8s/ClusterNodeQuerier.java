/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.clusterinfo.k8s;

import java.io.IOException;
import java.util.Set;

/**
 * Interface for getting information about node in the cluster
 */
public interface ClusterNodeQuerier {

	public Set<String> getClusterNodeIPs() throws IOException;

}
