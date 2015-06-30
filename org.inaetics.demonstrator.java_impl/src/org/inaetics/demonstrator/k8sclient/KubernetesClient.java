package org.inaetics.demonstrator.k8sclient;

import org.inaetics.demonstrator.k8sclient.impl.rc.ReplicationControllerImpl;

/**
 * Provides a Java client for the Kubernetes REST API
 * Very limited functionality at the moment
 *
 */
public interface KubernetesClient {

	/**
	 * Get the {@link ReplicationControllerImpl} with the given name
	 * 
	 * @param name the name of the ReplicationController
	 * @return the ReplicationController
	 */
	public abstract ReplicationController getReplicationController(String name);

	/**
	 * Update the given {@link ReplicationControllerImpl}
	 * 
	 * @param rc the ReplicationController to update
	 */
	public abstract void updateReplicationController(ReplicationController rc);

}