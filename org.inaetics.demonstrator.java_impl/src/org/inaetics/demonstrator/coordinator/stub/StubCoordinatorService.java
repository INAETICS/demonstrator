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
package org.inaetics.demonstrator.coordinator.stub;

import java.util.ArrayList;
import java.util.Dictionary;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Random;
import java.util.function.Supplier;

import org.inaetics.demonstrator.api.coordinator.CoordinatorService;
import org.inaetics.demonstrator.coordinator.util.CoordinatorConfig;
import org.osgi.framework.BundleActivator;
import org.osgi.framework.BundleContext;
import org.osgi.service.cm.ConfigurationException;
import org.osgi.service.cm.ManagedService;
import org.osgi.service.log.LogService;

/**
 * Provides a coordinator that simply starts/stops new producers/processors directly in an OSGi environment
 */
public class StubCoordinatorService implements ManagedService, CoordinatorService {
    private final Map<Type, List<BundleActivator>> m_services;
    private final Random m_prng;
    // Managed by Felix DM...
    private volatile LogService m_log;
    private volatile BundleContext m_context;
    // Locally managed...
    private volatile CoordinatorConfig m_config;

    public StubCoordinatorService() {
        this(new CoordinatorConfig());
    }

    public StubCoordinatorService(CoordinatorConfig config) {
        m_config = config;
        m_services = new HashMap<>();
        m_prng = new Random();
    }

    @Override
    public int getMaxReplicaCount(Type type) {
        CoordinatorConfig config = m_config;
        switch (type) {
            case PROCESSOR:
                return config.getMaxNrProcessors();
            case PRODUCER:
                return config.getMaxNrProducers();
            default:
                throw new IllegalArgumentException("Invalid type: " + type);
        }
    }

    @Override
    public int getReplicaCount(Type type) {
        List<BundleActivator> comp;
        switch (type) {
            case PROCESSOR:
            case PRODUCER:
                comp = m_services.computeIfAbsent(type, k -> new ArrayList<BundleActivator>());
                break;
            default:
                throw new IllegalArgumentException("Invalid type: " + type);
        }
        return comp.size();
    }

    @Override
    public boolean setReplicaCount(Type type, int number) {
        CoordinatorConfig config = m_config;

        List<BundleActivator> comp;
        switch (type) {
            case PROCESSOR:
                comp = m_services.computeIfAbsent(type, k -> new ArrayList<BundleActivator>());
                if (number >= 0 && number <= config.getMaxNrProcessors() && number != comp.size()) {
                    updateServices(type, comp, number - comp.size(),
                        () -> new org.inaetics.demonstrator.processor.Activator());
                    return true;
                } else {
                    return false;
                }

            case PRODUCER:
                comp = m_services.computeIfAbsent(type, k -> new ArrayList<BundleActivator>());
                if (number >= 0 && number <= config.getMaxNrProducers() && number != comp.size()) {
                    Supplier<BundleActivator> supplier;
                    if (m_prng.nextBoolean()) {
                        supplier = () -> new org.inaetics.demonstrator.producer.periodic.Activator();
                    } else {
                        supplier = () -> new org.inaetics.demonstrator.producer.burst.Activator();
                    }
                    updateServices(type, comp, number - comp.size(), supplier);
                    return true;
                } else {
                    return false;
                }
            default:
                throw new IllegalArgumentException("Invalid type: " + type);
        }
    }

    @Override
    public void updated(Dictionary<String, ?> properties) throws ConfigurationException {
        if (properties != null) {
            m_config = new CoordinatorConfig(properties);
        } else {
            m_config = new CoordinatorConfig();
        }
    }

    private <T extends BundleActivator> void updateServices(Type type, List<BundleActivator> comps, int replicas,
        Supplier<T> s) {
        if (replicas < 0) {
            // Cull the number of services to the requested number...
            while (!comps.isEmpty() && replicas++ < 0) {
                try {
                    comps.remove(0).stop(m_context);

                    m_log.log(LogService.LOG_INFO, "Removed replica of type " + type + " (replicas: " + comps.size() + ")");
                } catch (Exception e) {
                    m_log.log(LogService.LOG_WARNING, "Removal of replica of type " + type + " failed!", e);
                }
            }
        } else if (replicas > 0) {
            while (replicas-- > 0) {
                try {
                    T ba = s.get();
                    ba.start(m_context);

                    if (comps.add(ba)) {
                        m_log.log(LogService.LOG_INFO, "Added replica of type " + type + " (replicas: " + comps.size() + ")");
                    }
                } catch (Exception e) {
                    m_log.log(LogService.LOG_WARNING, "Addition of replica of type " + type + " failed!", e);
                }
            }
        }
    }
}
