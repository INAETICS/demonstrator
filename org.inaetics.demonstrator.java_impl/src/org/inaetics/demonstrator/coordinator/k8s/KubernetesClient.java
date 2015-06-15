/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.coordinator.k8s;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.HttpURLConnection;
import java.net.URL;
import java.net.URLConnection;

public class KubernetesClient {
	
	private String m_url;
	
	private static final String PATH_API = "/api/v1beta3";
	private static final String PATH_NAMESPACE = "/namespaces/default";
	private static final String PATH_REPLICATIONCONTROLLERS = "/replicationcontrollers/";

	public KubernetesClient(String url) {
		m_url = url;
	}
	
	public ReplicationController getReplicationController(String name) {
		String json = getReplicationControllerRessource(name);
		if(json == null || json.length() == 0) {
			return null;
		}
		return new ReplicationController(name, json);
	}
	
	public void updateReplicationController(ReplicationController rc) {
		updateResource(PATH_REPLICATIONCONTROLLERS + rc.getName(), rc.getJson());
	}
	
	private String getReplicationControllerRessource(String name) {
		return getResource(PATH_REPLICATIONCONTROLLERS + name);
	}
	
	private String getResource(String resourcePath) {
		try {
		
			URLConnection connection = new URL(m_url + PATH_API + PATH_NAMESPACE + resourcePath).openConnection();
	        connection.connect();

	        // read response from the server
	        BufferedReader reader = new BufferedReader(new InputStreamReader(connection.getInputStream()));
	        StringBuilder stringBuilder = new StringBuilder();

	        String line = null;
	        while ((line = reader.readLine()) != null)
	        {
	            stringBuilder.append(line + "\n");
	        }
	        
	        return stringBuilder.toString();
		
		} catch (Exception e) {
			e.printStackTrace();
			return null;
		}
	}
	
	private void updateResource(String resourcePath, String resource) {
		try {
			
			HttpURLConnection connection = (HttpURLConnection) new URL(m_url + PATH_API + PATH_NAMESPACE + resourcePath).openConnection();
			
			connection.setDoOutput(true);
			connection.setRequestMethod("PUT");
			
			PrintWriter writer = new PrintWriter(connection.getOutputStream());
			writer.print(resource);
			writer.flush();

	        // read response from the server
	        BufferedReader reader = new BufferedReader(new InputStreamReader(connection.getInputStream()));
	        StringBuilder stringBuilder = new StringBuilder();

	        String line = null;
	        while ((line = reader.readLine()) != null)
	        {
	            stringBuilder.append(line + "\n");
	        }
	        //System.out.println(stringBuilder.toString());
		
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

}
