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
package org.inaetics.demonstrator.stub.processor;

import java.util.Dictionary;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.ScheduledFuture;
import java.util.concurrent.TimeUnit;

import org.inaetics.demonstrator.api.data.Result;
import org.inaetics.demonstrator.api.data.Sample;
import org.inaetics.demonstrator.api.datastore.DataStore;
import org.inaetics.demonstrator.api.processor.Processor;
import org.inaetics.demonstrator.api.queue.SampleQueue;
import org.osgi.service.cm.ConfigurationException;
import org.osgi.service.cm.ManagedService;
import org.osgi.service.log.LogService;

/**
 * Simple {@link Processor} implementation that take the last N samples from the
 * queue and store them in a datastore.
 */
public class IdentityProcessor implements Processor, Runnable, ManagedService {

	private ScheduledExecutorService m_executor;
	private ScheduledFuture<?> m_future;

	// Injected by Felix DM...
	private volatile SampleQueue m_queue;
	private volatile DataStore m_store;
	private volatile LogService m_log;

	@Override
	public void run() {
		try {
			Sample sample = m_queue.take();
			Result result = new Result(10, 1.0, sample); // XXX
			m_store.store(result);
			
			m_log.log(LogService.LOG_INFO, "Processed: " + sample + " to " + result);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	@Override
	public void updated(Dictionary<String, ?> properties) throws ConfigurationException {
		// Nothing yet... 
	}

	/**
	 * Called by Felix DM when starting this component.
	 */
	protected void start() throws Exception {
		m_executor = Executors.newSingleThreadScheduledExecutor();
		m_future = m_executor.scheduleAtFixedRate(this, 500, 20, TimeUnit.MILLISECONDS);
	}

	/**
	 * Called by Felix DM when stopping this component.
	 */
	protected void stop() throws Exception {
		if (m_future != null) {
			m_future.cancel(true);
		}
		m_executor.shutdown();
		m_executor.awaitTermination(10, TimeUnit.SECONDS);
	}
}
