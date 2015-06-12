/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.stub.producer;

import java.util.Dictionary;
import java.util.Random;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;

import org.apache.felix.dm.Component;
import org.inaetics.demonstrator.api.producer.Producer;
import org.inaetics.demonstrator.api.stats.StatsProvider;
import org.osgi.framework.Constants;
import org.osgi.service.cm.ConfigurationException;
import org.osgi.service.cm.ManagedService;
import org.osgi.service.log.LogService;

/**
 * Abstract base implementation of a sample {@link Producer}.
 */
public abstract class AbstractSampleProducer implements Producer, StatsProvider, ManagedService {
    private final Random m_rnd;
    private final String m_name;
    private final int m_minTaskInterval;

    private ExecutorService m_executor;
    private Future<?> m_generatorFuture;
    private Future<?> m_samplerFuture;

    private volatile double m_producedAvg;
    private volatile int m_taskInterval;

    // Injected by Felix DM...
    private volatile Component m_component;
    private volatile LogService m_log;

    /**
     * @param name the name of this producer;
     * @param taskInterval the initial interval (in milliseconds) to wait between two produced samples;
     * @param minTaskInterval the minimal interval (in milliseconds) to wait between two produced samples.
     */
    protected AbstractSampleProducer(String name, int taskInterval, int minTaskInterval) {
        m_name = name;
        m_taskInterval = taskInterval;
        m_minTaskInterval = minTaskInterval;

        m_rnd = new Random();
        m_producedAvg = 0;
    }

    @Override
    public final int getMaxSampleRate() {
        return (1000 / m_minTaskInterval);
    }

    @Override
    public String getMeasurementUnit() {
        return "samples/sec";
    }

    @Override
    public final String getName() {
        Long id = (Long) m_component.getServiceRegistration().getReference().getProperty(Constants.SERVICE_ID);
        return String.format("%s #%d", m_name, id);
    }

    @Override
    public int getSampleRate() {
        return 1000 / m_taskInterval;
    }

    @Override
    public String getType() {
        return "throughput";
    }

    @Override
    public double getValue() {
        return m_producedAvg;
    }

    @Override
    public void setSampleRate(int rate) {
        if (rate < m_minTaskInterval) {
            throw new IllegalArgumentException("Invalid sample rate!");
        }
        m_taskInterval = 1000 / rate;
    }

    @Override
    public void updated(Dictionary<String, ?> properties) throws ConfigurationException {
        // Nothing yet...
    }

    final int getTaskInterval() {
        return m_taskInterval;
    }

    /**
     * @param time the time (in milliseconds) this producer is running.
     * @return the throughput of this producer.
     */
    protected abstract double calculateThroughput(long time);

    protected final void info(String msg, Object... args) {
        m_log.log(LogService.LOG_INFO, String.format(msg, args));
    }

    protected final void msleep(long time) {
        try {
            TimeUnit.MILLISECONDS.sleep(time);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }

    /**
     * Implement this to produce the actual sample(s).s
     */
    protected abstract void produceSampleData();

    protected final double randomSampleValue() {
        return (m_rnd.nextDouble() * 200.0) - 100.0;
    }

    /**
     * Called by Felix DM when starting this component.
     */
    protected final void start() throws Exception {
        m_executor = Executors.newFixedThreadPool(2);

        m_generatorFuture = m_executor.submit(new Runnable() {
            @Override
            public void run() {
                while (!Thread.currentThread().isInterrupted()) {
                    try {
                        produceSampleData();

                        TimeUnit.MILLISECONDS.sleep(getTaskInterval());
                    } catch (InterruptedException e) {
                        // Break out of our loop...
                        Thread.currentThread().interrupt();
                    } catch (Exception e) {
                        // Ignore, not much we can do about this...
                        info("Failed to produce sample(s)! Cause: %s", (e.getMessage() == null ? "NullPointerException"
                            : e.getMessage()));
                        m_log.log(LogService.LOG_DEBUG, "Failed to produce sample(s)!", e);
                    }
                }
            }
        });

        m_samplerFuture = m_executor.submit(new Runnable() {
            @Override
            public void run() {
                long startTime = System.currentTimeMillis();

                while (!Thread.currentThread().isInterrupted()) {
                    try {
                        TimeUnit.SECONDS.sleep(1);

                        m_producedAvg = calculateThroughput(System.currentTimeMillis() - startTime);
                    } catch (InterruptedException e) {
                        // Break out of our loop...
                        Thread.currentThread().interrupt();
                    } catch (Exception e) {
                        m_log.log(LogService.LOG_DEBUG, "Failed to calculate average throughput!", e);
                    }
                }
            }
        });
    }

    /**
     * Called by Felix DM when stopping this component.
     */
    protected final void stop() throws Exception {
        if (m_generatorFuture != null) {
            m_generatorFuture.cancel(true);
            m_generatorFuture = null;
        }
        if (m_samplerFuture != null) {
            m_samplerFuture.cancel(true);
            m_samplerFuture = null;
        }
        m_executor.shutdown();
        m_executor.awaitTermination(10, TimeUnit.SECONDS);
    }
}
