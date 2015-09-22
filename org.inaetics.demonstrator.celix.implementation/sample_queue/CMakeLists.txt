set(BUNDLE_SYMBOLICNAME "org.inaetics.demonstrator.sample_queue")
set(BUNDLE_VERSION "1.0.0")
set(BUNDLE_NAME "org.inaetics.demonstrator.api.queue.SampleQueue")

include_directories("${CELIX_INCLUDE_DIR}")
include_directories("${CELIX_INCLUDE_DIR}/remote_service_admin")
include_directories("private/include")

bundle(org.inaetics.demonstrator.api.queue.SampleQueue
	SOURCES private/src/activator
		    private/src/arraylist_sample_queue_impl
)

target_link_libraries(org.inaetics.demonstrator.api.queue.SampleQueue log ${CELIX_LIBRARIES})
