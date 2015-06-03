set(BUNDLE_SYMBOLICNAME "org.inaetics.demonstrator.processor")
set(BUNDLE_VERSION "1.0.0")
set(BUNDLE_NAME "processor")

include_directories("${CELIX_INCLUDE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/org.inaetics.demonstrator.api/src/org/inaetics/demonstrator/api/sample_queue/public/include")
include_directories("${PROJECT_SOURCE_DIR}/org.inaetics.demonstrator.api/src/org/inaetics/demonstrator/api/data_store/public/include")
include_directories("private/include")

bundle(processor
	SOURCES
	 	private/src/activator
	 	private/src/processor_impl
)

target_link_libraries(processor rt pthread ${CELIX_LIBRARIES})
