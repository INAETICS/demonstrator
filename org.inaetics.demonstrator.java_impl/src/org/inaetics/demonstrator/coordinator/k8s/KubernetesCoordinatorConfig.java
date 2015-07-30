/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.coordinator.k8s;

import static org.inaetics.demonstrator.coordinator.util.ConfigUtils.*;

import java.util.Dictionary;

import org.inaetics.demonstrator.coordinator.util.CoordinatorConfig;
import org.osgi.service.cm.ConfigurationException;

/**
 * Configuration class for the coordinator
 */
public class KubernetesCoordinatorConfig extends CoordinatorConfig {
    private static final String KEY_K8S_NAMESPACE = "kubernetes.namespace";
    private static final String KEY_PROCESSOR_NAMES = "processor.names";
    private static final String KEY_PRODUCER_NAMES = "producer.names";

    private final String[] m_processorNames;
    private final String[] m_producerNames;
    private final String m_k8sNamespace;

    public KubernetesCoordinatorConfig() {
    	super();
        m_k8sNamespace = "default";
        m_processorNames = new String[] { "inaetics-processor-controller", "inaetics-processor-celix-controller" };
        m_producerNames = new String[] { "inaetics-producer-controller" };
    }

    public KubernetesCoordinatorConfig(Dictionary<String, ?> properties) throws ConfigurationException {
        super(properties);
    	m_processorNames = getConfigStringArrayValue(properties, KEY_PROCESSOR_NAMES);
        m_producerNames = getConfigStringArrayValue(properties, KEY_PRODUCER_NAMES);
        m_k8sNamespace = getConfigStringValue(properties, KEY_K8S_NAMESPACE);
    }

    public String getKubernetesNamespace() {
        return m_k8sNamespace;
    }

    public String[] getProcessorControllerNames() {
        return m_processorNames;
    }

    public String[] getProducerControllerNames() {
        return m_producerNames;
    }
}