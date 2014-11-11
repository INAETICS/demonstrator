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

import java.util.Properties;

import org.apache.felix.dm.DependencyActivatorBase;
import org.apache.felix.dm.DependencyManager;
import org.inaetics.demonstrator.api.datastore.DataStore;
import org.inaetics.demonstrator.api.processor.Processor;
import org.inaetics.demonstrator.api.queue.SampleQueue;
import org.osgi.framework.BundleContext;
import org.osgi.framework.Constants;
import org.osgi.service.cm.ManagedService;
import org.osgi.service.log.LogService;
import org.osgi.service.remoteserviceadmin.RemoteConstants;

public class Activator extends DependencyActivatorBase {
	private static final String PID = "IdentityProcessor";

	@Override
	public void init(BundleContext context, DependencyManager manager) throws Exception {
		String[] ifaces = { Processor.class.getName(), ManagedService.class.getName() };
		
		Properties props = new Properties();
		props.put(Constants.SERVICE_PID, PID);
		props.put(RemoteConstants.SERVICE_EXPORTED_INTERFACES, ifaces[0]);

		manager.add(createComponent()
			.setInterface(ifaces, props)
			.setImplementation(IdentityProcessor.class)
			.add(createServiceDependency().setService(DataStore.class).setRequired(true))
			.add(createServiceDependency().setService(SampleQueue.class).setRequired(true))
			.add(createServiceDependency().setService(LogService.class).setRequired(false))
		);
	}
}
