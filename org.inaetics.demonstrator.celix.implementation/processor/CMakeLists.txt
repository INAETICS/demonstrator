set(BUNDLE_SYMBOLICNAME "org.inaetics.demonstrator.processor")
set(BUNDLE_VERSION "1.0.0")
set(BUNDLE_NAME "org.inaetics.demonstrator.api.processor.Processor")

include_directories("${CELIX_INCLUDE_DIR}")
include_directories("${CELIX_INCLUDE_DIR}/remote_service_admin")
include_directories("private/include")

bundle(org.inaetics.demonstrator.api.processor.Processor
	SOURCES
	 	private/src/activator
	 	private/src/processor_impl
	FILES
		${PROJECT_SOURCE_DIR}/org.inaetics.demonstrator.api/include/inaetics_demonstrator_api/org.inaetics.demonstrator.api.queue.SampleQueue.descriptor
		${PROJECT_SOURCE_DIR}/org.inaetics.demonstrator.api/include/inaetics_demonstrator_api/org.inaetics.demonstrator.api.datastore.DataStore.descriptor
                ${PROJECT_SOURCE_DIR}/org.inaetics.demonstrator.api/include/inaetics_demonstrator_api/org.inaetics.demonstrator.api.stats.StatsProvider.descriptor
)

target_link_libraries(org.inaetics.demonstrator.api.processor.Processor rt pthread ${CELIX_LIBRARIES})
