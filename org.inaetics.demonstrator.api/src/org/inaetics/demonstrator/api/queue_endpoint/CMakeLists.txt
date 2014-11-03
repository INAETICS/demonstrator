find_package(Jansson REQUIRED)

include_directories("${JANSSON_INCLUDE_DIRS}")
include_directories("${CELIX_INCLUDE_DIR}")
include_directories("${CELIX_INCLUDE_DIR}/remote_service_admin")
include_directories("${PROJECT_SOURCE_DIR}/org.inaetics.demonstrator.api/src/org/inaetics/demonstrator/api/sample_queue/public/include")
include_directories("private/include")

set(BUNDLE_SYMBOLICNAME "org.inaetics.demonstrator.queue_endpoint")
set(BUNDLE_VERSION "1.0.0")
set(BUNDLE_NAME "org.inaetics.demonstator.api.SampleQueue_endpoint")

bundle(org.inaetics.demonstator.api.SampleQueue_endpoint SOURCES 
	private/src/queue_endpoint_activator
	private/src/queue_endpoint_impl.c
    
    private/include/queue_endpoint_impl.h
)

target_link_libraries(org.inaetics.demonstator.api.SampleQueue_endpoint ${JANSSON_LIBRARIES} ${CELIX_LIBRARIES})
