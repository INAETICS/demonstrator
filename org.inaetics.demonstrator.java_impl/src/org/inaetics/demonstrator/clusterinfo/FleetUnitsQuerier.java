/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.clusterinfo;


import java.net.URL;
import java.util.Iterator;
import java.util.Set;
import java.util.TimerTask;

import org.inaetics.demonstrator.api.stats.FleetUnitInfo;

import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;

public class FleetUnitsQuerier extends TimerTask{

	private final static String BASE_ETCD_DIR = "/v2/keys";
	private final static String FLEET_ETCD_DIR = "/_coreos.com/fleet/machines";
	private final static String FLEET_PUBLICIP_KEY = "PublicIP";
	private final static String FLEET_METADATA_KEY = "Metadata";
	private final static String FLEET_ROLE_KEY = "role";
	//private final static String DEFAULT_ETCD_ENDPOINT = "127.0.0.1:2379";
	private final static String DEFAULT_ETCD_ENDPOINT = "172.17.8.20:2379";
	private final static String SLAVE_ROLE_TAG = "kubernetes";

	private final String m_etcdEndpoint;
	private Set<FleetUnitInfo> m_hostList;
	private final int m_updatePeriod;
	private final ObjectMapper mapper;

	public FleetUnitsQuerier (Set<FleetUnitInfo> list,int updatePeriod){
		m_etcdEndpoint = "http://" + DEFAULT_ETCD_ENDPOINT;
		m_hostList=list;
		m_updatePeriod=updatePeriod;
		mapper=new ObjectMapper();
	}

	public FleetUnitsQuerier (String endpoint,Set<FleetUnitInfo> list, int updatePeriod){
		m_etcdEndpoint = "http://" + endpoint;
		m_hostList=list;
		m_updatePeriod=updatePeriod;
		mapper=new ObjectMapper();
	}

	@Override
	public void run(){

		JsonNode root = null;

		try{
			root = mapper.readTree(new URL(m_etcdEndpoint+BASE_ETCD_DIR+FLEET_ETCD_DIR)).path("node").path("nodes");
		}
		catch(Exception e){
			e.printStackTrace();
		}

		if(root!=null){
			
			Iterator<JsonNode> iter = root.iterator();
			while(iter.hasNext()){
				String f_unitKey = iter.next().path("key").textValue();
				
				JsonNode f_unitInfo = null;
				
				try{
					 f_unitInfo = mapper.readTree(new URL(m_etcdEndpoint+BASE_ETCD_DIR+f_unitKey+"/object")).path("node").path("value");
					 
					 JsonNode md = mapper.readTree(f_unitInfo.asText()).path(FLEET_METADATA_KEY);
					 
					 if(md.path(FLEET_ROLE_KEY).textValue().equals(SLAVE_ROLE_TAG)){
						 
						 String ip = mapper.readTree(f_unitInfo.asText()).path(FLEET_PUBLICIP_KEY).textValue();
						 
						 m_hostList.add(new FleetUnitInfo(ip,m_updatePeriod));
						
					 }
					 
				}
				catch(Exception e){
					e.printStackTrace();
				}
				
			}

			//System.out.println(m_hostList);
			
		}
	}

}