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
package org.inaetics.demonstrator.stub.producer.periodic;

import java.util.concurrent.atomic.AtomicLong;

import org.inaetics.demonstrator.api.data.Sample;
import org.inaetics.demonstrator.api.queue.SampleQueue;
import org.inaetics.demonstrator.stub.producer.AbstractSampleProducer;

/**
 * Generates a random sample once every 10 ms.
 */
public class PeriodicSampleProducer extends AbstractSampleProducer {
    private final AtomicLong m_produced;

    // Injected by Felix DM...
    private volatile SampleQueue m_queue;

    public PeriodicSampleProducer() {
        super(10 /* msec */);
        m_produced = new AtomicLong(0L);
    }

    @Override
    public String getName() {
        return "Periodic Sample Producer";
    }

    @Override
    protected double calculateThroughput(long time) {
        return (1000 * m_produced.get()) / time;
    }

    @Override
    protected void produceSamples() {
        double val1 = randomSampleValue();
        double val2 = randomSampleValue();
        Sample sample = new Sample(System.currentTimeMillis(), val1, val2);

        m_queue.put(sample);
        m_produced.addAndGet(1L);

        info("Produced sample: %s", sample);
    }
}
