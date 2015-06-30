package org.inaetics.demonstrator.k8sclient;

/**
 * Represents a replication controller 
 *
 */
public interface ReplicationController {

	/**
	 * Increase the desired number of replicas by 1
	 * 
	 * @param maxCount the maximum number of replicas
	 */
	public abstract void increaseReplicaCount(int maxCount);

	/**
	 * Descrease the desired number of replicas by 1
	 * 
	 * @param minCount the minimum number of replicas
	 */
	public abstract void decreaseReplicaCount(int minCount);

	/**
	 * Get the current number of desired replicas. Can be different from the number of actually running replicas.
	 * @return
	 */
	public abstract int getReplicaCount();

	/**
	 * Set the desired number of replicas
	 * 
	 * @param replicaCount the desired number of replicas
	 */
	public abstract void setReplicaCount(int replicaCount);

	/**
	 * Get the name of the replication controller
	 * @return the name of the replication controller
	 */
	public abstract String getName();

}