/*
 * Copyright (c) 2015 INAETICS, <www.inaetics.org>
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
package org.inaetics.demonstrator.view.endpoint;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;
import java.util.concurrent.CopyOnWriteArrayList;
import java.util.concurrent.atomic.AtomicInteger;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.inaetics.demonstrator.api.processor.Processor;
import org.inaetics.demonstrator.api.producer.Producer;
import org.inaetics.demonstrator.api.stats.StatsProvider;
import org.osgi.framework.ServiceReference;
import org.osgi.service.log.LogService;

import com.fasterxml.jackson.core.JsonFactory;
import com.fasterxml.jackson.core.JsonGenerator;

/**
 * Provides read-only access to some of the (low-level) system statistics.
 */
public class CoordinatorServlet extends HttpServlet {

    static class StatsContainer {
        final StatsProvider m_provider;
        final TimestampMap<Double> m_stats;

        public StatsContainer(StatsProvider provider, TimestampMap<Double> stats) {
            m_provider = provider;
            m_stats = stats;
        }

        public void updateStats(Long time) {
            m_stats.put(time, m_provider.getValue());
        }
    }

    // Injected by Felix DM...
    private volatile LogService m_log;
    private volatile StatsProvider m_aggregator;

    private final ConcurrentMap<ServiceReference<StatsProvider>, StatsContainer> m_providerStats;
    private final List<Producer> m_producers;
    private final AtomicInteger m_processorCount;
    private final AtomicInteger m_productionRate;

    public CoordinatorServlet() {
        m_providerStats = new ConcurrentHashMap<>();
        m_producers = new CopyOnWriteArrayList<>();
        m_processorCount = new AtomicInteger(0);
        m_productionRate = new AtomicInteger(5);
    }

    /* Called by Felix DM. */
    public final void addProcessor(Processor p) {
        m_processorCount.incrementAndGet();
    }

    /* Called by Felix DM. */
    public final void addProducer(Producer p) {
        m_producers.add(p);
        setSampleRate(p, m_productionRate.get());
    }

    /* Called by Felix DM. */
    public final void removeProcessor(Processor p) {
        m_processorCount.decrementAndGet();
    }

    /* Called by Felix DM. */
    public final void removeProducer(Producer p) {
        m_producers.remove(p);
    }

    /* Called by Felix DM. */
    public void addStatsProvider(ServiceReference<StatsProvider> serviceRef, StatsProvider provider) {
        if ("true".equals(serviceRef.getProperty("aggregator"))) {
            m_aggregator = provider;
        }
        m_providerStats.putIfAbsent(serviceRef, new StatsContainer(provider, new TimestampMap<Double>()));
    }

    /* Called by Felix DM. */
    public void removeStatsProvider(ServiceReference<StatsProvider> serviceRef, StatsProvider provider) {
        if ("true".equals(serviceRef.getProperty("aggregator"))) {
            m_aggregator = null;
        }
        m_providerStats.remove(serviceRef);
    }

    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        resp.setContentType("application/json");

