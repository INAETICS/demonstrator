/*
 * Copyright (c) 2015 INAETICS, <www.inaetics.org>
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
package org.inaetics.demonstrator.coordinator.autoscaler;

import java.util.Dictionary;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;

import org.apache.felix.dm.Component;
import org.inaetics.demonstrator.api.coordinator.CoordinatorService;
import org.inaetics.demonstrator.api.coordinator.CoordinatorService.Type;
import org.inaetics.demonstrator.api.processor.Processor;
import org.inaetics.demonstrator.api.stats.StatsProvider;
import org.osgi.service.cm.ConfigurationException;
import org.osgi.service.cm.ManagedService;
import org.osgi.service.log.LogService;

/**
 * Provides an auto-scaler functionality for {@link Processor}s.
 */
public class AutoScalerImpl implements ManagedService {
    // Managed by Felix DM...
    private volatile StatsProvider m_queueStatsProvider;
    private volatile CoordinatorService m_coordinator;
    private volatile LogService m_log;
    // Locally managed...
    private volatile ExecutorService m_executor;
    private volatile AutoScalerConfig m_config;
    private volatile Future<?> m_pollFuture;

    private volatile double m_oldValue;
    private volatile double m_slope;

    public AutoScalerImpl() {
        this(new AutoScalerConfig());
    }

    public AutoScalerImpl(AutoScalerConfig config) {
        m_config = config;
        m_slope = 0.0;
        m_oldValue = 0.0;
    }

    @Override
    public void updated(Dictionary<String, ?> properties) throws ConfigurationException {
        if (properties != null) {
            m_config = new AutoScalerConfig(properties);
        } else {
            m_config = new AutoScalerConfig();
        }
    }

    final void checkQueue() {
        AutoScalerConfig config = m_config;

        double value = m_queueStatsProvider.getValue();

        m_slope = (value - m_oldValue) / config.getPollInterval();
        m_oldValue = value;

        info("Current queue utilization: %.3f %%, configured boundaries: [%d..%d], slope = %f.", value,
            m_config.getQueueLowBarrier(), m_config.getQueueHighBarrier(), m_slope);

        if (m_slope > 1.0) {
            // If our slope is > 1.0, then our producers are producing far more items than we can process, so let's counter this by adding 2 new processors;
            if (scale(Type.PROCESSOR, +2)) {
                info("Producing too many items than is processed, added 2 new processors...");
            } else {
                warn("System is overloaded, but no resources are available to compensate!");
            }
        } else if (m_slope > 0.5) {
            // If our slope is > 0.5 && <= 1.0, then our producers are producing more items than we can process, add 1 new processor;
            if (scale(Type.PROCESSOR, +1)) {
                info("Producing more items than is processed, added 1 new processor...");
            } else {
                warn("System is overloaded, but no resources are available to compensate!");
            }
        } else if (m_slope > -0.5) {
            // If our slope is > -0.5 && <= 0.5, then our producers are producing roughly enough items,
            // check whether we're in the desired utilization of the queue...
            if (value < config.getQueueLowBarrier()) {
                info("Queue is under utilized, trying to decrease the processor count.");

                scale(Type.PROCESSOR, -1);
            } else if (value > config.getQueueHighBarrier()) {
                info("Queue is over utilized, trying to increase the processor count.");

                scale(Type.PROCESSOR, +1);
            } else {
                info("Producing roughly the same number of items as processed, nothing needs to be done...");
            }
        } else if (m_slope > -1.0) {
            // If our slope is > -1.0 && <= -0.5, then our processors are processing more than produces, add 1 new producer;
            if (scale(Type.PROCESSOR, -1)) {
                info("Producing too little items than processed, removing 1 processor...");
            } else {
                warn("System is overloaded, but no resources are available to compensate!");
            }
        } else {
            // If our slope is <= -1.0, then we need to produce far more items...
            if (scale(Type.PROCESSOR, -2)) {
                info("Producing far too little items than processed, removing 2 processors...");
            } else {
                warn("System is overloaded, but no resources are available to compensate!");
            }
        }
    }

    private boolean scale(Type type, int relReplicas) {
        int current = m_coordinator.getReplicaCount(type);
        return m_coordinator.setReplicaCount(type, current + relReplicas);
    }

    /**
     * Called by Felix DM when this component is started.
     */
    protected final void start(Component comp) {
        m_executor = Executors.newSingleThreadExecutor();

        m_pollFuture = m_executor.submit(() -> {
            int ival = m_config.getPollInterval() / 2;
            while (!Thread.currentThread().isInterrupted() && !m_executor.isShutdown()) {
                try {
                    TimeUnit.SECONDS.sleep(ival);

                    checkQueue();

                    ival = m_config.getPollInterval();
                } catch (Exception e) {
                    if (e instanceof InterruptedException) {
                        m_log.log(LogService.LOG_INFO, "Got interrupted while polling the queue, going down!", e);
                        Thread.currentThread().interrupt();
                    } else {
                        m_log.log(LogService.LOG_WARNING, "Unhandled exception while polling the queue!", e);
                    }
                }
            }
        });

        // TODO
        m_coordinator.setReplicaCount(Type.PRODUCER, 1);
//        m_coordinator.setReplicaCount(Type.PROCESSOR, 1);
    }

    /**
     * Called by Felix DM when this component is stopped.
     */
    protected final void stop(Component comp) throws Exception {
        try {
            if (m_pollFuture != null) {
                m_pollFuture.cancel(true);
            }
            if (m_executor != null) {
                m_executor.shutdown();
                m_executor.awaitTermination(5, TimeUnit.SECONDS);
            }
        } finally {
            m_pollFuture = null;
            m_executor = null;
        }
    }

    protected final void info(String msg, Object... args) {
        m_log.log(LogService.LOG_INFO, String.format(msg, args));
    }

    protected final void warn(String msg, Object... args) {
        m_log.log(LogService.LOG_WARNING, String.format(msg, args));
    }
}
