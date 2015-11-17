/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.processor;

import java.util.concurrent.Callable;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;
import java.util.concurrent.atomic.AtomicLong;

import org.inaetics.demonstrator.api.data.Result;
import org.inaetics.demonstrator.api.data.Sample;
import org.inaetics.demonstrator.api.datastore.DataStore;
import org.inaetics.demonstrator.api.processor.Processor;
import org.inaetics.demonstrator.api.queue.SampleQueue;

/**
 * Simple {@link Processor} implementation that take the last N samples from the
 * queue and store them in a datastore.
 */
public class IdentityProcessor extends AbstractSampleProcessor {
    private final AtomicLong m_processed;
    private ExecutorService m_executor;
    
    // Injected by Felix DM...
    private volatile SampleQueue m_queue;
    private volatile DataStore m_store;

    public IdentityProcessor() {
        super("Identity processor", 3 /* msec */);
        m_processed = new AtomicLong(0L);
    }

    @Override
    public void processSampleData() {
    	Future<Void> result = null;
    	try {
    		//prevent waiting longer than 1 second
    		result = m_executor.submit(new Callable<Void>() {
    			@Override
    			public Void call() throws Exception {
    				Sample sample = m_queue.take();
    				Result result = new Result(10, 1.0, sample);
    				m_store.store(result);
    				return null;
    			}
    		});
			result.get(500, TimeUnit.MILLISECONDS);
			m_processed.incrementAndGet();
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
    protected long getProcessedCount() {
        return m_processed.getAndSet(0);
    }
    
    @Override
    protected void start() throws Exception {
        m_executor = Executors.newSingleThreadScheduledExecutor();
    	super.start();
    }

    @Override
    protected void stop() throws Exception {
    	super.stop();
    	if (m_executor != null && !m_executor.isShutdown()) {
    		m_executor.shutdownNow();
    	}
    }
}
