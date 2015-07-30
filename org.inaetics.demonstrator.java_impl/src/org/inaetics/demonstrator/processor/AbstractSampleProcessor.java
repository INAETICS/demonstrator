/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.processor;

import java.util.Random;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;

import org.inaetics.demonstrator.api.processor.Processor;
import org.inaetics.demonstrator.api.producer.Producer;
import org.inaetics.demonstrator.api.stats.StatsProvider;
import org.osgi.service.log.LogService;

/**
 * Abstract base implementation of a sample {@link Producer}.
 */
public abstract class AbstractSampleProcessor implements Processor, StatsProvider {
    private final String m_name;
    private final Random m_rnd;
    private final int m_taskInterval;

    private ExecutorService m_executor;
    private Future<?> m_generatorFuture;

    private volatile double m_processedAvg = 0;
    // Injected by Felix DM...
    private volatile LogService m_log;

    protected AbstractSampleProcessor(String name, int taskInterval) {
        m_name = String.format("%s (%x)", name, System.identityHashCode(this));
        m_rnd = new Random();
        m_taskInterval = taskInterval;
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
    public String getType() {
        return "throughput";
    }

    @Override
    public double getValue() {
        return m_processedAvg;
    }

    final int getTaskInterval() {
        return m_taskInterval;
    }

    /**
     * @return how many items up to now were produced.
     */
    protected abstract long getProcessedCount();

    protected final void info(String msg, Object... args) {
        m_log.log(LogService.LOG_INFO, String.format(msg, args));
    }

    /**
     * Implement this to process the actual sample(s).s
     */
    protected abstract void processSampleData();

    protected final double randomSampleValue() {
        return (m_rnd.nextDouble() * 200.0) - 100.0;
    }

    /**
     * Called by Felix DM when starting this component.
     */
    protected final void start() throws Exception {
        m_executor = Executors.newSingleThreadExecutor();

        m_generatorFuture = m_executor.submit(() -> {
            long oldProcessedCount = 0L;
            while (!Thread.currentThread().isInterrupted() && !m_executor.isShutdown()) {
                try {
                    processSampleData();

                    long processedCount = getProcessedCount();
                    m_processedAvg = (1000.0 * (processedCount - oldProcessedCount)) / getTaskInterval();
                    oldProcessedCount = processedCount;

                    TimeUnit.MILLISECONDS.sleep(getTaskInterval());
                } catch (InterruptedException e) {
                    // Break out of our loop...
                    Thread.currentThread().interrupt();
                } catch (Exception e) {
                    // Ignore, not much we can do about this...
                    info("Failed to process sample(s)! Cause: %s",
                        (e.getMessage() == null ? "NullPointerException" : e.getMessage()));
                    m_log.log(LogService.LOG_DEBUG, "Failed to process sample(s)!", e);
                }
            }
            ;
        });

        info("Processor %s started...", getName());
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
        
        info("Processor %s stopped...", getName());
    }
}
