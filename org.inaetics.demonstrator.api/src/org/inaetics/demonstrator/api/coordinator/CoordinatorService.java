/*
 * Copyright (c) 2015 INAETICS, <www.inaetics.org>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
