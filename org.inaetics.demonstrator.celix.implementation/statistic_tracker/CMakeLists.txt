set(BUNDLE_SYMBOLICNAME "org.inaetics.demonstrator.statistic_tracker")
set(BUNDLE_VERSION "1.0.0")
set(BUNDLE_NAME "statistic_tracker")

include_directories("${CELIX_INCLUDE_DIR}")
include_directories("private/include")

bundle(statistic_tracker
	SOURCES
	 	private/src/activator.c
	 	private/src/statistic_tracker_impl.c
)

target_link_libraries(statistic_tracker pthread ${CELIX_LIBRARIES})
