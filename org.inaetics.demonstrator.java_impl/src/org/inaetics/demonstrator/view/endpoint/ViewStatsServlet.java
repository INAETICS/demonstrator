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
package org.inaetics.demonstrator.view.endpoint;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;
import java.util.concurrent.CopyOnWriteArrayList;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.felix.dm.Component;
import org.inaetics.demonstrator.api.stats.StatsProvider;
import org.osgi.service.log.LogService;

import com.fasterxml.jackson.core.JsonFactory;
import com.fasterxml.jackson.core.JsonGenerator;

/**
 * Tracks all {@link StatsProvider}s and shows them in a single view.
 */
public class ViewStatsServlet extends HttpServlet {
    private final CopyOnWriteArrayList<StatsProvider> m_providers;
    private final ConcurrentMap<String, TimestampMap<Double>> m_statistics;
    private final ScheduledExecutorService m_executor;
    // Injected by Felix DM...
    private volatile LogService m_log;

    private volatile Future<?> m_pollFuture;

    public ViewStatsServlet() {
        m_providers = new CopyOnWriteArrayList<>();
        m_statistics = new ConcurrentHashMap<>();
        m_executor = Executors.newSingleThreadScheduledExecutor();
    }

    public void add(StatsProvider provider) {
        if (m_providers.addIfAbsent(provider)) {
            m_statistics.putIfAbsent(provider.getName(), new TimestampMap<>());
        }
    }

    public void remove(StatsProvider provider) {
        if (m_providers.remove(provider)) {
            m_statistics.remove(provider.getName());
        }
    }

    final void pollAllProviders() {
        for (StatsProvider provider : m_providers) {
            try {
                TimestampMap<Double> values = m_statistics.get(provider.getName());
                if (values != null) {
                    // Best effort...
                    values.put(provider.getValue());
                }
            } catch (Exception e) {
                // Ignore...
                m_log.log(LogService.LOG_WARNING, "Failed to poll stats provider!", e);
            }
        }
    }

    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        resp.setContentType("application/json");

        JsonFactory factory = new JsonFactory();

        try (JsonGenerator generator = factory.createGenerator(resp.getOutputStream())) {
            String path = req.getPathInfo();

            if (path == null || "".equals(path.trim()) || "/".equals(path.trim())) {
                generator.writeStartArray();

                // Return an array with all providers...
                for (StatsProvider provider : m_providers) {
                    try {
                        generator.writeString(getProviderStatsURL(req.getRequestURL(), provider));
                    } catch (Exception e) {
                        m_log.log(LogService.LOG_WARNING, "Failed to write provider name to JSON!", e);
                    }
                }

                generator.writeEndArray();
            } else {
                String providerName = getProviderName(path);

                for (StatsProvider provider : m_providers) {
                    try {
                        if (providerName.equals(provider.getName())) {
                            writeAsJSON(generator, provider);
                        }
                    } catch (Exception e) {
                        m_log.log(LogService.LOG_WARNING, "Failed to write provider to JSON!", e);
                    }
                }
            }
        }
    }

    private String getProviderStatsURL(StringBuffer baseURL, StatsProvider provider) {
        if (baseURL.charAt(baseURL.length() - 1) != '/') {
            baseURL.append('/');
        }
        baseURL.append(provider.getName());
        return baseURL.toString();
    }

    /**
     * Called by Felix DM.
     */
    protected final void start(Component comp) {
        // Start our executor that polls all providers for their statistics...
        m_pollFuture = m_executor.scheduleAtFixedRate(new Runnable() {
            @Override
            public void run() {
                pollAllProviders();
            }
        }, 1, 1, TimeUnit.SECONDS);
    }

    /**
     * Called by Felix DM.
     */
    protected final void stop(Component comp) {
        try {
            if (m_pollFuture != null) {
                m_pollFuture.cancel(true);
                m_pollFuture = null;
            }
        } finally {
            m_executor.shutdownNow();
        }
    }

    private String getProviderName(String path) {
        String providerName = path;
        if (providerName == null) {
            return "";
        }
        if (providerName.startsWith("/")) {
            providerName = providerName.substring(1);
        }
        if (providerName.endsWith("/")) {
            providerName = providerName.substring(0, providerName.length() - 1);
        }
        return providerName;
    }

    private void writeAsJSON(JsonGenerator generator, StatsProvider provider) throws IOException {
        String name = provider.getName();
        String type = provider.getType();
        String unit = provider.getMeasurementUnit();

        TimestampMap<Double> values = m_statistics.get(name);
        List<Long> timestamps = new ArrayList<Long>(values.keySet());
        Collections.sort(timestamps);

        generator.writeStartObject();
        generator.writeStringField("name", name);
        generator.writeStringField("type", type);
        generator.writeStringField("unit", unit);
        generator.writeArrayFieldStart("timestamps");
        for (Long timestamp : timestamps) {
            generator.writeNumber(timestamp);
        }
        generator.writeEndArray();
        generator.writeArrayFieldStart("values");
        for (Long timestamp : timestamps) {
            generator.writeNumber(values.get(timestamp));
        }
        generator.writeEndArray();

        generator.writeEndObject();
    }
}
