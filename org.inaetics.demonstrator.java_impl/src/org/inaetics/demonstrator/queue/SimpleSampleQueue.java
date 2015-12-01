/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.queue;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Dictionary;
import java.util.List;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;

import org.inaetics.demonstrator.api.data.Sample;
import org.inaetics.demonstrator.api.queue.SampleQueue;
import org.inaetics.demonstrator.api.stats.StatsProvider;
import org.osgi.service.cm.ConfigurationException;
import org.osgi.service.cm.ManagedService;
import org.osgi.service.log.LogService;

public class SimpleSampleQueue implements SampleQueue, StatsProvider, ManagedService {
    private static final int MAX_QUEUE_SIZE = 10 * 1000;
    private static final int SLEEP_IN_MS = 5000;

    private final BlockingQueue<Sample> m_queue;
    private final Thread m_printThread;

    // Injected by Felix DM...
    private volatile LogService m_log;

    public SimpleSampleQueue() {
        m_queue = new LinkedBlockingQueue<>(MAX_QUEUE_SIZE);
        m_printThread = new Thread(new Runnable() {
			@Override
			public void run() {
				while (!Thread.interrupted()) {
					try {
						printStatus();
						Thread.sleep(SLEEP_IN_MS);
					} catch (InterruptedException e) {
						Thread.currentThread().interrupt();
					}
				}
			}
        }, "queue-print");
    }
    
    public void start() {
    	m_printThread.start();
    }
    
    public void stop() {
    	m_printThread.interrupt();
    }
    
    private void printStatus() {
    	m_log.log(LogService.LOG_INFO, "Queue size : " + m_queue.size());
    }

    @Override
    public String getName() {
        return "Queue";
    }

    @Override
    public String getMeasurementUnit() {
        return "%";
    }

    @Override
    public String getType() {
        return "utilization";
    }

    @Override
    public double getValue() {
        return (100.0 * m_queue.size()) / MAX_QUEUE_SIZE;
    }

    @Override
    public boolean put(Sample sample) {
        try {
            m_queue.put(sample);
            return true;
        } catch (InterruptedException e) {
            return false;
        }
    }

    @Override
    public int putAll(Collection<Sample> samples) {
        int size = 0;
        for (Sample sample : samples) {
            if (put(sample)) {
            	size++;
            };
        }
        return size;
    }

    @Override
    public Sample take() {
        try {
            Sample result = m_queue.take();

            return result;
        } catch (InterruptedException e) {
            return null;
        }
    }

    @Override
    public Collection<Sample> takeMinMax(int min, int max) {
        List<Sample> result = new ArrayList<>(max);
        // NOTE: this is not strictly conform our contract...
        m_queue.drainTo(result, max);

        return result;
    }

    @Override
    public void updated(Dictionary<String, ?> properties) throws ConfigurationException {
        // Nothing yet...
        m_log.log(LogService.LOG_INFO, "SimpleSampleQueue updated...");
    }
}
