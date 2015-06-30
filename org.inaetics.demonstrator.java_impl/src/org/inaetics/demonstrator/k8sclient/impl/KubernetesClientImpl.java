/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.k8sclient.impl;

import java.io.IOException;

import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.fluent.Request;
import org.apache.http.client.methods.HttpPatch;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.client.DefaultHttpClient;
import org.inaetics.demonstrator.k8sclient.KubernetesClient;
import org.inaetics.demonstrator.k8sclient.ReplicationController;
import org.inaetics.demonstrator.k8sclient.impl.rc.ReplicationControllerImpl;
import org.osgi.service.log.LogService;

public class KubernetesClientImpl implements KubernetesClient {
	
	private static final String PATH_API = "/api/v1beta3";
	private static final String PATH_NAMESPACE = "/namespaces/default";
	private static final String PATH_REPLICATIONCONTROLLERS = "/replicationcontrollers/";

	private KubernetesConfig m_config;
	
	// injected by Felix DM
	private volatile LogService m_logService;
	
	public KubernetesClientImpl(KubernetesConfig config) {
		m_config = config;
	}

	@Override
	public ReplicationController getReplicationController(String name) {
		try {
			String json = getReplicationControllerResource(name);
			if(json == null || json.length() == 0) {
				m_logService.log(LogService.LOG_ERROR, "error getting rc (json is empty) " + name);
				return null;
			}
			return new ReplicationControllerImpl(name, json);
		} catch (Throwable e) {
			m_logService.log(LogService.LOG_ERROR, "error getting rc " + name, e);
			return null;
		}
	}
	
	@Override
	public void updateReplicationController(ReplicationController rc) {
		try {
			patchResource(PATH_REPLICATIONCONTROLLERS + rc.getName(), ((ReplicationControllerImpl)rc).getJsonPatch());
		} catch (Throwable e) {
			m_logService.log(LogService.LOG_ERROR, "error updating rc " + rc.getName(), e);
		}
	}
	
	private String getReplicationControllerResource(String name) throws ClientProtocolException, IOException {
		return getResource(PATH_REPLICATIONCONTROLLERS + name);
	}
	
	private String getResource(String resourcePath) throws ClientProtocolException, IOException {
		m_logService.log(LogService.LOG_INFO, "getting resource " + resourcePath);
		return Request.Get(m_config.getK8sMasterUrl() + PATH_API + PATH_NAMESPACE + resourcePath).execute().returnContent().asString();
	}
	
	private void patchResource(String resourcePath, String resource) throws ClientProtocolException, IOException {
		m_logService.log(LogService.LOG_INFO, "patching resource " + resourcePath);
		HttpPatch request = null;
		try {
			// see https://github.com/GoogleCloudPlatform/kubernetes/blob/master/docs/api-conventions.md#patch-operations
			request = new HttpPatch(m_config.getK8sMasterUrl() + PATH_API + PATH_NAMESPACE + resourcePath);
			request.setHeader("Content-Type", "application/strategic-merge-patch+json");
			request.setEntity(new StringEntity(resource));
			
			DefaultHttpClient httpclient = new DefaultHttpClient();
			httpclient.execute(request);
		} finally {
		    request.releaseConnection();
		}
	}

}
