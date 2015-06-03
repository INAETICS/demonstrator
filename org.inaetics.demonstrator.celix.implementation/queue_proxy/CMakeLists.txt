
find_package(Jansson REQUIRED)

include_directories("${JANSSON_INCLUDE_DIRS}")
include_directories("${CELIX_INCLUDE_DIR}")
include_directories("${CELIX_INCLUDE_DIR}/remote_service_admin")
include_directories("${PROJECT_SOURCE_DIR}/org.inaetics.demonstrator.api/src/org/inaetics/demonstrator/api/sample_queue/public/include")
include_directories("private/include")


set(BUNDLE_SYMBOLICNAME "org.inaetics.demonstrator.queue_proxy")
set(BUNDLE_VERSION "1.0.0")
set(BUNDLE_NAME "org.inaetics.demonstrator.api.queue.SampleQueue_proxy")

bundle(org.inaetics.demonstrator.api.queue.SampleQueue_proxy SOURCES 
	private/src/queue_proxy_activator
	private/src/queue_proxy_impl.c
	
	${CELIX_DIR}/share/celix/remote_service_admin/remote_proxy_factory_impl.c

    private/include/queue_proxy_impl.h
)

target_link_libraries(org.inaetics.demonstrator.api.queue.SampleQueue_proxy ${JANSSON_LIBRARIES} ${CELIX_LIBRARIES})
