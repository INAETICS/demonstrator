/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.api.coordinator;

import aQute.bnd.annotation.ProviderType;

/**
 * Provides a concrete implementation of a coordinator service, such as for Kubernetes.
 */
@ProviderType
public interface CoordinatorService {

    enum Type {
        /** Entities that process samples from the queue and store the result in a datastore. */
        PROCESSOR,
        /** Entities that produce samples. */
        PRODUCER;
    }

    /**
     * Returns the maximum amount of replicas allowed for a given type of service.
     * 
     * @param type the type of service to get the replica-count for, cannot be <code>null</code>;
     * @return the maximum amount of replicas, &gt;= 0.
     */
    int getMaxReplicaCount(Type type);

    /**
     * Returns the current amount of replicas for a given type of service.
     * 
     * @param type the type of service to get the replica-count for, cannot be <code>null</code>;
     * @return the current amount of replicas, &gt;= 0.
     */
    int getReplicaCount(Type type);

    /**
     * Sets the desired amount of replicas for a given type of service.
     * 
     * @param type the type of service to set the replica-count for, cannot be <code>null</code>;
     * @param number the new desired number of replicas, &gt;= 0. Negative values or values beyond the maximum number of replicas are ignored.
     * @return <code>true</code> if the replica count is changed, <code>false</code> otherwise.
     */
    boolean setReplicaCount(Type type, int number);
}
