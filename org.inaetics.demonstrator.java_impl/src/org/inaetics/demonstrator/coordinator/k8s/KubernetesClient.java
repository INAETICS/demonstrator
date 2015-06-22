/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.coordinator.k8s;

import java.io.IOException;

import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.fluent.Request;
import org.apache.http.client.methods.HttpPatch;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.client.DefaultHttpClient;
import org.inaetics.demonstrator.coordinator.SimpleQueueProcessorCoordinator;
import org.inaetics.demonstrator.coordinator.k8s.rc.ReplicationController;
import org.osgi.service.log.LogService;

public class KubernetesClient {
	
	private String m_url;
	private SimpleQueueProcessorCoordinator m_coordinator;
	
	private static final String PATH_API = "/api/v1beta3";
	private static final String PATH_NAMESPACE = "/namespaces/default";
	private static final String PATH_REPLICATIONCONTROLLERS = "/replicationcontrollers/";

	public KubernetesClient(String url, SimpleQueueProcessorCoordinator coordinator) {
		m_url = url;
		m_coordinator = coordinator;
	}

	private LogService getLogService() {
		return m_coordinator.getLogService();
	}
	
	public ReplicationController getReplicationController(String name) {
		try {
			String json = getReplicationControllerRessource(name);
			if(json == null || json.length() == 0) {
				getLogService().log(LogService.LOG_ERROR, "error getting rc (json is empty) " + name);
				return null;
			}
			return new ReplicationController(name, json);
		} catch (Throwable e) {
			getLogService().log(LogService.LOG_ERROR, "error getting rc " + name, e);
			return null;
		}
	}
	
	public void updateReplicationController(ReplicationController rc) {
		try {
			patchResource(PATH_REPLICATIONCONTROLLERS + rc.getName(), rc.getJsonPatch());
		} catch (Throwable e) {
			getLogService().log(LogService.LOG_ERROR, "error updating rc " + rc.getName(), e);
		}
	}
	
	private String getReplicationControllerRessource(String name) throws ClientProtocolException, IOException {
		return getResource(PATH_REPLICATIONCONTROLLERS + name);
	}
	
	private String getResource(String resourcePath) throws ClientProtocolException, IOException {
		getLogService().log(LogService.LOG_INFO, "getting resource " + resourcePath);
		return Request.Get(m_url + PATH_API + PATH_NAMESPACE + resourcePath).execute().returnContent().asString();
	}
	
	private void patchResource(String resourcePath, String resource) throws ClientProtocolException, IOException {
		getLogService().log(LogService.LOG_INFO, "patching resource " + resourcePath);
		HttpPatch request = null;
		try {
			// see https://github.com/GoogleCloudPlatform/kubernetes/blob/master/docs/api-conventions.md#patch-operations
			request = new HttpPatch(m_url + PATH_API + PATH_NAMESPACE + resourcePath);
			request.setHeader("Content-Type", "application/strategic-merge-patch+json");
			request.setEntity(new StringEntity(resource));
			
			DefaultHttpClient httpclient = new DefaultHttpClient();
			httpclient.execute(request);
		} finally {
		    request.releaseConnection();
		}
	}

}
