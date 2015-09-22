set(BUNDLE_SYMBOLICNAME "org.inaetics.demonstrator.data_store")
set(BUNDLE_VERSION "1.0.0")
set(BUNDLE_NAME "org.inaetics.demonstrator.api.datastore.DataStore")

include_directories("${CELIX_INCLUDE_DIR}")
include_directories("${CELIX_INCLUDE_DIR}/remote_service_admin")
include_directories("private/include")

bundle(org.inaetics.demonstrator.api.datastore.DataStore
	SOURCES private/src/activator
		    private/src/arraylist_data_store_impl
)

target_link_libraries(org.inaetics.demonstrator.api.datastore.DataStore log ${CELIX_LIBRARIES})
