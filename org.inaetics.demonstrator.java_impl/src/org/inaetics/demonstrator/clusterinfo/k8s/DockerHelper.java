/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.clusterinfo.k8s;

import java.io.IOException;
import java.math.BigInteger;
import java.net.URL;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import org.inaetics.demonstrator.api.clusterinfo.ContainerInfo;

import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;

public class DockerHelper {

    private final static String CADVISOR_PATH = "/api/v1.3/docker";
    private final static double NANOSEC_PER_SEC = 1000000000.0;

    public static List<ContainerInfo> getContainerInfo(String containerIpAddress, int cAdvisorPort) {
        List<ContainerInfo> result = new ArrayList<>();

        JsonNode root = null;

        try {
            ObjectMapper m = new ObjectMapper();
            root = m.readTree(new URL("http://" + containerIpAddress + ":" + cAdvisorPort + CADVISOR_PATH));
        } catch (IOException e) {
            return result;
        }

        Iterator<JsonNode> iter = root.iterator();
        while (iter.hasNext()) {
            JsonNode c_node = iter.next();

            String c_name = c_node.path("aliases").get(0).textValue();

            // Let's filter out the POD containers, they don't belong to us
            if (!c_name.contains("POD") && !c_name.contains("kube-")) {

                JsonNode lastStats = getLast(c_node.path("stats"));

                double cpuLoad = getContainerCpuUsage(c_node);
                double avgLoad = getContainerAvgLoad(lastStats);
                long usedMem = getContainerUsedMem(lastStats);
                long hotMem = getContainerHotMem(lastStats);

                result.add(new ContainerInfo(c_name, cpuLoad, avgLoad, usedMem, hotMem));
            }
        }

        return result;
    }

    private static double getContainerCpuUsage(JsonNode c_node) {
        int size = getSize(c_node.path("stats"));

        JsonNode firstStat = c_node.path("stats").get(0);
        JsonNode lastStat = c_node.path("stats").get(size - 1);

        BigInteger firstCpuStat = firstStat.path("cpu").path("usage").path("total").bigIntegerValue();
        BigInteger lastCpuStat = lastStat.path("cpu").path("usage").path("total").bigIntegerValue();

        return ((lastCpuStat.doubleValue() - firstCpuStat.doubleValue()) / (size * NANOSEC_PER_SEC));

    }

    private static double getContainerAvgLoad(JsonNode lastStats) {
        return lastStats.path("cpu").path("load_average").doubleValue();
    }

    private static long getContainerUsedMem(JsonNode lastStats) {
        return lastStats.path("memory").path("usage").longValue();
    }

    private static long getContainerHotMem(JsonNode lastStats) {
        return lastStats.path("memory").path("working_set").longValue();
    }

    private static JsonNode getLast(JsonNode parent) {
        return parent.get(getSize(parent) - 1);
    }

    private static int getSize(JsonNode parent) {
        Iterator<JsonNode> elements = parent.elements();
        int size = 0;
        while (elements.hasNext()) {
            size++;
            elements.next();
        }
        return size;
    }

}
