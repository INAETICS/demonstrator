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

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.inaetics.demonstrator.api.stats.StatsProvider;

import com.fasterxml.jackson.core.JsonFactory;
import com.fasterxml.jackson.core.JsonGenerator;

/**
 * Provides read/write access to the system utilisation.
 */
public class UtilisationServlet extends HttpServlet {
    private volatile StatsProvider m_aggregator;

    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
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
    }

    @Override
    protected void doPost(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        resp.setStatus(HttpServletResponse.SC_NO_CONTENT);
    }
}
