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
import java.util.Locale;
import java.util.Map.Entry;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;
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
    private final ConcurrentMap<String, StatsProvider> m_providers;
    private final ConcurrentMap<String, TimestampMap<Double>> m_statistics;
    private final ScheduledExecutorService m_executor;
    // Injected by Felix DM...
    private volatile LogService m_log;

    private volatile Future<?> m_pollFuture;

    public ViewStatsServlet() {
        m_providers = new ConcurrentHashMap<>();
        m_statistics = new ConcurrentHashMap<>();
        m_executor = Executors.newSingleThreadScheduledExecutor();
    }

    public void add(StatsProvider provider) {
        String name = getKey(provider);

        if (m_providers.putIfAbsent(name, provider) == null) {
            m_statistics.putIfAbsent(name, new TimestampMap<Double>());
        }
    }

    public void remove(StatsProvider provider) {
        // Do *not* use the getKey method, as we're not sure the provider is still "alive"...
        String name = null;
        for (Entry<String, StatsProvider> entry : m_providers.entrySet()) {
            if (entry.getValue() == provider) {
                // Found it...
                name = entry.getKey();
            }
        }
        if (name != null) {
            m_providers.remove(name);
            m_statistics.remove(name);
        }
    }

    final void pollAllProviders() {
        for (Entry<String, StatsProvider> entry : m_providers.entrySet()) {
            try {
                TimestampMap<Double> values = m_statistics.get(entry.getKey());
                if (values != null) {
                    StatsProvider provider = entry.getValue();
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
                for (Entry<String, StatsProvider> entry : m_providers.entrySet()) {
                    try {
                        String name = entry.getKey();
                        String url = getProviderStatsURL(req.getRequestURL(), name);

                        generator.writeStartObject();
                        generator.writeStringField("name", name);
                        generator.writeStringField("url", url);
                        generator.writeEndObject();
                    } catch (Exception e) {
                        m_log.log(LogService.LOG_WARNING, "Failed to write provider name to JSON!", e);
                    }
                }

                generator.writeEndArray();
            } else {
                try {
                    String providerName = getProviderName(path);
                    StatsProvider provider = m_providers.get(providerName);
                    if (provider != null) {
                        TimestampMap<Double> stats = m_statistics.get(providerName);
                        if (stats != null) {
                            writeAsJSON(generator, provider, stats);
                        } else {
                            resp.setStatus(HttpServletResponse.SC_NO_CONTENT);
                        }
                    } else {
                        resp.sendError(HttpServletResponse.SC_NOT_FOUND);
                    }

                    resp.flushBuffer();
                } catch (Exception e) {
                    if (!resp.isCommitted()) {
                        resp.sendError(HttpServletResponse.SC_INTERNAL_SERVER_ERROR, e.getMessage());
                    }
                    m_log.log(LogService.LOG_WARNING, "Failed to write provider to JSON!", e);
                }
            }
        }
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

    private String getKey(StatsProvider provider) {
        String name = provider.getName() + "-" + provider.hashCode();
        return name.toLowerCase(Locale.US).replaceAll("\\W+", "");
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

    private String getProviderStatsURL(StringBuffer baseURL, String providerName) {
        if (baseURL.charAt(baseURL.length() - 1) != '/') {
            baseURL.append('/');
        }
        baseURL.append(providerName);
        return baseURL.toString();
    }

    private void writeAsJSON(JsonGenerator generator, StatsProvider provider, TimestampMap<Double> stats) throws IOException {
        String name = getKey(provider);
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
}
