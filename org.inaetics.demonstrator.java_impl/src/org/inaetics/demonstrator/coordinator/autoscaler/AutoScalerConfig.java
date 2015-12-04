/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.coordinator.autoscaler;

import static org.inaetics.demonstrator.util.config.ConfigUtils.*;

import java.util.Dictionary;

import org.osgi.service.cm.ConfigurationException;

/**
 * Configuration class for the scaler
 */
public class AutoScalerConfig {
    private static final String KEY_POLL_INTERVAL = "poll.interval";
    private static final String KEY_QUEUE_LOW_BARRIER = "queue.barrierLow";
    private static final String KEY_QUEUE_HIGH_BARRIER = "queue.barrierHigh";
    private static final String KEY_QUEUE_SLOPE1 = "queue.slope1";
    private static final String KEY_QUEUE_SLOPE2 = "queue.slope2";

    private final int m_pollInterval;
    private final int m_queueLowBarrier;
    private final int m_queueHighBarrier;
    private final double m_slope1; 
    private final double m_slope2; 

    public AutoScalerConfig() {
        m_queueLowBarrier = 10;
        m_queueHighBarrier = 35;
        m_pollInterval = 5;
        m_slope1 = 5;
        m_slope2 = 10;
    }

    public AutoScalerConfig(Dictionary<String, ?> properties) throws ConfigurationException {
        m_pollInterval = getIntValue(properties, KEY_POLL_INTERVAL);
        if (m_pollInterval < 1) {
            throw new ConfigurationException(KEY_POLL_INTERVAL, "invalid value!");
        }
        m_queueLowBarrier = getIntValue(properties, KEY_QUEUE_LOW_BARRIER);
        if (m_queueLowBarrier < 1 || m_queueLowBarrier > 100) {
            throw new ConfigurationException(KEY_QUEUE_LOW_BARRIER, "invalid value!");
        }
        m_queueHighBarrier = getIntValue(properties, KEY_QUEUE_HIGH_BARRIER);
        if (m_queueHighBarrier < 1 || m_queueHighBarrier > 100 || m_queueHighBarrier <= m_queueLowBarrier) {
            throw new ConfigurationException(KEY_QUEUE_HIGH_BARRIER, "invalid value!");
        }
        m_slope1 = getIntValue(properties, KEY_QUEUE_SLOPE1);
        if (getSlope1() < 0 || getSlope1() > 100) {
            throw new ConfigurationException(KEY_QUEUE_SLOPE1, "invalid value!");
        }
        m_slope2 = getIntValue(properties, KEY_QUEUE_SLOPE2);
        if (m_slope2 < 0 || m_slope2 > 100 || m_slope2 <= getSlope1()) {
            throw new ConfigurationException(KEY_QUEUE_SLOPE2, "invalid value!");
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

	public double getSlope1() {
		return m_slope1;
	}

	public double getSlope2() {
		return m_slope2;
	}
}