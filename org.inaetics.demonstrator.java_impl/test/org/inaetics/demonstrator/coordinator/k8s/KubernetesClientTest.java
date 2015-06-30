/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.coordinator.k8s;

import java.util.Hashtable;

import org.inaetics.demonstrator.k8sclient.KubernetesClient;
import org.inaetics.demonstrator.k8sclient.ReplicationController;
import org.inaetics.demonstrator.k8sclient.impl.KubernetesClientImpl;
import org.inaetics.demonstrator.k8sclient.impl.KubernetesConfig;
import org.junit.Assert;
import org.junit.Test;
import org.osgi.service.cm.ConfigurationException;

public class KubernetesClientTest {
	
	private static final boolean EXECUTE_TEST = false;

	@Test
	public void testScalingProcessorOk() throws ConfigurationException {
		
		if (!EXECUTE_TEST) {
			return;
		}
		
		Hashtable<String, String> props = new Hashtable<>();
		props.put(KubernetesConfig.KEY_K8S_MASTER_URI, "http://172.17.8.20:10260");
		KubernetesClient kc = new KubernetesClientImpl(new KubernetesConfig(null, props));
		ReplicationController rc = kc.getReplicationController("inaetics-processor-controller");
		Assert.assertNotNull(rc);
		rc.increaseReplicaCount(5);
//		rc.decreaseReplicaCount(1);
		kc.updateReplicationController(rc);
		
	}
}
