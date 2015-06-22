/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.coordinator.k8s.rc.model;

public class Spec {
	
	private int m_replicas;

	public int getReplicas() {
		return m_replicas;
	}

	public void setReplicas(int m_replicas) {
		this.m_replicas = m_replicas;
	}
}
