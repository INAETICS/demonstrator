find_package(Jansson REQUIRED)

include_directories("${JANSSON_INCLUDE_DIRS}")
include_directories("${CELIX_INCLUDE_DIR}")
include_directories("${CELIX_INCLUDE_DIR}/remote_service_admin")
include_directories("private/include")


set(BUNDLE_SYMBOLICNAME "org.inaetics.demonstrator.stats_provider_proxy")
set(BUNDLE_VERSION "1.0.0")
set(BUNDLE_NAME "org.inaetics.demonstrator.api.stats.StatsProvider_proxy")

bundle(org.inaetics.demonstrator.api.stats.StatsProvider_proxy SOURCES 
	private/src/stats_provider_proxy_activator
	private/src/stats_provider_proxy_impl
    
    ${CELIX_DIR}/share/celix/remote_service_admin/remote_proxy_factory_impl.c
)

target_link_libraries(org.inaetics.demonstrator.api.stats.StatsProvider_proxy m ${JANSSON_LIBRARIES} ${CELIX_LIBRARIES})
