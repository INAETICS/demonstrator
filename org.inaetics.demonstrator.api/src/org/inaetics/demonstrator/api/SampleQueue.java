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
package org.inaetics.demonstrator.api;

import java.util.Collection;

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
