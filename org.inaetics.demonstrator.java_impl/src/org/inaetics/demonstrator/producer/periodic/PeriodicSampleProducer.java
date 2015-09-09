/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.producer.periodic;

import java.util.concurrent.Callable;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;
import java.util.concurrent.atomic.AtomicLong;

import org.inaetics.demonstrator.api.data.Sample;
import org.inaetics.demonstrator.api.queue.SampleQueue;
import org.inaetics.demonstrator.producer.AbstractSampleProducer;

/**
 * Generates a random sample once every 10 ms.
 */
public class PeriodicSampleProducer extends AbstractSampleProducer {
    private final AtomicLong m_produced;
    private final ScheduledExecutorService m_executor;

    // Injected by Felix DM...
    private volatile SampleQueue m_queue;

    public PeriodicSampleProducer() {
        super("Periodic Sample Producer", 10000 /* msec */, 1000 /* msec */);
        m_produced = new AtomicLong(0L);
        m_executor = Executors.newSingleThreadScheduledExecutor();
    }

    @Override
    protected long getProductionCount() {
        return m_produced.getAndSet(0);
    }

    @Override
    protected void produceSampleData() {
    	Future<Void> result = null;
    	try {
    		result = m_executor.submit(new Callable<Void>() {
    			@Override
    			public Void call() throws Exception {
    				double val1 = randomSampleValue();
    				double val2 = randomSampleValue();
    				Sample sample = new Sample(System.currentTimeMillis(), val1, val2);
    				m_queue.put(sample);
    				return null;
    			}
    		});
			result.get(500, TimeUnit.MILLISECONDS);
			m_produced.incrementAndGet();
		} catch (TimeoutException e) {
			// do not wait any longer
			if (result != null && !result.isDone()) {
				result.cancel(true);
			}
		} catch (Exception e) {
			// nothing we can do
		}
    }
    
    @Override
    protected void cleanup() {
    	if (m_executor != null && !m_executor.isShutdown()) {
    		m_executor.shutdownNow();
    	}
    }
}
