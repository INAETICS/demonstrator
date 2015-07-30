/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.producer.periodic;

import java.util.concurrent.atomic.AtomicLong;

import org.inaetics.demonstrator.api.data.Sample;
import org.inaetics.demonstrator.api.queue.SampleQueue;
import org.inaetics.demonstrator.producer.AbstractSampleProducer;

/**
 * Generates a random sample once every 10 ms.
 */
public class PeriodicSampleProducer extends AbstractSampleProducer {
    private final AtomicLong m_produced;

    // Injected by Felix DM...
    private volatile SampleQueue m_queue;

    public PeriodicSampleProducer() {
        super("Periodic Sample Producer", 10 /* msec */, 1 /* msec */);
        m_produced = new AtomicLong(0L);
    }

    @Override
    protected long getProductionCount() {
        return m_produced.get();
    }

    @Override
    protected void produceSampleData() {
        double val1 = randomSampleValue();
        double val2 = randomSampleValue();
        Sample sample = new Sample(System.currentTimeMillis(), val1, val2);

        m_queue.put(sample);
        m_produced.incrementAndGet();
    }
}
