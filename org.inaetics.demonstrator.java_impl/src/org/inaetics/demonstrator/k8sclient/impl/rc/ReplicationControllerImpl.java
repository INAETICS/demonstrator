/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.k8sclient.impl.rc;

import java.io.IOException;

import org.inaetics.demonstrator.k8sclient.ReplicationController;
import org.inaetics.demonstrator.k8sclient.impl.rc.model.ReplicationControllerModel;

import com.fasterxml.jackson.core.JsonParseException;
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.DeserializationFeature;
import com.fasterxml.jackson.databind.ObjectMapper;

public class ReplicationControllerImpl implements ReplicationController {

	private ObjectMapper m_mapper;

	private String m_name;
	private ReplicationControllerModel m_model;
	
	public ReplicationControllerImpl(String name, String json) throws JsonParseException, IOException {
		m_name = name;

		m_mapper = new ObjectMapper();
		m_mapper.disable(DeserializationFeature.FAIL_ON_UNKNOWN_PROPERTIES);
		m_model = m_mapper.readValue(json, ReplicationControllerModel.class);
	}
	
	/* (non-Javadoc)
	 * @see org.inaetics.demonstrator.k8sclient.impl.rc.ReplicationControllerI#increaseReplicaCount(int)
	 */
	@Override
	public void increaseReplicaCount(int maxCount) {
		int replicas = m_model.getSpec().getReplicas();
		if (replicas >= maxCount) {
			return;
		}
		m_model.getSpec().setReplicas(++replicas);
	}
	
	/* (non-Javadoc)
	 * @see org.inaetics.demonstrator.k8sclient.impl.rc.ReplicationControllerI#decreaseReplicaCount(int)
	 */
	@Override
	public void decreaseReplicaCount(int minCount) {
		int replicas = m_model.getSpec().getReplicas();
		if (replicas <= minCount) {
			return;
		}
		m_model.getSpec().setReplicas(--replicas);
	}

	/* (non-Javadoc)
	 * @see org.inaetics.demonstrator.k8sclient.impl.rc.ReplicationControllerI#getReplicaCount()
	 */
	@Override
	public int getReplicaCount() {
		return m_model.getSpec().getReplicas();
	}

	/* (non-Javadoc)
	 * @see org.inaetics.demonstrator.k8sclient.impl.rc.ReplicationControllerI#setReplicaCount(int)
	 */
	@Override
	public void setReplicaCount(int replicaCount) {
		m_model.getSpec().setReplicas(replicaCount);
	}

	/* (non-Javadoc)
	 * @see org.inaetics.demonstrator.k8sclient.impl.rc.ReplicationControllerI#getName()
	 */
	@Override
	public String getName() {
		return m_name;
	}

	public String getJsonPatch() throws JsonProcessingException {
		return m_mapper.writeValueAsString(m_model);
	}
}
