/*
 * Copyright (c) 2014 INAETICS, <www.inaetics.org>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package org.inaetics.demonstrator.stub.processor;

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
        super(20 /* msec */);
        m_processed = new AtomicLong(0L);
    }

    @Override
    public void processSamples() {
        Sample sample = m_queue.take();
        Result result = new Result(10, 1.0, sample);

        m_store.store(result);
        m_processed.incrementAndGet();
    }

    @Override
    protected double calculateThroughput(long time) {
        return time == 0 ? 0 : (1000 * m_processed.get()) / time;
    }
}
