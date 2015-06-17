/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.view.endpoint;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.TreeMap;
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
import org.osgi.framework.ServiceReference;
import org.osgi.service.log.LogService;

import com.fasterxml.jackson.core.JsonFactory;
import com.fasterxml.jackson.core.JsonGenerator;

/**
 * Tracks all {@link StatsProvider}s and shows them in a single view.
 */
public class ViewStatsServlet extends HttpServlet {
    static class Container {
        final StatsProvider m_provider;
        final TimestampMap<Double> m_stats;

        public Container(StatsProvider provider, TimestampMap<Double> stats) {
            m_provider = provider;
            m_stats = stats;
        }

        public void updateStats(Long time) {
            m_stats.put(time, m_provider.getValue());
        }
    }

    private final ConcurrentMap<ServiceReference<StatsProvider>, Container> m_providerStats;
    private final ScheduledExecutorService m_executor;
    // Injected by Felix DM...
    private volatile LogService m_log;

    private volatile Future<?> m_pollFuture;

    public ViewStatsServlet() {
        m_providerStats = new ConcurrentHashMap<>();
        m_executor = Executors.newSingleThreadScheduledExecutor();
    }

    public void add(ServiceReference<StatsProvider> serviceRef, StatsProvider provider) {
        m_providerStats.putIfAbsent(serviceRef, new Container(provider, new TimestampMap<Double>()));
    }

    public void remove(ServiceReference<StatsProvider> serviceRef, StatsProvider provider) {
        m_providerStats.remove(serviceRef);
    }

    final void pollAllProviders() {
        Long now = System.currentTimeMillis();
        for (Container container : m_providerStats.values()) {
            try {
                // Best effort...
                container.updateStats(now);
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
            generator.writeStartArray();

            // Return an array with all providers sorted on their name...
            Map<String, Container> names = new TreeMap<>();
            for (Container c : m_providerStats.values()) {
                names.put(c.m_provider.getName(), c);
            }

            for (Container container : names.values()) {
                try {
                    StatsProvider provider = container.m_provider;
                    TimestampMap<Double> stats = container.m_stats;

                    if (provider != null) {
                        writeAsJSON(generator, provider, stats);
                    }
                } catch (Exception e) {
                    m_log.log(LogService.LOG_WARNING, "Failed to write provider name to JSON!", e);
                }
            }

            generator.writeEndArray();
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
        String name = provider.getName();
        name = name.toLowerCase(Locale.US).replaceAll("\\W+", "");
        return name + "_" + System.identityHashCode(provider);
    }

    private void writeAsJSON(JsonGenerator generator, StatsProvider provider, TimestampMap<Double> stats)
        throws IOException {
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
