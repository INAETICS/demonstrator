/**
 * Licensed under Apache License v2. See LICENSE for more information.
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
