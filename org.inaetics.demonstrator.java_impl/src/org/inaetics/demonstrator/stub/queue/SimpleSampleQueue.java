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
package org.inaetics.demonstrator.stub.queue;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Dictionary;
import java.util.List;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;

import org.inaetics.demonstrator.api.data.Sample;
import org.inaetics.demonstrator.api.queue.SampleQueue;
import org.inaetics.demonstrator.api.stats.StatsProvider;
import org.osgi.service.cm.ConfigurationException;
import org.osgi.service.cm.ManagedService;
import org.osgi.service.log.LogService;

public class SimpleSampleQueue implements SampleQueue, StatsProvider, ManagedService {

    private final BlockingQueue<Sample> m_queue;

    // Injected by Felix DM...
    private volatile LogService m_log;

    public SimpleSampleQueue() {
        m_queue = new LinkedBlockingQueue<>();
    }

    @Override
    public String getName() {
        return "Queue";
    }

    @Override
    public String getMeasurementUnit() {
        return "";
    }

    @Override
    public String getType() {
        return "utilization";
    }

    @Override
    public double getValue() {
        return m_queue.size(); // XXX
    }

    @Override
    public boolean put(Sample sample) {
        try {
            m_queue.put(sample);
            return true;
        } catch (InterruptedException e) {
            return false;
        }
    }

    @Override
    public int putAll(Collection<Sample> samples) {
        int size = m_queue.size();
        for (Sample sample : samples) {
            put(sample);
        }

        return m_queue.size() - size;
    }

    @Override
    public Sample take() {
        try {
            Sample result = m_queue.take();

            return result;
        } catch (InterruptedException e) {
            return null;
        }
    }

    @Override
    public Collection<Sample> takeMinMax(int min, int max) {
        List<Sample> result = new ArrayList<>(max);
        // NOTE: this is not strictly conform our contract...
        m_queue.drainTo(result, max);

        return result;
    }

    @Override
    public void updated(Dictionary<String, ?> properties) throws ConfigurationException {
        // Nothing yet...
        m_log.log(LogService.LOG_INFO, "SimpleSampleQueue updated...");
    }
}
