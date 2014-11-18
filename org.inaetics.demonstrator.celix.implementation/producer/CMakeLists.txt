set(BUNDLE_SYMBOLICNAME "org.inaetics.demonstrator.producer")
set(BUNDLE_VERSION "1.0.0")
set(BUNDLE_NAME "producer")

include_directories("${CELIX_INCLUDE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/org.inaetics.demonstrator.api/src/org/inaetics/demonstrator/api/sample_queue/public/include")
include_directories("private/include")

bundle(producer
	SOURCES
	 	private/src/activator.c
	 	private/src/producer_impl.c
)

target_link_libraries(producer pthread ${CELIX_LIBRARIES})
