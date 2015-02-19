/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.stub.datastore;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Dictionary;
import java.util.List;
import java.util.concurrent.CopyOnWriteArrayList;

import org.inaetics.demonstrator.api.data.Result;
import org.inaetics.demonstrator.api.data.Sample;
import org.inaetics.demonstrator.api.datastore.DataStore;
import org.inaetics.demonstrator.api.stats.StatsProvider;
import org.osgi.service.cm.ConfigurationException;
import org.osgi.service.cm.ManagedService;
import org.osgi.service.log.LogService;

public class InMemoryDataStore implements DataStore, StatsProvider, ManagedService {
    private static final int MAX_STORE_SIZE = 1 * 1000 * 1000; // 1 million records

    private final CopyOnWriteArrayList<Result> m_store;

    // Injected by Felix DM...
    private volatile LogService m_log;

    public InMemoryDataStore() {
        m_store = new CopyOnWriteArrayList<>();
    }

    @Override
    public Collection<Result> findResultsBetween(long begin, long end) {
        List<Result> results = new ArrayList<>();
        List<Result> snapshot = new ArrayList<>(m_store);
        for (Result result : snapshot) {
            Sample sample = result.getSample();
            if (sample.getValue1() >= begin && sample.getValue2() <= end) {
                results.add(result);
            }
        }
        return results;
    }

    @Override
    public String getName() {
        return "Data store";
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
        return (100.0 * m_store.size()) / MAX_STORE_SIZE;
    }

    @Override
    public void store(Result result) {
        if (m_store.size() < MAX_STORE_SIZE) {
            m_store.add(result);
        }
    }

    @Override
    public void storeAll(Collection<Result> results) {
        if (m_store.size() + results.size() < MAX_STORE_SIZE) {
            m_store.addAll(results);
        }
    }

    @Override
    public void updated(Dictionary<String, ?> properties) throws ConfigurationException {
        // Nothing yet...
        m_log.log(LogService.LOG_INFO, "InMemoryDataStore updated...");
    }
}
