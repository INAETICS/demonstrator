set(BUNDLE_SYMBOLICNAME "com.inaetics_demonstrator.consumer")
set(BUNDLE_VERSION "1.0.0")
set(BUNDLE_NAME "consumer")

include_directories("${CELIX_INCLUDE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/bundles/sample_queue/public/include")

bundle(consumer
	SOURCES
	 	private/src/activator.c
)

target_link_libraries(consumer pthread ${CELIX_LIBRARIES})
