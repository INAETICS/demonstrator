set(BUNDLE_SYMBOLICNAME "org.inaetics.demonstrator.data_store")
set(BUNDLE_VERSION "1.0.0")
set(BUNDLE_NAME "data_store")

include_directories("${CELIX_INCLUDE_DIR}")
include_directories("${CELIX_INCLUDE_DIR}/remote_service_admin")
include_directories("${PROJECT_SOURCE_DIR}/org.inaetics.demonstrator.api/src/org/inaetics/demonstrator/api/sample_queue/public/include")
include_directories("private/include")
include_directories("public/include")

bundle(data_store 
	SOURCES private/src/activator 
		    private/src/arraylist_data_store_impl
)

target_link_libraries(data_store pthread ${CELIX_LIBRARIES})

