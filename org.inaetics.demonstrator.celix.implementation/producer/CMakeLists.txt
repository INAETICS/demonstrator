set(BUNDLE_SYMBOLICNAME "org.inaetics.demonstrator.producer")
set(BUNDLE_VERSION "1.0.0")
set(BUNDLE_NAME "org.inaetics.demonstrator.api.producer.Producer")

include_directories("${CELIX_INCLUDE_DIR}")
include_directories("${CELIX_INCLUDE_DIR}/remote_service_admin")
include_directories("private/include")

bundle(org.inaetics.demonstrator.api.producer.Producer
	SOURCES
	 	private/src/activator.c
	 	private/src/producer_impl.c
	FILES
		../../org.inaetics.demonstrator.api/include/inaetics_demonstrator_api/org.inaetics.demonstrator.api.queue.SampleQueue.descriptor
)

target_link_libraries(org.inaetics.demonstrator.api.producer.Producer pthread ${CELIX_LIBRARIES})
