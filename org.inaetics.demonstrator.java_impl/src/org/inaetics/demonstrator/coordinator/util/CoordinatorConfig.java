/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.coordinator.util;

import static org.inaetics.demonstrator.coordinator.util.ConfigUtils.*;

import java.util.Dictionary;

import org.osgi.service.cm.ConfigurationException;

/**
 * Configuration class for the coordinator
 */
public class CoordinatorConfig {
    private static final String KEY_MAX_NR_PROCESSORS = "processors.max";
    private static final String KEY_MAX_NR_PRODUCERS = "producers.max";

    private final int m_maxNrProcessors;
    private final int m_maxNrProducers;

    public CoordinatorConfig() {
        m_maxNrProcessors = 8;
        m_maxNrProducers = 4;
    }

    public CoordinatorConfig(Dictionary<String, ?> properties) throws ConfigurationException {
        m_maxNrProcessors = getConfigIntValue(properties, KEY_MAX_NR_PROCESSORS);
        m_maxNrProducers = getConfigIntValue(properties, KEY_MAX_NR_PRODUCERS);
    }

    public int getMaxNrProcessors() {
        return m_maxNrProcessors;
    }

    public int getMaxNrProducers() {
        return m_maxNrProducers;
    }
}