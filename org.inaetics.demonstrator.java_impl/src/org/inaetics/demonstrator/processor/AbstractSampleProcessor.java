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
    private Future<?> m_samplerFuture;

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
     * @param time the time (in milliseconds) this processor is running.
     * @return the throughput of this processor.
     */
    protected abstract double calculateThroughput(long time);

    protected final void info(String msg, Object... args) {
        m_log.log(LogService.LOG_INFO, String.format(msg, args));
    }

    /**
     * Implement this to process the actual sample(s).s
     */
    protected abstract void processSamples();

    protected final double randomSampleValue() {
        return (m_rnd.nextDouble() * 200.0) - 100.0;
    }

    /**
     * Called by Felix DM when starting this component.
     */
    protected final void start() throws Exception {
        m_executor = Executors.newFixedThreadPool(2);

        m_generatorFuture =
            m_executor.submit(() -> //
                {
                    while (!Thread.currentThread().isInterrupted() && !m_executor.isShutdown()) {
                        try {
                            processSamples();

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

        m_samplerFuture = m_executor.submit(() -> //
            {
                while (!Thread.currentThread().isInterrupted() && !m_executor.isShutdown()) {
                    try {
                    	int sleepDuration = 1;
                        TimeUnit.SECONDS.sleep(sleepDuration);

                        m_processedAvg = calculateThroughput(sleepDuration * 1000);
                    } catch (InterruptedException e) {
                        // Break out of our loop...
                        Thread.currentThread().interrupt();
                    } catch (Exception e) {
                        m_log.log(LogService.LOG_DEBUG, "Failed to calculate average throughput!", e);
                    }
                }
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
        if (m_samplerFuture != null) {
            m_samplerFuture.cancel(true);
            m_samplerFuture = null;
        }
        m_executor.shutdown();
        m_executor.awaitTermination(10, TimeUnit.SECONDS);
        
        info("Processor %s stopped...", getName());
    }
}
