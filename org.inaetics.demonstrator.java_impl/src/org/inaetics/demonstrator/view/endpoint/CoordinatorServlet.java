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
package org.inaetics.demonstrator.view.endpoint;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.atomic.AtomicInteger;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.inaetics.demonstrator.api.processor.Processor;
import org.inaetics.demonstrator.api.producer.Producer;
import org.inaetics.demonstrator.api.stats.StatsProvider;
import org.osgi.service.log.LogService;

import com.fasterxml.jackson.core.JsonFactory;
import com.fasterxml.jackson.core.JsonGenerator;

/**
 * Provides read-only access to some of the (low-level) system statistics.
 */
public class CoordinatorServlet extends HttpServlet {
    private volatile StatsProvider m_aggregator;
    private volatile LogService m_log;

    private final List<Producer> m_producers = new ArrayList<>();
    private final AtomicInteger m_processorCount = new AtomicInteger(0);

    /* Called by Felix DM. */
    public final void addProcessor(Processor p) {
        m_processorCount.incrementAndGet();
    }

    /* Called by Felix DM. */
    public final void addProducer(Producer p) {
        m_producers.add(p);
    }

    /* Called by Felix DM. */
    public final void removeProcessor(Processor p) {
        m_processorCount.decrementAndGet();
    }

    /* Called by Felix DM. */
    public final void removeProducer(Producer p) {
        m_producers.remove(p);
    }

    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        String pathInfo = req.getPathInfo();
        if ("/systemStats".equals(pathInfo)) {
            JsonFactory factory = new JsonFactory();

            try (JsonGenerator generator = factory.createGenerator(resp.getOutputStream())) {
                generator.writeStartObject();
                generator.writeNumberField("processors", m_processorCount.get());
                generator.writeNumberField("producers", m_producers.size());
                generator.writeEndObject();
            }

            resp.setStatus(HttpServletResponse.SC_OK);
            resp.flushBuffer();
        } else if ("/utilisation".equals(pathInfo)) {
            JsonFactory factory = new JsonFactory();

            try (JsonGenerator generator = factory.createGenerator(resp.getOutputStream())) {
                generator.writeStartObject();
                generator.writeStringField("name", m_aggregator.getName());
                generator.writeStringField("type", m_aggregator.getType());
                generator.writeStringField("unit", m_aggregator.getMeasurementUnit());
                generator.writeNumberField("value", m_aggregator.getValue());
                generator.writeEndObject();
            }

            resp.setStatus(HttpServletResponse.SC_OK);
            resp.flushBuffer();
        } else {
            resp.setStatus(HttpServletResponse.SC_NOT_FOUND);
        }
    }

    @Override
    protected void doPost(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        Integer value = getIntParameter(req, "value");
        if (value == null || value < 0 || value > 100) {
            resp.setStatus(HttpServletResponse.SC_BAD_REQUEST);
            return;
        }

        for (Producer p : m_producers) {
            try {
                int newRate = (int) ((p.getMaxSampleRate() / 100.0) * value);

                m_log.log(LogService.LOG_INFO, String.format("Setting sample rate to %d (%d %%)", newRate, value));

                p.setSampleRate(newRate);
            } catch (Exception e) {
                m_log.log(LogService.LOG_WARNING, "Failed to set sample rate!", e);
            }
        }

        resp.setStatus(HttpServletResponse.SC_NO_CONTENT);
    }

    private Integer getIntParameter(HttpServletRequest req, String paramName) {
        String[] value = req.getParameterValues(paramName);
        if (value.length == 1) {
            try {
                return Integer.parseInt(value[0]);
            } catch (NumberFormatException e) {
                // Ignore, return null...
            }
        }
        return null;
    }
}
