/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.api.queue;

import java.util.Collection;

import org.inaetics.demonstrator.api.data.Sample;

import aQute.bnd.annotation.ProviderType;

@ProviderType
public interface SampleQueue {

	/**
	 * Non blocking call to add a sample to the queue
	 * as long as there is space. 
	 * @return true if the sample is added to the queue. false if 
	 * the max queue size is reached. 
	 */
	public boolean put(Sample sample);
	
	/**
	 * Non blocking call to add multiple samples to the queue
	 * @return The number of samples added to the queue. Could
	 * be smaller than the provided samples if the max queue 
	 * sized is reached.
	 */
	public int putAll(Collection<Sample> samples);
	
	/**
	 * Blocking call to retrieve one sample from the queue
	 * @return a sample for the queue.
	 */
	public Sample take();
	
	/**
	 * Blocking call to retrieve multiple samples from the queue.
	 * @param min 	The minimal number of sample to retrieve. The method will block
	 * 				until the minimal amount is collected.
	 * @param max	The maximal number of sample to retrieve. If there are abundant 
	 * 				sample in the queue, only the maximum amount specified will be returned.
	 * @return 		Multiple samples from the queue.	
	 */
	public Collection<Sample> takeMinMax(int min, int max);
}
