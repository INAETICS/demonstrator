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
