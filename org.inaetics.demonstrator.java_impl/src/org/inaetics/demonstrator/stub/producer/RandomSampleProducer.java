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
package org.inaetics.demonstrator.stub.producer;

import java.util.Dictionary;
import java.util.Random;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.ScheduledFuture;
import java.util.concurrent.TimeUnit;

import org.inaetics.demonstrator.api.data.Sample;
import org.inaetics.demonstrator.api.producer.Producer;
import org.inaetics.demonstrator.api.queue.SampleQueue;
import org.osgi.service.cm.ConfigurationException;
import org.osgi.service.cm.ManagedService;
import org.osgi.service.log.LogService;

/**
 * Generates a random sample once every 10 ms.
 */
public class RandomSampleProducer implements Producer, Runnable, ManagedService {
	private final Random m_rnd = new Random();

	private ScheduledExecutorService m_executor;
	private ScheduledFuture<?> m_future;

	// Injected by Felix DM...
	private volatile SampleQueue m_queue;
	private volatile LogService m_log;

	@Override
	public void run() {
		try {
			double val1 = randomSampleValue();
			double val2 = randomSampleValue();
			Sample sample = new Sample(System.currentTimeMillis(), val1, val2);

			m_queue.put(sample);

			m_log.log(LogService.LOG_INFO, "Produced: " + sample);
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
		m_future = m_executor.scheduleAtFixedRate(this, 500, 10, TimeUnit.MILLISECONDS);
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

	private double randomSampleValue() {
		return (m_rnd.nextDouble() * 200.0) - 100.0;
	}
}