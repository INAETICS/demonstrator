/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.coordinator.k8s;

import org.junit.Assert;
import org.junit.Test;

public class KubernetesClientTest {
	
	private static final boolean EXECUTE_TEST = false;

	@Test
	public void testScalingProcessorOk() {
		
		if (!EXECUTE_TEST) {
			return;
		}
		
		KubernetesClient kc = new KubernetesClient("http://172.17.8.20:10260");
		ReplicationController rc = kc.getReplicationController("inaetics-processor-controller");
		Assert.assertNotNull(rc);
		rc.increaseReplicaCount(5);
//		rc.decreaseReplicaCount(1);
		kc.updateReplicationController(rc);
		
	}
}
