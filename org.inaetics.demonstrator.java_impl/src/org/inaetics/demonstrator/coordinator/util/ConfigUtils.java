/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.coordinator.util;

import java.util.Dictionary;

import org.osgi.service.cm.ConfigurationException;

/**
 * Provides convenience utilities to parse configuration from dictionaries.
 */
public class ConfigUtils {

    public static int getConfigIntValue(Dictionary<String, ?> properties, String key) throws ConfigurationException {
        Object value = properties.get(key);
        if (value == null) {
            throw new ConfigurationException(key, "missing value!");
        }

        try {
            return Integer.parseInt(value.toString());
        } catch (NumberFormatException e) {
            throw new ConfigurationException(key, "not an integer", e);
        }
    }

    public static String getConfigStringValue(Dictionary<String, ?> properties, String key)
        throws ConfigurationException {
        Object value = properties.get(key);
        if (value == null) {
            throw new ConfigurationException(key, "missing value!");
        }

        String result = value.toString().trim();
        if ("".equals(result)) {
            throw new ConfigurationException(key, "empty value!");
        }

        return result;
    }

    public static String[] getConfigStringArrayValue(Dictionary<String, ?> properties, String key)
        throws ConfigurationException {
        Object value = properties.get(key);
        if (value == null) {
            throw new ConfigurationException(key, "missing value!");
        }

        String[] result;
        if (value instanceof String[]) {
            result = (String[]) value;
        } else {
            result = value.toString().trim().split("\\s*,\\s*");
        }

        if (result.length < 1) {
            throw new ConfigurationException(key, "empty value!");
        }

        return result;
    }
}
