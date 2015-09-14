package org.inaetics.demonstrator.clusterinfo;

import java.math.BigInteger;
import java.net.URL;
import java.util.Iterator;
import java.util.List;

import org.inaetics.demonstrator.api.data.DockerContainerInfo;
import org.inaetics.demonstrator.api.data.FleetUnitInfo;

import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;

public class DockerInfoUpdater {

	private final static int CADVISOR_PORT=4194;
	private final static String CADVISOR_PATH="/api/v1.3/docker";
	private final static double NANOSEC_PER_SEC = 1000000000.0; 
	private final static int LAST_STATS_INDEX = 59; /* Stats is an array of 60 JSON objects */

	public static void updateDockerContainerInfo(FleetUnitInfo f){

		List<DockerContainerInfo> c_list = f.getContainerList(); 
		
		c_list.clear();;

		ObjectMapper m = new ObjectMapper();
		JsonNode root = null;

		try{
			root = m.readTree(new URL("http://"+f.getIpAddress()+":"+CADVISOR_PORT+CADVISOR_PATH));
		}
		catch(Exception e){
			e.printStackTrace();
			return;
		}

		Iterator<JsonNode> iter = root.iterator();
		while(iter.hasNext()){

			JsonNode c_node = iter.next();

			String c_name = c_node.path("aliases").get(0).textValue();

			// Let's filter out the POD containers, they don't belong to us
			if(! c_name.contains("POD")){

				double cpuLoad = getContainerCpuUsage(c_node,f.getUpdatePeriod());
				double avgLoad = getContainerAvgLoad(c_node);
				long usedMem   = getContainerUsedMem(c_node);
				long hotMem    = getContainerHotMem(c_node);
				
				DockerContainerInfo c_info = new DockerContainerInfo(c_name,cpuLoad,avgLoad,usedMem,hotMem);

				c_list.add(c_info);

			}
		}

	}

	private static double getContainerCpuUsage(JsonNode c_node,int updatePeriod){

		c_node.path("stats");

		JsonNode firstStat=c_node.path("stats").get(LAST_STATS_INDEX - updatePeriod + 1);
		JsonNode lastStat=c_node.path("stats").get(LAST_STATS_INDEX);

		BigInteger firstCpuStat=firstStat.path("cpu").path("usage").path("total").bigIntegerValue();
		BigInteger lastCpuStat=lastStat.path("cpu").path("usage").path("total").bigIntegerValue();

		return ((lastCpuStat.doubleValue()-firstCpuStat.doubleValue())/(updatePeriod*NANOSEC_PER_SEC));

	}

	private static double getContainerAvgLoad(JsonNode c_node){

		JsonNode lastStat=c_node.path("stats").get(LAST_STATS_INDEX);

		return lastStat.path("cpu").path("load_average").doubleValue();

	}

	private static long getContainerUsedMem(JsonNode c_node){

		JsonNode lastStat=c_node.path("stats").get(LAST_STATS_INDEX);

		return lastStat.path("memory").path("usage").longValue();

	}

	private static long getContainerHotMem(JsonNode c_node){

		JsonNode lastStat=c_node.path("stats").get(LAST_STATS_INDEX);

		return lastStat.path("memory").path("working_set").longValue();

	}

}
