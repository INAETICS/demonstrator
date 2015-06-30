/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.k8sclient.impl;

import java.util.Dictionary;

import org.osgi.framework.BundleContext;
import org.osgi.service.cm.ConfigurationException;

/**
 * Provides convenience utilities to parse configuration from bundlecontext or
 * dictionaries.
 */
public class ConfigUtils {

	public static String getConfigStringValue(BundleContext context,
			Dictionary<String, ?> properties, String key)
			throws ConfigurationException {

		String value = null;
		if (properties != null && properties.get(key) != null) {
			value = properties.get(key).toString();
		}
		if (context != null && value == null) {
			value = context.getProperty(key);
		}
		if (value == null) {
			throw new ConfigurationException(key, "not found");
		}
		return value;
	}

	public static int getConfigIntValue(BundleContext context,
			Dictionary<String, ?> properties, String key)
			throws ConfigurationException {

		String value = null;
		if (properties != null && properties.get(key) != null) {
			value = properties.get(key).toString();
		}
		if (context != null && value == null) {
			value = context.getProperty(key);
		}
		if (value == null) {
			throw new ConfigurationException(key, "not found");
		}
		try {
			return Integer.parseInt(value);
		} catch (NumberFormatException e) {
			throw new ConfigurationException(key, "not an integer", e);
		}
	}

}
