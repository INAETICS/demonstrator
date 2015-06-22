/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.coordinator.k8s.rc.model;

public class ReplicationControllerModel {

	private Spec m_spec;

	public Spec getSpec() {
		return m_spec;
	}

	public void setSpec(Spec m_spec) {
		this.m_spec = m_spec;
	}
}
