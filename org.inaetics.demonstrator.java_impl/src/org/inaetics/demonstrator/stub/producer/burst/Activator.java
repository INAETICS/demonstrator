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
package org.inaetics.demonstrator.stub.producer.burst;

import java.util.Properties;

import org.apache.felix.dm.DependencyActivatorBase;
import org.apache.felix.dm.DependencyManager;
import org.inaetics.demonstrator.api.producer.Producer;
import org.inaetics.demonstrator.api.queue.SampleQueue;
import org.inaetics.demonstrator.api.stats.StatsProvider;
import org.osgi.framework.BundleContext;
import org.osgi.framework.Constants;
import org.osgi.service.cm.ManagedService;
import org.osgi.service.log.LogService;
import org.osgi.service.remoteserviceadmin.RemoteConstants;

public class Activator extends DependencyActivatorBase {
	private static final String PID = "BurstSampleProducer";

	@Override
	public void init(BundleContext context, DependencyManager manager) throws Exception {
		String[] ifaces = { Producer.class.getName(), ManagedService.class.getName() };
		
		Properties props = new Properties();
		props.put(Constants.SERVICE_PID, PID);
		props.put(RemoteConstants.SERVICE_EXPORTED_INTERFACES, ifaces[0]);
		props.put("type", "burst");
		
		Producer service = new BurstSampleProducer();

		manager.add(createComponent()
			.setInterface(ifaces, props)
			.setImplementation(service)
			.add(createServiceDependency().setService(SampleQueue.class).setRequired(true))
			.add(createServiceDependency().setService(LogService.class).setRequired(false))
		);

        /*
         * IMPORTANT: We do not want a direct coupling between the DataStore and StatsProvider,
         * hence we create a singleton service that is registered as two *different* services.
         */
        props = new Properties();
        props.put(RemoteConstants.SERVICE_EXPORTED_INTERFACES, StatsProvider.class.getName());

        manager.add(createComponent()
            .setInterface(StatsProvider.class.getName(), props)
            .setImplementation(service)
        );
	}
}
