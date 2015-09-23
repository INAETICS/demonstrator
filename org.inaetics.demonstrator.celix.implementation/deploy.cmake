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
        ${CELIX_BUNDLES_DIR}/discovery_etcd.zip
		${CELIX_BUNDLES_DIR}/topology_manager.zip
		${CELIX_BUNDLES_DIR}/remote_service_admin_dfi.zip
		org.inaetics.demonstrator.api.queue.SampleQueue
        PROPERTIES
            DISCOVERY_CFG_POLL_INTERVAL=1
            LOGHELPER_ENABLE_STDOUT_FALLBACK=true
            RSA_PORT=50801
            DISCOVERY_ETCD_ROOT_PATH=inaetics/discovery

  )

  deploy("inaetics-demonstrator-statistic-tracker" BUNDLES
		${CELIX_BUNDLES_DIR}/shell.zip
		${CELIX_BUNDLES_DIR}/shell_tui.zip
		${CELIX_BUNDLES_DIR}/topology_manager.zip
        ${CELIX_BUNDLES_DIR}/discovery_etcd.zip
		${CELIX_BUNDLES_DIR}/remote_service_admin_dfi.zip
        org.inaetics.demonstrator.api.stats.StatisticTracker
        PROPERTIES
          DISCOVERY_CFG_POLL_INTERVAL=1
          LOGHELPER_ENABLE_STDOUT_FALLBACK=true
          RSA_PORT=50802
          DISCOVERY_ETCD_ROOT_PATH=inaetics/discovery

  )

  deploy("inaetics-demonstrator-producer" BUNDLES
		${CELIX_BUNDLES_DIR}/shell.zip
		${CELIX_BUNDLES_DIR}/shell_tui.zip
		${CELIX_BUNDLES_DIR}/topology_manager.zip
        ${CELIX_BUNDLES_DIR}/discovery_etcd.zip
		${CELIX_BUNDLES_DIR}/remote_service_admin_dfi.zip
        org.inaetics.demonstrator.api.producer.Producer
        PROPERTIES
            DISCOVERY_CFG_POLL_INTERVAL=1
            LOGHELPER_ENABLE_STDOUT_FALLBACK=true
            RSA_PORT=50803
            DISCOVERY_ETCD_ROOT_PATH=inaetics/discovery
      )

  deploy("inaetics-demonstrator-processor" BUNDLES
		${CELIX_BUNDLES_DIR}/shell.zip
		${CELIX_BUNDLES_DIR}/shell_tui.zip
		${CELIX_BUNDLES_DIR}/topology_manager.zip
        ${CELIX_BUNDLES_DIR}/discovery_etcd.zip
        ${CELIX_BUNDLES_DIR}/remote_service_admin_dfi.zip
        org.inaetics.demonstrator.api.processor.Processor
        PROPERTIES
            DISCOVERY_CFG_POLL_INTERVAL=1
            LOGHELPER_ENABLE_STDOUT_FALLBACK=true
            RSA_PORT=50804
            DISCOVERY_ETCD_ROOT_PATH=inaetics/discovery
      )
  
  deploy("inaetics-demonstrator-dataStore" BUNDLES
        ${CELIX_BUNDLES_DIR}/shell.zip
        ${CELIX_BUNDLES_DIR}/shell_tui.zip
        ${CELIX_BUNDLES_DIR}/discovery_etcd.zip
        ${CELIX_BUNDLES_DIR}/topology_manager.zip
        ${CELIX_BUNDLES_DIR}/remote_service_admin_dfi.zip
        org.inaetics.demonstrator.api.datastore.DataStore
        PROPERTIES
            DISCOVERY_CFG_POLL_INTERVAL=1
            LOGHELPER_ENABLE_STDOUT_FALLBACK=true
            RSA_PORT=50805
            DISCOVERY_ETCD_ROOT_PATH=inaetics/discovery
      )
