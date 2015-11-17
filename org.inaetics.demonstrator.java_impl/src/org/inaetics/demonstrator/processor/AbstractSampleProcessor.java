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

    private ExecutorService m_processExecutor;
    private ExecutorService m_avgExecutor;
    private Future<?> m_processFuture;
    private Future<?> m_avgFuture;

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
    protected void start() throws Exception {
        // start calculating avg
        m_avgExecutor = Executors.newSingleThreadExecutor();
        m_avgFuture = m_avgExecutor.submit(() -> {
            long oldTimeMillis = System.currentTimeMillis();
            while (!Thread.currentThread().isInterrupted()) {
                try {
                    long processedCount = getProcessedCount();
                    long currentTimeMillis = System.currentTimeMillis();
                    m_processedAvg = (1000.0 * processedCount) / (currentTimeMillis - oldTimeMillis);
                    oldTimeMillis = currentTimeMillis;

                    TimeUnit.MILLISECONDS.sleep(900);
                } catch (InterruptedException e) {
                    // Break out of our loop...
                    Thread.currentThread().interrupt();
                } catch (Exception e) {
                    // Ignore, not much we can do about this...
                    info("Failed to process sample(s)! Cause: %s",
                        (e.getMessage() == null ? "NullPointerException" : e.getMessage()));
                }
            }
        });
        
    	// start producing
    	m_processExecutor = Executors.newSingleThreadExecutor();
        m_processFuture = m_processExecutor.submit(() -> {
            while (!Thread.currentThread().isInterrupted() && !m_processExecutor.isShutdown()) {
                try {
                    processSampleData();
                    TimeUnit.MILLISECONDS.sleep(getTaskInterval());
                } catch (InterruptedException e) {
                    // Break out of our loop...
                    Thread.currentThread().interrupt();
                } catch (Exception e) {
                    // Ignore, not much we can do about this...
                    info("Failed to process sample(s)! Cause: %s",
                        (e.getMessage() == null ? "NullPointerException" : e.getMessage()));
                }
            }
        });
        info("Processor %s started...", getName());
    }

    /**
     * Called by Felix DM when stopping this component.
     */
    protected void stop() throws Exception {
        if (m_processFuture != null) {
            m_processFuture.cancel(true);
            m_processFuture = null;
        }
        m_processExecutor.shutdown();
        
        if (m_avgFuture != null) {
        	m_avgFuture.cancel(true);
        	m_avgFuture = null;
        }
        m_avgExecutor.shutdown();

        m_processedAvg = 0;
        
        m_processExecutor.awaitTermination(10, TimeUnit.SECONDS);
        m_avgExecutor.awaitTermination(10, TimeUnit.SECONDS);
        
        info("Processor %s stopped...", getName());
    }
}