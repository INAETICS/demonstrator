/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.api.clusterinfo;

import java.util.Collection;

/**
 * Provides statistics and information about nodes in a cluster.
 */
public interface ClusterInfoProvider {

    /**
     * @return a collection with information about each node in a cluster, cannot be <code>null</code>, but an empty collection is possible.
     */
    Collection<NodeInfo> getClusterInfo();

}
