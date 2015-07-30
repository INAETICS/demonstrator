/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.utilization;

import java.util.List;
import java.util.concurrent.CopyOnWriteArrayList;

import org.inaetics.demonstrator.api.producer.Producer;
import org.inaetics.demonstrator.api.stats.StatsProvider;
import org.osgi.service.log.LogService;

/**
 * Aggregates the utilization of other {@link StatsProvider}s.
 */
public class UtilitizationAggegrator implements StatsProvider {
    private final List<Producer> m_producers;
    // Injected by Felix DM...
    private volatile LogService m_log;

    /**
     * Creates a new {@link UtilitizationAggegrator} instance.
     */
    public UtilitizationAggegrator() {
        m_producers = new CopyOnWriteArrayList<>();
    }

    public void addProducer(Producer producer) {
        m_producers.add(producer);
    }

    public void removeProducer(Producer producer) {
        m_producers.remove(producer);
    }

    @Override
    public String getMeasurementUnit() {
        return "%";
    }

    @Override
    public String getName() {
        return "Utilization";
    }

    @Override
    public String getType() {
        return "utilization";
    }

    @Override
    public double getValue() {
        long actual = 0, max = 0;

        for (Producer producer : m_producers) {
            try {
                actual += producer.getSampleRate();
                max += producer.getMaxSampleRate();
            } catch (Exception e) {
                m_log.log(LogService.LOG_WARNING, "Failed to get utilization from producer!", e);
            }
        }

        if (max == 0) {
            // 
            return 0.0;
        }
        
        return (100.0 * actual) / max;
    }
}
