/**
 * Licensed under Apache License v2. See LICENSE for more information.
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
