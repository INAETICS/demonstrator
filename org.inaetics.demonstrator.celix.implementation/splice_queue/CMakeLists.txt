set(BUNDLE_SYMBOLICNAME "org.inaetics.demonstrator.splice_queue")
set(BUNDLE_VERSION "1.0.0")
set(BUNDLE_NAME "org.inaetics.demonstrator.api.queue.SpliceQueue")
set(OPENSPLICE_LIB_PATH "${CMAKE_CURRENT_LIST_DIR}/opensplice/x86_64.linux/lib")
set(OPENSPLICE_LIBRARIES "${OPENSPLICE_LIB_PATH}/libdcpssac.so" "${OPENSPLICE_LIB_PATH}/libddskernel.so")
file(GLOB OPENSPLICE__TO_ADDLIBRARIES ${OPENSPLICE_LIB_PATH}/*.so)
set(OPENSPLICE_INCLUDE_DIR "opensplice/x86_64.linux/include")
set(OPENSPLICE_CONFIG_FILES_DIR "${CMAKE_CURRENT_LIST_DIR}/opensplice/x86_64.linux/etc/config")

include_directories("${CELIX_INCLUDE_DIR}")
include_directories("${CELIX_INCLUDE_DIR}/remote_service_admin")
include_directories("${OPENSPLICE_INCLUDE_DIR}/dcps/C/SAC")
include_directories("${OPENSPLICE_INCLUDE_DIR}/sys")
include_directories("${OPENSPLICE_INCLUDE_DIR}")
include_directories("private/include")

link_directories("${OPENSPLICE_LIB_PATH}")

bundle(org.inaetics.demonstrator.api.queue.SpliceQueue
	SOURCES private/src/activator 
		private/src/splice_sample_queue_impl
		private/src/splice_data_queue
		private/src/sampleSacDcps
		private/src/sampleSplDcps
		
	FILES ${OPENSPLICE__TO_ADDLIBRARIES}
              ${OPENSPLICE_CONFIG_FILES_DIR}/ospl_sp_ddsi_statistics.xml
              ${OPENSPLICE_CONFIG_FILES_DIR}/ospl_sp_ddsi.xml
              ${OPENSPLICE_CONFIG_FILES_DIR}/ospl_sp_no_network.xml
              ${OPENSPLICE_CONFIG_FILES_DIR}/ospl.xml
)


target_link_libraries(org.inaetics.demonstrator.api.queue.SpliceQueue pthread ${CELIX_LIBRARIES} ${OPENSPLICE_LIBRARIES})
set_target_properties(org.inaetics.demonstrator.api.queue.SpliceQueue PROPERTIES INSTALL_RPATH $ORIGIN BUILD_WITH_INSTALL_RPATH YES)
