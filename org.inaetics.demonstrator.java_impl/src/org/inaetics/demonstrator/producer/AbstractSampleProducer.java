/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.producer;

import java.util.Random;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;

import org.inaetics.demonstrator.api.producer.Producer;
import org.inaetics.demonstrator.api.stats.StatsProvider;
import org.osgi.service.log.LogService;

/**
 * Abstract base implementation of a sample {@link Producer}.
 */
public abstract class AbstractSampleProducer implements Producer, StatsProvider {
    private final Random m_rnd;
    private final String m_name;
    private final int m_minTaskInterval;

    private ExecutorService m_executor;
    private Future<?> m_generatorFuture;

    private volatile double m_producedAvg;
    private volatile int m_taskInterval;

    // Injected by Felix DM...
    private volatile LogService m_log;

    /**
     * @param name the name of this producer;
     * @param taskInterval the initial interval (in milliseconds) to wait between two produced samples;
     * @param minTaskInterval the minimal interval (in milliseconds) to wait between two produced samples.
     */
    protected AbstractSampleProducer(String name, int taskInterval, int minTaskInterval) {
        m_name = String.format("%s (%x)", name, System.identityHashCode(this));
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
        return m_name;
    }

    @Override
    public int getSampleRate() {
        if (m_taskInterval < m_minTaskInterval) {
            return 0;
        }
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
        // In case we should stop producing samples (rate = 0), use a 
        // taskInterval outside the valid range to detect this...
        m_taskInterval = (rate == 0) ? (m_minTaskInterval - 1) : (1000 / rate);
    }

    /**
     * @return how many items up to now were produced.
     */
    protected abstract long getProductionCount();

    protected final int getTaskInterval() {
        return m_taskInterval;
    }
    
    protected final int getMinTaskInterval() {
        return m_minTaskInterval;
    }

    protected final void info(String msg, Object... args) {
        m_log.log(LogService.LOG_INFO, String.format(msg, args));
    }

    /**
     * Implement this to produce the actual sample(s).s
     */
    protected abstract void produceSampleData() throws InterruptedException;

    protected final double randomSampleValue() {
        return (m_rnd.nextDouble() * 200.0) - 100.0;
    }

    /**
     * Called by Felix DM when starting this component.
     */
    protected final void start() throws Exception {
        m_executor = Executors.newSingleThreadExecutor();

        m_generatorFuture = m_executor.submit(() -> {
            long oldProductionCount = 0L;
            long oldTime = System.currentTimeMillis();

            while (!Thread.currentThread().isInterrupted()) {
                try {
                    int taskInterval = getTaskInterval();
                    if (taskInterval >= m_minTaskInterval) {
                        produceSampleData();
                    }

                    long produced = getProductionCount();
                    long time = System.currentTimeMillis();
                    m_producedAvg = (1000.0 * (produced - oldProductionCount)) / (time - oldTime);
                    oldProductionCount = produced;
                    oldTime = time;

                    // Make sure we sleep at least 1 millisecond, even when our taskInterval is zero or negative... 
                    TimeUnit.MILLISECONDS.sleep(Math.max(1, taskInterval));
                } catch (InterruptedException e) {
                    // Break out of our loop...
                    Thread.currentThread().interrupt();
                } catch (Exception e) {
                    // Ignore, not much we can do about this...
                    info("Failed to produce sample(s)! Cause: %s",
                        (e.getMessage() == null ? "NullPointerException" : e.getMessage()));
                    m_log.log(LogService.LOG_DEBUG, "Failed to produce sample(s)!", e);
                }
            }
        });

        info("Producer %s started...", getName());
    }

    /**
     * Called by Felix DM when stopping this component.
     */
    protected final void stop() throws Exception {
        if (m_generatorFuture != null) {
            m_generatorFuture.cancel(true);
            m_generatorFuture = null;
        }
        m_executor.shutdown();
        m_executor.awaitTermination(10, TimeUnit.SECONDS);

        info("Producer %s stopped...", getName());
    }
}
