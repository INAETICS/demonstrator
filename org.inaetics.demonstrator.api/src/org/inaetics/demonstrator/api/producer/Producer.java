/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.api.producer;

import aQute.bnd.annotation.ConsumerType;

@ConsumerType
public interface Producer {

    /**
     * @return the maximum sample rate allowed, in samples/sec, &gt; 0.
     */
    int getMaxSampleRate();

    /**
     * @return the actual sample rate, in samples/sec, &gt; 0.
     */
    int getSampleRate();

    /**
     * @param rate the new sample rate, in samples/sec, &gt; 0.
     */
    void setSampleRate(int rate);
}
