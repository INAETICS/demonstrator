/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.coordinator.k8s;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class ReplicationController {

	private static final String REPLICA_PATTERN_STRING = "\"spec\": \\{\\s*\"replicas\": ";
	private static final Pattern REPLICA_PATTERN = Pattern.compile(REPLICA_PATTERN_STRING + "(\\d+)");

	private String m_name;
	private String m_json;
	private int m_replicaCount;
	
	public ReplicationController(String name, String json) {
		m_name = name;
		m_json = json;
		
		Matcher m = REPLICA_PATTERN.matcher(json);
		m.find();
		m_replicaCount = Integer.valueOf(m.group(1));
	}
	
	public void increaseReplicaCount(int maxCount) {
		if (m_replicaCount >= maxCount) {
			return;
		}
		m_replicaCount++;
		updateJson();
	}
	
	public void decreaseReplicaCount(int minCount) {
		if (m_replicaCount <= minCount) {
			return;
		}
		m_replicaCount--;
		updateJson();
	}

	public int getReplicaCount() {
		return m_replicaCount;
	}

	public void setReplicaCount(int replicaCount) {
		this.m_replicaCount = replicaCount;
		updateJson();
	}

	public String getName() {
		return m_name;
	}

	private void updateJson() {
		m_json = m_json.replaceAll(REPLICA_PATTERN.pattern(), REPLICA_PATTERN_STRING.replace("\\s*", " ") + m_replicaCount);
	}

	String getJson() {
		return m_json;
	}
}
