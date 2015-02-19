/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.view.endpoint;

import java.util.LinkedHashMap;

/**
 * Provides a simple map of timestamps to any kind of value with a bounded size.
 */
public class TimestampMap<V> extends LinkedHashMap<Long, V> {
    private static final int DEFAULT_MAX_SIZE = 25;

    private final int m_maxSize;

    public TimestampMap() {
        this(DEFAULT_MAX_SIZE);
    }

    /**
     * @param maxSize the maximum number of entries to preserve, &gt; 0.
     */
    public TimestampMap(int maxSize) {
        if (maxSize < 1) {
            throw new IllegalArgumentException("Invalid maximum size!");
        }
        m_maxSize = maxSize;
    }

    public V put(V value) {
        return super.put(System.currentTimeMillis(), value);
    }

    @Override
    protected boolean removeEldestEntry(java.util.Map.Entry<Long, V> eldest) {
        return size() >= m_maxSize;
    }
}
