set(BUNDLE_SYMBOLICNAME "org.inaetics.demonstrator.statistic_tracker")
set(BUNDLE_VERSION "1.0.0")
set(BUNDLE_NAME "org.inaetics.demonstrator.api.stats.StatisticTracker")

include_directories("${CELIX_INCLUDE_DIR}")
include_directories("private/include")

bundle(org.inaetics.demonstrator.api.stats.StatisticTracker
        SOURCES
                private/src/activator.c
                private/src/statistic_tracker_impl.c
        FILES
                ${PROJECT_SOURCE_DIR}/org.inaetics.demonstrator.api/include/inaetics_demonstrator_api/org.inaetics.demonstrator.api.stats.StatsProvider.descriptor
)

target_link_libraries(org.inaetics.demonstrator.api.stats.StatisticTracker pthread ${CELIX_LIBRARIES})
