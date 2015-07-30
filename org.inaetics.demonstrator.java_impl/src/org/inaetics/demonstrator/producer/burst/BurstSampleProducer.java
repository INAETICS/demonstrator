/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.producer.burst;

import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicLong;

import org.inaetics.demonstrator.api.data.Sample;
import org.inaetics.demonstrator.api.queue.SampleQueue;
import org.inaetics.demonstrator.producer.AbstractSampleProducer;

/**
 * Generates a burst of samples once every 500 ms.
 */
public class BurstSampleProducer extends AbstractSampleProducer {
    private static final int BURST_LENGTH = 10; // samples

    private final AtomicLong m_produced;

    // Injected by Felix DM...
    private volatile SampleQueue m_queue;

    public BurstSampleProducer() {
        super("Burst Sample Producer", 500 /* msec */, 1 /* msec */);
        m_produced = new AtomicLong(0L);
    }

    @Override
    protected long getProductionCount() {
        return m_produced.get();
    }

    @Override
    protected void produceSampleData() throws InterruptedException {
        for (int i = 0; !Thread.currentThread().isInterrupted() && i < BURST_LENGTH; i++) {

            double val1 = randomSampleValue();
            double val2 = randomSampleValue();
            Sample sample = new Sample(System.currentTimeMillis(), val1, val2);

            m_queue.put(sample);
            m_produced.addAndGet(1l);
        }

        TimeUnit.MILLISECONDS.sleep(150); // simulate dead-time...
    }
}
