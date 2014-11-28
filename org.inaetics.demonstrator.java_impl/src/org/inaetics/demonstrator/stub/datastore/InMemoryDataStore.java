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
        return "";
    }

    @Override
    public String getType() {
        return "size";
    }

    @Override
    public double getValue() {
        return m_store.size();
    }

    @Override
    public void store(Result result) {
        m_store.add(result);
    }

    @Override
    public void storeAll(Collection<Result> results) {
        m_store.addAll(results);
    }

    @Override
    public void updated(Dictionary<String, ?> properties) throws ConfigurationException {
        // Nothing yet...
        m_log.log(LogService.LOG_INFO, "InMemoryDataStore updated...");
    }
}
