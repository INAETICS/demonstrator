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

import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Dictionary;
import java.util.List;
import java.util.Map;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;

import org.inaetics.demonstrator.api.data.Sample;
import org.inaetics.demonstrator.api.queue.SampleQueue;
import org.inaetics.demonstrator.stub.stats.StatsProvider;
import org.osgi.service.cm.ConfigurationException;
import org.osgi.service.cm.ManagedService;
import org.osgi.service.log.LogService;

public class SimpleSampleQueue extends StatsProvider implements SampleQueue, ManagedService {
    private static final String TITLE = "Queue Stats";

    private final BlockingQueue<Sample> m_queue;
    private final SizeStatsCollector m_statsCollector;
    private volatile double m_lastSlope;

    // Injected by Felix DM...
    private volatile LogService m_log;

    public SimpleSampleQueue() {
        m_queue = new LinkedBlockingQueue<>();
        m_statsCollector = new SizeStatsCollector();
        m_lastSlope = 0.0;
    }

    @Override
    public boolean put(Sample sample) {
        try {
            m_queue.put(sample);

            m_statsCollector.put(m_queue.size());
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

        m_statsCollector.put(m_queue.size());
        return m_queue.size() - size;
    }

    @Override
    public Sample take() {
        try {
            Sample result = m_queue.take();

            m_statsCollector.put(m_queue.size());

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
        m_statsCollector.put(m_queue.size());

        return result;
    }

    @Override
    public void updated(Dictionary<String, ?> properties) throws ConfigurationException {
        // Nothing yet...
        m_log.log(LogService.LOG_INFO, "SimpleSampleQueue updated...");
    }

    @Override
    protected String getTitle() {
        return TITLE;
    }

    @Override
    protected void renderStats(PrintWriter writer) {
        SizeStats stats = m_statsCollector.getStats();

        double slope = stats.getSlope();
        renderSlope(writer, m_lastSlope, slope);
        m_lastSlope = slope;

        writer.append("<dl class=\"dl-horizontal\">");
        for (Map.Entry<Long, Integer> entry : stats.getEntries()) {
            writer.printf("<dt>%1$tF %1$tT.%1$tL</dt><dd>size: %2$d entries</dd>", entry.getKey(), entry.getValue());
        }
        writer.append("</dl>");
    }
}
