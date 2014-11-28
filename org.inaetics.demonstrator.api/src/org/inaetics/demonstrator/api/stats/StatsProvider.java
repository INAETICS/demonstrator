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
package org.inaetics.demonstrator.api.stats;

/**
 * Provides simple (one-value) statistics.
 */
public interface StatsProvider {

    /**
     * @return the unit of measurement of the returned value, for example, "%". Use the
     *         empty string in case no unit is available.
     */
    String getMeasurementUnit();

    /**
     * @return a displayable name for this provider, for example, "Queue statistics".
     *         Cannot be <code>null</code>.
     */
    String getName();

    /**
     * @return a lowercase ASCII name for the type of statistic that is returned by this
     *         provider, such as "utilization" or "size". Cannot be <code>null</code>
     */
    String getType();

    /**
     * @return the statistical value, as double-precision floating point number.
     */
    double getValue();

}
