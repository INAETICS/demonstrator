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
package org.inaetics.demonstrator.stub.stats;

import java.io.IOException;
import java.io.PrintWriter;
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

/**
 *
 */
public abstract class StatsProvider extends HttpServlet {

    protected static final String TEMPLATE_START =
        "<!DOCTYPE html><head><meta charset=\"utf-8\"><meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\"><meta name=\"description\" content=\"\"><meta name=\"author\" content=\"\"><meta http-equiv=\"refresh\" content=\"2\"><title>%1$s</title><link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.0/css/bootstrap.min.css\"><link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.0/css/bootstrap-theme.min.css\"></head>"
            + "<body><div class=\"container\"><div class=\"page-header\"><h1>%1$s</h1><h5>%2$tF %2$tT</h5><p class=\"lead\">";

    protected static final String TEMPLATE_END = "</p></div></div></body></html>";

    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        try (PrintWriter writer = resp.getWriter()) {
            writer.printf(TEMPLATE_START, getTitle(), System.currentTimeMillis());
            renderStats(writer);
            writer.append(TEMPLATE_END);
        }
    }

    protected abstract String getTitle();

    protected abstract void renderStats(PrintWriter writer);

    protected void renderSlope(PrintWriter writer, double oldSlope, double newSlope) {
        double speed = newSlope / oldSlope;
        int signum = Double.compare(newSlope, oldSlope);
        if (signum > 0) {
            writer.append("<h3><span class=\"label label-");
            if (speed > 2.5) {
                writer.append("danger\">&#8593;");
            } else {
                writer.append("warning\">&#8599;");
            }
            writer.append("</span></h3>");
        } else if (signum == 0) {
            writer.append("<h3><span class=\"label label-success\">&#8596;</span></h3>");
        } else {
            writer.append("<h3><span class=\"label label-");
            if (speed > 2.5) {
                writer.append("warning\">&#8595;");
            } else {
                writer.append("primary\">&#8600;");
            }
            writer.append("</span></h3>");
        }
    }

    protected static class SizeStats {
        private final Map<Long, Integer> m_stats;
        private double m_meanValue;
        private long m_minTime;
        private long m_maxTime;

        public SizeStats(Map<Long, Integer> stats) {
            m_stats = new HashMap<>(stats);

            m_meanValue = 0;
            m_minTime = m_stats.isEmpty() ? 0 : Long.MAX_VALUE;
            m_maxTime = 0;

            for (Entry<Long, Integer> entry : m_stats.entrySet()) {
                long time = entry.getKey();
                int value = entry.getValue();

                m_meanValue += value;
                m_minTime = Math.min(m_minTime, time);
                m_maxTime = Math.max(m_maxTime, time);
            }
            
            m_meanValue = m_meanValue / m_stats.size();
        }

        public Set<Entry<Long, Integer>> getEntries() {
            return m_stats.entrySet();
        }

        public double getSlope() {
            double dt = (m_maxTime - m_minTime) / 1000.0;
            double slope = m_meanValue / dt;
            return Double.isNaN(slope) ? 0 : slope;
        }
    }

    protected static class SizeStatsCollector {
        protected final TimestampMap<Integer> m_timestamps;

        public SizeStatsCollector() {
            m_timestamps = new TimestampMap<>();
        }

        public synchronized void put(int val) {
            long now = System.currentTimeMillis();
            m_timestamps.put(now, val);
        }

        public synchronized SizeStats getStats() {
            return new SizeStats(m_timestamps);
        }
    }

    private static class TimestampMap<V> extends LinkedHashMap<Long, V> {
        public TimestampMap() {
            super();
        }

        @Override
        protected boolean removeEldestEntry(Map.Entry<Long, V> eldest) {
            return size() > 20;
        }
    }
}
