/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.utilization;

import java.util.ArrayList;
import java.util.List;

import org.inaetics.demonstrator.api.producer.Producer;
import org.inaetics.demonstrator.api.stats.StatsProvider;

/**
 * Aggregates the utilization of other {@link StatsProvider}s.
 */
public class UtilitizationAggegrator implements StatsProvider {
    private final List<Producer> m_producers;

    /**
     * Creates a new {@link UtilitizationAggegrator} instance.
     */
    public UtilitizationAggegrator() {
        m_producers = new ArrayList<>();
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
        double actual = 0, max = 0;

        for (Producer producer : m_producers) {
            actual += producer.getSampleRate();
            max += producer.getMaxSampleRate();
        }

        return (100.0 * actual) / max;
    }
}
