/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.coordinator.autoscaler;

import static org.inaetics.demonstrator.coordinator.util.ConfigUtils.*;

import java.util.Dictionary;

import org.osgi.service.cm.ConfigurationException;

/**
 * Configuration class for the scaler
 */
public class AutoScalerConfig {
    private static final String KEY_POLL_INTERVAL = "poll.interval";
    private static final String KEY_QUEUE_LOW_BARRIER = "queue.barrierLow";
    private static final String KEY_QUEUE_HIGH_BARRIER = "queue.barrierHigh";

    private final int m_pollInterval;
    private final int m_queueLowBarrier;
    private final int m_queueHighBarrier;

    public AutoScalerConfig() {
        m_queueLowBarrier = 40;
        m_queueHighBarrier = 60;
        m_pollInterval = 5;
    }

    public AutoScalerConfig(Dictionary<String, ?> properties) throws ConfigurationException {
        m_pollInterval = getConfigIntValue(properties, KEY_POLL_INTERVAL);
        if (m_pollInterval < 1) {
            throw new ConfigurationException(KEY_POLL_INTERVAL, "invalid value!");
        }
        m_queueLowBarrier = getConfigIntValue(properties, KEY_QUEUE_LOW_BARRIER);
        if (m_queueLowBarrier < 1 || m_queueLowBarrier > 100) {
            throw new ConfigurationException(KEY_QUEUE_LOW_BARRIER, "invalid value!");
        }
        m_queueHighBarrier = getConfigIntValue(properties, KEY_QUEUE_HIGH_BARRIER);
        if (m_queueHighBarrier < 1 || m_queueHighBarrier > 100) {
            throw new ConfigurationException(KEY_QUEUE_HIGH_BARRIER, "invalid value!");
        }
    }

    public int getPollInterval() {
        return m_pollInterval;
    }

    public int getQueueHighBarrier() {
        return m_queueHighBarrier;
    }

    public int getQueueLowBarrier() {
        return m_queueLowBarrier;
    }
}