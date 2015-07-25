/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.view.endpoint;

import java.util.Properties;

import javax.servlet.Servlet;

import org.apache.felix.dm.DependencyActivatorBase;
import org.apache.felix.dm.DependencyManager;
import org.inaetics.demonstrator.api.processor.Processor;
import org.inaetics.demonstrator.api.producer.Producer;
import org.inaetics.demonstrator.api.stats.StatsProvider;
import org.osgi.framework.BundleContext;
import org.osgi.service.log.LogService;

public class Activator extends DependencyActivatorBase {

    @Override
    public void destroy(BundleContext context, DependencyManager manager) throws Exception {
        // Nop
    }

    @Override
    public void init(BundleContext context, DependencyManager manager) throws Exception {
        Properties props = new Properties();
        props.put("alias", "/stats");

        manager.add(createComponent()
            .setInterface(Servlet.class.getName(), props)
            .setImplementation(ViewStatsServlet.class)
            .add(createServiceDependency().setService(StatsProvider.class).setCallbacks("add", "remove").setRequired(false))
            .add(createServiceDependency().setService(LogService.class).setRequired(false)));

        props = new Properties();
        props.put("alias", "/coordinator");

        manager.add(createComponent()
            .setInterface(Servlet.class.getName(), props)
            .setImplementation(CoordinatorServlet.class)
            .add(createServiceDependency().setService(Producer.class).setCallbacks("addProducer", "removeProducer").setRequired(false))
            .add(createServiceDependency().setService(Processor.class).setCallbacks("addProcessor", "removeProcessor").setRequired(false))
            .add(createServiceDependency().setService(StatsProvider.class, "(aggregator=true)").setRequired(true))
            .add(createServiceDependency().setService(LogService.class).setRequired(false)));
    }
}
