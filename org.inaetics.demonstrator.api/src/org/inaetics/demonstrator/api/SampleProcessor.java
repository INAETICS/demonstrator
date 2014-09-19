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
public interface SampleProcessor {

	/**
	 * Blocking call to retrieve one result from the sample processor
	 * @return a result for the sample processor.
	 */
	public Result take();
	
	/**
	 * Blocking call to retrieve multiple result from the sample processor.
	 * @param min 	The minimal number of result to retrieve. The method will block
	 * 				until the minimal amount is collected.
	 * @param max	The maximal number of results to retrieve. If there are abundant 
	 * 				results in the sample processor, only the maximum amount specified will be returned.
	 * @return 		Multiple results from the sample processor.	
	 */
	public Collection<Result> takeMinMax(int min, int max);
	
}
