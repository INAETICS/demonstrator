deploy("inaetics-demonstrator" BUNDLES
		${CELIX_BUNDLES_DIR}/shell.zip
		${CELIX_BUNDLES_DIR}/shell_tui.zip
                sample_queue
        producer
            processor
      )

  deploy("inaetics-demonstrator-queue" BUNDLES
		${CELIX_BUNDLES_DIR}/shell.zip
		${CELIX_BUNDLES_DIR}/shell_tui.zip
		${CELIX_BUNDLES_DIR}/discovery_configured.zip
		${CELIX_BUNDLES_DIR}/topology_manager.zip
		${CELIX_BUNDLES_DIR}/remote_service_admin_http.zip
		sample_queue
        ENDPOINTS org.inaetics.demonstator.api.SampleQueue_endpoint
      )

  deploy("inaetics-demonstrator-producer" BUNDLES
		${CELIX_BUNDLES_DIR}/shell.zip
		${CELIX_BUNDLES_DIR}/shell_tui.zip
		${CELIX_BUNDLES_DIR}/topology_manager.zip
		${CELIX_BUNDLES_DIR}/discovery_configured.zip
		${CELIX_BUNDLES_DIR}/remote_service_admin_http.zip
        producer
        ENDPOINTS org.inaetics.demonstator.api.SampleQueue_proxy
      )

  deploy("inaetics-demonstrator-processor" BUNDLES
		${CELIX_BUNDLES_DIR}/shell.zip
		${CELIX_BUNDLES_DIR}/shell_tui.zip
		${CELIX_BUNDLES_DIR}/topology_manager.zip
                ${CELIX_BUNDLES_DIR}/discovery_configured.zip
                ${CELIX_BUNDLES_DIR}/remote_service_admin_http.zip
        processor
        ENDPOINTS 
        org.inaetics.demonstator.api.SampleQueue_proxy
        org.inaetics.demonstator.api.DataStore_proxy
      )
  
  deploy("inaetics-demonstrator-dataStore" BUNDLES
                ${CELIX_BUNDLES_DIR}/shell.zip
                ${CELIX_BUNDLES_DIR}/shell_tui.zip
                ${CELIX_BUNDLES_DIR}/discovery_configured.zip
                ${CELIX_BUNDLES_DIR}/topology_manager.zip
                ${CELIX_BUNDLES_DIR}/remote_service_admin_http.zip
                data_store
        ENDPOINTS org.inaetics.demonstator.api.DataStore_endpoint
      )
