
find_package(Jansson REQUIRED)

include_directories("${JANSSON_INCLUDE_DIRS}")
include_directories("${CELIX_INCLUDE_DIR}")
include_directories("${CELIX_INCLUDE_DIR}/remote_service_admin")
include_directories("${PROJECT_SOURCE_DIR}/org.inaetics.demonstrator.api/src/org/inaetics/demonstrator/api/sample_queue/public/include")
include_directories("${PROJECT_SOURCE_DIR}/org.inaetics.demonstrator.api/src/org/inaetics/demonstrator/api/data_store/public/include")
include_directories("private/include")


set(BUNDLE_SYMBOLICNAME "org.inaetics.demonstrator.data_store_endpoint")
set(BUNDLE_VERSION "1.0.0")
set(BUNDLE_NAME "org.inaetics.demonstrator.api.DataStore_proxy")

bundle(org.inaetics.demonstrator.api.DataStore_proxy SOURCES 
	private/src/data_store_proxy_activator.c
	private/src/data_store_proxy_impl.c
    
    private/include/data_store_proxy_impl.h
)

target_link_libraries(org.inaetics.demonstrator.api.DataStore_proxy ${JANSSON_LIBRARIES} ${CELIX_LIBRARIES})
