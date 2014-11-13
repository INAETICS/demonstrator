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

import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Dictionary;
import java.util.List;
import java.util.Map;
import java.util.concurrent.CopyOnWriteArrayList;

import org.inaetics.demonstrator.api.data.Result;
import org.inaetics.demonstrator.api.data.Sample;
import org.inaetics.demonstrator.api.datastore.DataStore;
import org.inaetics.demonstrator.stub.stats.StatsProvider;
import org.osgi.service.cm.ConfigurationException;
import org.osgi.service.cm.ManagedService;
import org.osgi.service.log.LogService;

public class InMemoryDataStore extends StatsProvider implements DataStore, ManagedService {
	private static final String TITLE = "Data Store Stats";

	private final CopyOnWriteArrayList<Result> m_store;
	private final SizeStatsCollector m_statsCollector;
	private volatile double m_lastSlope;

	// Injected by Felix DM...
	private volatile LogService m_log;

	public InMemoryDataStore() {
		m_store = new CopyOnWriteArrayList<>();
		m_statsCollector = new SizeStatsCollector();
		m_lastSlope = 0.0;
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
	public void store(Result result) {
		m_store.add(result);
		m_statsCollector.put(m_store.size());
	}

	@Override
	public void storeAll(Collection<Result> results) {
		m_store.addAll(results);
		m_statsCollector.put(m_store.size());
	}

	@Override
	public void updated(Dictionary<String, ?> properties) throws ConfigurationException {
		// Nothing yet...
	    m_log.log(LogService.LOG_INFO, "InMemoryDataStore updated...");
	}

	@Override
	protected String getTitle() {
		return TITLE;
	}

	@Override
	protected void renderStats(PrintWriter writer) {
		SizeStats stats = m_statsCollector.getStats();

		double slope = stats.getSlope();
        renderSlope(writer, m_lastSlope, slope);
        m_lastSlope = slope;

		writer.append("<dl class=\"dl-horizontal\">");
		for (Map.Entry<Long, Integer> entry : stats.getEntries()) {
			writer.printf("<dt>%1$tF %1$tT.%1$tL</dt><dd>size: %2$d entries</dd>", entry.getKey(), entry.getValue());
		}
		writer.append("</dl>");
	}
}
