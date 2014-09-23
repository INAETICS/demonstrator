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
package org.inaetics.demonstrator.api.datastore;

import java.util.Collection;

import org.inaetics.demonstrator.api.data.Result;

import aQute.bnd.annotation.ProviderType;

@ProviderType
public interface DataStore {

	/**
	 * Find result in the data store with a simple time between begin and end.
	 * @param begin 	Begin time in milliseconds since unix epoch time.
	 * @param end 		End time in milliseconds since unix epoch time.
	 * @return			A collection of Result. The collection size will be 0 when
	 * 					no result can be found.
	 */
	public Collection<Result> findResultsBetween(long begin, long end);
	
	
	/**
	 * Store the result in the data store.
	 */
	public void store(Result result);
	
	/*
	 * Store all the results in the data store.
	 */
	public void storeAll(Collection<Result> results);
}
