find_package(Jansson REQUIRED)

include_directories("${JANSSON_INCLUDE_DIRS}")
include_directories("${CELIX_INCLUDE_DIR}")
include_directories("${CELIX_INCLUDE_DIR}/remote_service_admin")
include_directories("private/include")

set(BUNDLE_SYMBOLICNAME "org.inaetics.demonstrator.stats_provider_endpoint")
set(BUNDLE_VERSION "1.0.0")
set(BUNDLE_NAME "org.inaetics.demonstrator.api.stats.StatsProvider_endpoint")

bundle(org.inaetics.demonstrator.api.stats.StatsProvider_endpoint SOURCES 
	private/src/stats_provider_endpoint_activator
	private/src/stats_provider_endpoint_impl
    
    private/include/stats_provider_endpoint_impl.h
)

target_link_libraries(org.inaetics.demonstrator.api.stats.StatsProvider_endpoint m ${JANSSON_LIBRARIES} ${CELIX_LIBRARIES})
