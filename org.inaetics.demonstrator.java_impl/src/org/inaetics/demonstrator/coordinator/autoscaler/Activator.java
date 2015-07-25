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

import java.util.Properties;

import org.apache.felix.dm.DependencyActivatorBase;
import org.apache.felix.dm.DependencyManager;
import org.inaetics.demonstrator.api.coordinator.CoordinatorService;
import org.inaetics.demonstrator.api.stats.StatsProvider;
import org.osgi.framework.BundleContext;
import org.osgi.framework.Constants;
import org.osgi.service.log.LogService;

public class Activator extends DependencyActivatorBase {
    private static final String PID = "org.inaetics.demonstrator.coordinator.autoscalar.simple";

    @Override
    public void init(BundleContext context, DependencyManager manager) throws Exception {
        Properties props = new Properties();
        props.put(Constants.SERVICE_PID, PID);

        manager.add(createComponent()
            .setImplementation(AutoScalerImpl.class)
            .add(createServiceDependency().setService(CoordinatorService.class).setRequired(true))
            .add(createServiceDependency().setService(StatsProvider.class, "(type=queue)").setRequired(true))
            .add(createServiceDependency().setService(LogService.class).setRequired(false)));
    }
}
