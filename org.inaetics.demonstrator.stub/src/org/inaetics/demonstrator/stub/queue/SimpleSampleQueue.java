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
package org.inaetics.demonstrator.stub.queue;

import java.util.ArrayList;
import java.util.Collection;
import java.util.List;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;

import org.inaetics.demonstrator.api.data.Sample;
import org.inaetics.demonstrator.api.queue.SampleQueue;

public class SimpleSampleQueue implements SampleQueue {
	private static final int QUEUE_SIZE = 1024;

	private final BlockingQueue<Sample> m_queue = new ArrayBlockingQueue<>(
			QUEUE_SIZE);

	@Override
	public boolean put(Sample sample) {
		try {
			m_queue.put(sample);
			return true;
		} catch (InterruptedException e) {
			return false;
		}
	}

	@Override
	public int putAll(Collection<Sample> samples) {
		int size = m_queue.size();
		for (Sample sample : samples) {
			put(sample);
		}
		return m_queue.size() - size;
	}

	@Override
	public Sample take() {
		try {
			return m_queue.take();
		} catch (InterruptedException e) {
			return null;
		}
	}

	@Override
	public Collection<Sample> takeMinMax(int min, int max) {
		List<Sample> result = new ArrayList<>(max);
		// NOTE: this is not strictly conform our contract...
		m_queue.drainTo(result, max);

		return result;
	}
}
