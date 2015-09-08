/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.processor;

import java.util.concurrent.atomic.AtomicLong;

import org.inaetics.demonstrator.api.data.Result;
import org.inaetics.demonstrator.api.data.Sample;
import org.inaetics.demonstrator.api.datastore.DataStore;
import org.inaetics.demonstrator.api.processor.Processor;
import org.inaetics.demonstrator.api.queue.SampleQueue;

/**
 * Simple {@link Processor} implementation that take the last N samples from the
 * queue and store them in a datastore.
 */
public class IdentityProcessor extends AbstractSampleProcessor {
    private final AtomicLong m_processed;

    // Injected by Felix DM...
    private volatile SampleQueue m_queue;
    private volatile DataStore m_store;

    public IdentityProcessor() {
        super("Identity processor", 3 /* msec */);
        m_processed = new AtomicLong(0L);
    }

    @Override
    public void processSampleData() {
        Sample sample = m_queue.take();
        Result result = new Result(10, 1.0, sample);

        m_store.store(result);
        m_processed.incrementAndGet();
    }

    @Override
    protected long getProcessedCount() {
        return m_processed.get();
    }
}