        JsonFactory factory = new JsonFactory();
        try (JsonGenerator generator = factory.createGenerator(resp.getOutputStream())) {
            String pathInfo = req.getPathInfo();

            List<String> wantedNames = new ArrayList<>();
            String queryInfo = req.getQueryString();
            if (queryInfo != null && !"".equals(queryInfo.trim())) {
                for (String name : queryInfo.split(",")) {
                    wantedNames.add(name);
                }
            }

            if ("/statistics".equals(pathInfo)) {
                resp.setStatus(HttpServletResponse.SC_OK);

                // Return an array with all providers sorted on their name...
                Map<String, StatsContainer> names = new TreeMap<>();
                for (StatsContainer c : m_providerStats.values()) {
                    c.updateStats(System.currentTimeMillis());

                    try {
                        String name = getName(c.m_provider);
                        if (wantedNames.isEmpty() || wantedNames.contains(name)) {
                            names.put(name, c);
                        }
                    } catch (Exception e) {
                        warn("Failed to get name for provider %s: %s", c.m_provider, e.getMessage());
                    }
                }

                generator.writeStartArray();

                for (StatsContainer container : names.values()) {
                    try {
                        StatsProvider provider = container.m_provider;
                        TimestampMap<Double> stats = container.m_stats;

                        if (provider != null) {
                            writeAsJSON(generator, provider, stats);
                        }
                    } catch (Exception e) {
                        warn("Failed to write provider %s to JSON: %s", container.m_provider, e.getMessage());
                    }
                }

                generator.writeEndArray();
            } else if ("/systemStats".equals(pathInfo)) {
                resp.setStatus(HttpServletResponse.SC_OK);

                generator.writeStartObject();
                generator.writeNumberField("productionRate", m_productionRate.get());
                generator.writeNumberField("processors", m_processorCount.get());
                generator.writeNumberField("producers", m_producers.size());
                generator.writeEndObject();
            } else if ("/utilisation".equals(pathInfo)) {
                resp.setStatus(HttpServletResponse.SC_OK);

                generator.writeStartObject();
                generator.writeStringField("name", m_aggregator.getName());
                generator.writeStringField("type", m_aggregator.getType());
                generator.writeStringField("unit", m_aggregator.getMeasurementUnit());
                generator.writeNumberField("value", m_aggregator.getValue());
                generator.writeEndObject();
            } else {
                resp.setStatus(HttpServletResponse.SC_NOT_FOUND);

                generator.writeStartObject();
                generator.writeEndObject();
            }
        } finally {
            resp.flushBuffer();
        }

    }

    @Override
    protected void doPost(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        Integer value = getIntParameter(req, "value");
        if (value == null || value < 0 || value > 100) {
            resp.setStatus(HttpServletResponse.SC_BAD_REQUEST);
            return;
        }

        // Keep for later use: when new producers are added...
        m_productionRate.set(value);

        for (Producer p : m_producers) {
            setSampleRate(p, value);
        }

        resp.setStatus(HttpServletResponse.SC_NO_CONTENT);
    }

    private void writeAsJSON(JsonGenerator generator, StatsProvider provider, TimestampMap<Double> stats)
        throws IOException {
        String name = getName(provider);
        String displayName = provider.getName();
        String type = provider.getType();
        String unit = provider.getMeasurementUnit();

        List<Long> timestamps = new ArrayList<Long>(stats.keySet());
        Collections.sort(timestamps);

        generator.writeStartObject();
        generator.writeStringField("name", name);
        generator.writeStringField("displayName", displayName);
        generator.writeStringField("type", type);
        generator.writeStringField("unit", unit);
        generator.writeArrayFieldStart("timestamps");
        for (Long timestamp : timestamps) {
            generator.writeNumber(timestamp);
        }
        generator.writeEndArray();
        generator.writeArrayFieldStart("values");
        for (Long timestamp : timestamps) {
            generator.writeNumber(stats.get(timestamp));
        }
        generator.writeEndArray();

        generator.writeEndObject();
    }

    private String getName(StatsProvider sp) {
        String name = sp.getName();
        return name.toLowerCase().replaceAll("\\W+", "-");
    }

    private void setSampleRate(Producer p, int newRate) {
        try {
            int rate = (int) ((p.getMaxSampleRate() / 100.0) * newRate);

            info("Setting sample rate for producer to %d (%d %%)", rate, newRate);

            p.setSampleRate(rate);
        } catch (Exception e) {
            warn("Failed to set sample rate!", e);
        }
    }

    private Integer getIntParameter(HttpServletRequest req, String paramName) {
        String[] value = req.getParameterValues(paramName);
        if (value.length == 1) {
            try {
                return Integer.parseInt(value[0]);
            } catch (NumberFormatException e) {
                // Ignore, return null...
            }
        }
        return null;
    }

    protected final void info(String msg, Object... args) {
        m_log.log(LogService.LOG_INFO, String.format(msg, args));
    }

    protected final void warn(String msg, Object... args) {
        m_log.log(LogService.LOG_WARNING, String.format(msg, args));
    }
}
