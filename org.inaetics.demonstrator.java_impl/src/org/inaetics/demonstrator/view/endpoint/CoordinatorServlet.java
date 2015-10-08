/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.view.endpoint;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Set;
import java.util.TreeSet;
import java.util.concurrent.Callable;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;
import java.util.concurrent.CopyOnWriteArrayList;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.inaetics.demonstrator.api.coordinator.CoordinatorService;
import org.inaetics.demonstrator.api.coordinator.CoordinatorService.Type;
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

    static class StatsContainer implements Comparable<StatsContainer> {
        final StatsProvider m_provider;
        final TimestampMap<Double> m_stats;
        String name;

        public StatsContainer(StatsProvider provider, TimestampMap<Double> stats) {
            m_provider = provider;
            m_stats = stats;
        }

        public void updateStats(Long time) {
            m_stats.put(time, m_provider.getValue());
        }

		@Override
		public int compareTo(StatsContainer other) {
			if (name == null) {
				return 1;
			}
			if (other == null || other.name == null) {
				return -1;
			}
			return name.compareTo(other.name);
		}
    }

    // Injected by Felix DM...
    private volatile LogService m_log;
    private volatile StatsProvider m_aggregator;
    private volatile CoordinatorService m_coordinator;

    private final ConcurrentMap<ServiceReference<StatsProvider>, StatsContainer> m_providerStats;
    private final List<Producer> m_producers;
    private final AtomicInteger m_processorCount;
    private final AtomicInteger m_producerCount;
    private final AtomicInteger m_productionRate;
	private ScheduledExecutorService m_executor;

    public CoordinatorServlet() {
        m_providerStats = new ConcurrentHashMap<>();
        m_producers = new CopyOnWriteArrayList<>();
        m_processorCount = new AtomicInteger(0);
        m_producerCount = new AtomicInteger(0);
        m_productionRate = new AtomicInteger(5);
    }
    
    // called by Felix DM
    protected final void start() {
		m_executor = Executors.newScheduledThreadPool(10);
    }
    
    protected final void stop() throws Exception {
        m_executor.shutdown();
        m_executor.awaitTermination(10, TimeUnit.SECONDS);
    }

    /* Called by Felix DM. */
    public final void addProducer(Producer p) {
        m_producers.add(p);
        setSampleRate(p, m_productionRate.get());
        info("added producer");
    }

    /* Called by Felix DM. */
    public final void removeProducer(Producer p) {
        m_producers.remove(p);
        info("removed producer");
    }

    /* Called by Felix DM. */
    public void addStatsProvider(ServiceReference<StatsProvider> serviceRef, StatsProvider provider) {
        if ("true".equals(serviceRef.getProperty("aggregator"))) {
            m_aggregator = provider;
        }
        String type = "" + serviceRef.getProperty("type"); 
        if ("processor".equalsIgnoreCase(type)) {
            m_processorCount.incrementAndGet();
        }
        else if ("producer".equalsIgnoreCase(type)) {
            m_producerCount.incrementAndGet();
        }
        m_providerStats.putIfAbsent(serviceRef, new StatsContainer(provider, new TimestampMap<Double>()));
        info("added statsprovider of type " + type);
    }

    /* Called by Felix DM. */
    public void removeStatsProvider(ServiceReference<StatsProvider> serviceRef, StatsProvider provider) {
        if ("true".equals(serviceRef.getProperty("aggregator"))) {
            m_aggregator = null;
        }
        String type = "" + serviceRef.getProperty("type"); 
        if ("processor".equalsIgnoreCase(type)) {
            m_processorCount.decrementAndGet();
        }
        else if ("producer".equalsIgnoreCase(type)) {
            m_producerCount.decrementAndGet();
        }
        m_providerStats.remove(serviceRef);
        info("added statsprovider of type " + type);
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
                Set<StatsContainer> statsContainers = new TreeSet<>();
                
    			// get all stats async with a 1 second timeout
                // prevents waiting for bigger remote service call timeouts and a hanging UI
                List<Callable<StatsContainer>> tasks = new ArrayList<>();
                List<Future<StatsContainer>> results = null;

                for (StatsContainer c : m_providerStats.values()) {
        			tasks.add(new Callable<StatsContainer>(){
        				@Override
        				public StatsContainer call() throws Exception {
        					c.updateStats(System.currentTimeMillis());
        					c.name = getName(c.m_provider);
        					return c;
        				}
        			});
            	}
				
                try {
					results = m_executor.invokeAll(tasks, 1, TimeUnit.SECONDS);
				} catch (InterruptedException e1) {
					// what can we do...?
				}
            	
				if (results != null) {
					for (Future<StatsContainer> result : results) {
						try {
							StatsContainer c = result.get();
							String name = c.name;
							if (name != null && (wantedNames.isEmpty() || wantedNames.contains(name))) {
								statsContainers.add(c);
							}
						}
						catch (Exception e) {
							// just skip this one
							warn("Failed to process provider %s", e.getMessage());
							continue;
						}
					}
					
					generator.writeStartArray();
					
					for (StatsContainer container: statsContainers) {
						try {
							StatsProvider provider = container.m_provider;
							TimestampMap<Double> stats = container.m_stats;
							String name = container.name;
							
							if (provider != null && stats != null && name != null) {
								writeAsJSON(generator, provider, stats, name);
							}
						} catch (Exception e) {
							warn("Failed to write provider to JSON: %s", e.getMessage());
						}
					}
					
					generator.writeEndArray();
				}
            } else if ("/systemStats".equals(pathInfo)) {
                resp.setStatus(HttpServletResponse.SC_OK);

                int currentProcessorReplicaCount = m_coordinator.getReplicaCount(Type.PROCESSOR);
                int currentProducerReplicaCount = m_coordinator.getReplicaCount(Type.PRODUCER);

                generator.writeStartObject();
                generator.writeNumberField("productionRate", m_productionRate.get());
                generator.writeStringField("processors", m_processorCount.get() + " / " + currentProcessorReplicaCount);
                generator.writeStringField("producers", m_producerCount.get() + " / " + currentProducerReplicaCount);
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
    	
    	String pathInfo = req.getPathInfo();
    	
    	if ("/utilisation".equals(pathInfo)) {
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
    	} else if ("/scaleProducer".equals(pathInfo)) {
            Integer relReplicaCount = getIntParameter(req, "value");
            int currentReplicaCount = m_coordinator.getReplicaCount(Type.PRODUCER);
            m_coordinator.setReplicaCount(Type.PRODUCER, currentReplicaCount + relReplicaCount);
    	}

        resp.setStatus(HttpServletResponse.SC_NO_CONTENT);
    }

    private void writeAsJSON(JsonGenerator generator, StatsProvider provider, TimestampMap<Double> stats, String name)
        throws IOException {
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
