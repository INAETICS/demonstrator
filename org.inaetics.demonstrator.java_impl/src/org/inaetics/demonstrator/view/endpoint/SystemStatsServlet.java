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
import java.util.concurrent.atomic.AtomicInteger;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.inaetics.demonstrator.api.processor.Processor;
import org.inaetics.demonstrator.api.producer.Producer;

import com.fasterxml.jackson.core.JsonFactory;
import com.fasterxml.jackson.core.JsonGenerator;

/**
 * Provides read-only access to some of the (low-level) system statistics.
 */
public class SystemStatsServlet extends HttpServlet {
    private final AtomicInteger m_producerCount = new AtomicInteger(0);
    private final AtomicInteger m_processorCount = new AtomicInteger(0);

    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        JsonFactory factory = new JsonFactory();

        try (JsonGenerator generator = factory.createGenerator(resp.getOutputStream())) {
            generator.writeStartObject();
            generator.writeNumberField("processors", m_processorCount.get());
            generator.writeNumberField("producers", m_producerCount.get());
            generator.writeEndObject();
        }

        resp.setStatus(HttpServletResponse.SC_OK);
        resp.flushBuffer();
    }

    /* Called by Felix DM. */
    protected final void addProducer(Producer p) {
        m_producerCount.incrementAndGet();
    }

    /* Called by Felix DM. */
    protected final void removeProducer(Producer p) {
        m_producerCount.decrementAndGet();
    }

    /* Called by Felix DM. */
    protected final void addProcessor(Processor p) {
        m_processorCount.incrementAndGet();
    }

    /* Called by Felix DM. */
    protected final void removeProcessor(Processor p) {
        m_processorCount.decrementAndGet();
    }
}
