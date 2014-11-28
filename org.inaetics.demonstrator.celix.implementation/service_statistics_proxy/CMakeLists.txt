find_package(Jansson REQUIRED)

include_directories("${JANSSON_INCLUDE_DIRS}")
include_directories("${CELIX_INCLUDE_DIR}")
include_directories("${CELIX_INCLUDE_DIR}/remote_service_admin")
include_directories("private/include")


set(BUNDLE_SYMBOLICNAME "org.inaetics.demonstrator.service_statistics_proxy")
set(BUNDLE_VERSION "1.0.0")
set(BUNDLE_NAME "org.inaetics.demonstrator.api.servstat.ServiceStatistics_proxy")

bundle(org.inaetics.demonstrator.api.servstat.ServiceStatistics_proxy SOURCES 
	private/src/service_statistics_proxy_activator
	private/src/service_statistics_proxy_impl
    private/include/service_statistics_proxy_impl.h
)

target_link_libraries(org.inaetics.demonstrator.api.servstat.ServiceStatistics_proxy ${JANSSON_LIBRARIES} ${CELIX_LIBRARIES})
