/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.coordinator.k8s;

import java.util.Dictionary;

import org.amdatu.kubernetes.Kubernetes;
import org.inaetics.demonstrator.api.coordinator.CoordinatorService;
import org.osgi.service.cm.ConfigurationException;
import org.osgi.service.cm.ManagedService;
import org.osgi.service.log.LogService;

import rx.Observable;

/**
 * Provides a coordinator service that is using Kubernetes to scale up/down replicas of services.
 */
public class KubernetesCoordinatorService implements ManagedService, CoordinatorService {
    private static final boolean CURRENT_REPLICA_COUNT = true;
    private static final boolean MAX_REPLICA_COUNT = false;

    // Managed by Felix DM...
    private volatile LogService m_log;
    private volatile Kubernetes m_k8s;
    // Locally managed...
    private volatile CoordinatorConfig m_config;

    public KubernetesCoordinatorService() {
        this(new CoordinatorConfig());
    }

    public KubernetesCoordinatorService(CoordinatorConfig config) {
        m_config = config;
    }

    @Override
    public int getMaxReplicaCount(Type type) {
        CoordinatorConfig config = m_config;

        String[] names;
        switch (type) {
            case PROCESSOR:
                names = config.getProcessorControllerNames();
                break;
            case PRODUCER:
                names = config.getProducerControllerNames();
                break;
            default:
                throw new IllegalArgumentException("Invalid type: " + type);
        }

        return getReplicaCount(config.getKubernetesNamespace(), names, MAX_REPLICA_COUNT);
    }

    @Override
    public int getReplicaCount(Type type) {
        CoordinatorConfig config = m_config;

        String[] names;
        switch (type) {
            case PROCESSOR:
                names = config.getProcessorControllerNames();
                break;
            case PRODUCER:
                names = config.getProducerControllerNames();
                break;
            default:
                throw new IllegalArgumentException("Invalid type: " + type);
        }

        return getReplicaCount(config.getKubernetesNamespace(), names, CURRENT_REPLICA_COUNT);
    }

    @Override
    public boolean setReplicaCount(Type type, int number) {
        if (number < 0) {
            // Ignore invalid value...
            return false;
        }

        CoordinatorConfig config = m_config;

        String[] names;
        switch (type) {
            case PROCESSOR:
                names = config.getProcessorControllerNames();
                break;
            case PRODUCER:
                names = config.getProducerControllerNames();
                break;
            default:
                throw new IllegalArgumentException("Invalid type: " + type);
        }

        setReplicaCount(config.getKubernetesNamespace(), names, number);
        // TODO
        return true;
    }

    @Override
    public void updated(Dictionary<String, ?> properties) throws ConfigurationException {
        if (properties != null) {
            m_config = new CoordinatorConfig(properties);
        } else {
            m_config = new CoordinatorConfig();
        }
    }

    private void setReplicaCount(String ns, String[] rcNames, int newReplicaCount) {
        Observable
            .from(rcNames)
            .flatMap(rcName -> m_k8s.getReplicationController(ns, rcName))
            .reduce((a, b) -> a.getStatus().getReplicas() < b.getStatus().getReplicas() ? a : b)
            .subscribe(
                rc -> {
                    String rcName = rc.getMetadata().getName();
                    int curReplicaCount = rc.getStatus().getReplicas();
                    if (curReplicaCount != newReplicaCount) {
                        m_log.log(LogService.LOG_INFO,
                            String.format("Scaling %s to %d replicas...", rcName, newReplicaCount));

                        m_k8s.scale(ns, rcName, newReplicaCount).subscribe(ignored -> {},
                            t -> m_log.log(LogService.LOG_ERROR, "Scaling of " + rcName + " failed...", t));
                    }
                }, //
                t -> m_log.log(LogService.LOG_ERROR, "Setting replica count failed...", t));
    }

    private int getReplicaCount(String ns, String[] rcNames, boolean currentOrMax) {
        return Observable.from(rcNames).flatMap(rcName -> m_k8s.getReplicationController(ns, rcName))
            .<Integer> map(rc -> currentOrMax ? rc.getStatus().getReplicas() : rc.getSpec().getReplicas()) //
            .reduce(0, (a, b) -> a + b) //
            .toBlocking() //
            .last();
    }
}
