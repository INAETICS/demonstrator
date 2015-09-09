deploy("inaetics-demonstrator-all" BUNDLES
		org.inaetics.demonstrator.api.queue.SampleQueue
        org.inaetics.demonstrator.api.stats.StatisticTracker
        org.inaetics.demonstrator.api.datastore.DataStore
        org.inaetics.demonstrator.api.producer.Producer
        org.inaetics.demonstrator.api.processor.Processor
      )

  deploy("inaetics-demonstrator-queue" BUNDLES
		${CELIX_BUNDLES_DIR}/shell.zip
		${CELIX_BUNDLES_DIR}/shell_tui.zip
		${CELIX_BUNDLES_DIR}/discovery_configured.zip
		${CELIX_BUNDLES_DIR}/topology_manager.zip
		${CELIX_BUNDLES_DIR}/remote_service_admin_dfi.zip
		org.inaetics.demonstrator.api.queue.SampleQueue
        PROPERTIES
            LOGHELPER_ENABLE_STDOUT_FALLBACK=true
            RSA_PORT=50801
            DISCOVERY_CFG_SERVER_PORT=50901
            DISCOVERY_CFG_POLL_ENDPOINTS=http://127.0.0.1:50902/org.apache.celix.discovery.configured, http://127.0.0.1:50903/org.apache.celix.discovery.configured, http://127.0.0.1:50904/org.apache.celix.discovery.configured, http://127.0.0.1:50905/org.apache.celix.discovery.configured, http://127.0.0.1:50906/org.apache.celix.discovery.configured
  )

  deploy("inaetics-demonstrator-statistic-tracker" BUNDLES
		${CELIX_BUNDLES_DIR}/shell.zip
		${CELIX_BUNDLES_DIR}/shell_tui.zip
		${CELIX_BUNDLES_DIR}/topology_manager.zip
		${CELIX_BUNDLES_DIR}/discovery_configured.zip
		${CELIX_BUNDLES_DIR}/remote_service_admin_dfi.zip
        org.inaetics.demonstrator.api.stats.StatisticTracker
      )

  deploy("inaetics-demonstrator-producer" BUNDLES
		${CELIX_BUNDLES_DIR}/shell.zip
		${CELIX_BUNDLES_DIR}/shell_tui.zip
		${CELIX_BUNDLES_DIR}/topology_manager.zip
		${CELIX_BUNDLES_DIR}/discovery_configured.zip
		${CELIX_BUNDLES_DIR}/remote_service_admin_dfi.zip
        org.inaetics.demonstrator.api.producer.Producer
        PROPERTIES
            LOGHELPER_ENABLE_STDOUT_FALLBACK=true
            RSA_PORT=50802
            DISCOVERY_CFG_SERVER_PORT=50902
            DISCOVERY_CFG_POLL_ENDPOINTS=http://127.0.0.1:50901/org.apache.celix.discovery.configured, http://127.0.0.1:50903/org.apache.celix.discovery.configured, http://127.0.0.1:50904/org.apache.celix.discovery.configured, http://127.0.0.1:50905/org.apache.celix.discovery.configured, http://127.0.0.1:50906/org.apache.celix.discovery.configured
      )

  deploy("inaetics-demonstrator-processor" BUNDLES
		${CELIX_BUNDLES_DIR}/shell.zip
		${CELIX_BUNDLES_DIR}/shell_tui.zip
		${CELIX_BUNDLES_DIR}/topology_manager.zip
        ${CELIX_BUNDLES_DIR}/discovery_configured.zip
        ${CELIX_BUNDLES_DIR}/remote_service_admin_dfi.zip
        org.inaetics.demonstrator.api.processor.Processor
        PROPERTIES
            LOGHELPER_ENABLE_STDOUT_FALLBACK=true
            RSA_PORT=50803
            DISCOVERY_CFG_SERVER_PORT=50903
            DISCOVERY_CFG_POLL_ENDPOINTS=http://127.0.0.1:50901/org.apache.celix.discovery.configured, http://127.0.0.1:50902/org.apache.celix.discovery.configured, http://127.0.0.1:50904/org.apache.celix.discovery.configured, http://127.0.0.1:50905/org.apache.celix.discovery.configured, http://127.0.0.1:50906/org.apache.celix.discovery.configured
      )
  
  deploy("inaetics-demonstrator-dataStore" BUNDLES
        ${CELIX_BUNDLES_DIR}/shell.zip
        ${CELIX_BUNDLES_DIR}/shell_tui.zip
        ${CELIX_BUNDLES_DIR}/discovery_configured.zip
        ${CELIX_BUNDLES_DIR}/topology_manager.zip
        ${CELIX_BUNDLES_DIR}/remote_service_admin_dfi.zip
        org.inaetics.demonstrator.api.datastore.DataStore
        PROPERTIES
            LOGHELPER_ENABLE_STDOUT_FALLBACK=true
            RSA_PORT=50804
            DISCOVERY_CFG_SERVER_PORT=50904
            DISCOVERY_CFG_POLL_ENDPOINTS=http://127.0.0.1:50901/org.apache.celix.discovery.configured, http://127.0.0.1:50902/org.apache.celix.discovery.configured, http://127.0.0.1:50903/org.apache.celix.discovery.configured, http://127.0.0.1:50905/org.apache.celix.discovery.configured, http://127.0.0.1:50906/org.apache.celix.discovery.configured
      )
