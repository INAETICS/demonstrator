set(BUNDLE_SYMBOLICNAME "org.inaetics.demonstrator.sample_queue")
set(BUNDLE_VERSION "1.0.0")
set(BUNDLE_NAME "sample_queue")

include_directories("${CELIX_INCLUDE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/bundles/sample_queue/private/include")
include_directories("${PROJECT_SOURCE_DIR}/bundles/sample_queue/public/include")

bundle(sample_queue 
	SOURCES private/src/activator 
		private/src/queue_service_impl
)

target_link_libraries(sample_queue pthread ${CELIX_LIBRARIES})

