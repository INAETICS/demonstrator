cmake_minimum_required(VERSION 2.8)
project(inaetics-demonstrator C)
set(CELIX_DIR "<celix_install_dir>")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CELIX_DIR}/share/celix/cmake/modules")
find_package(CELIX REQUIRED)
include_directories(${CELIX_INCLUDE_DIRS})

add_subdirectory(bundles/sample_queue)
add_subdirectory(bundles/producer)
#add_subdirectory(bundles/consumer)

deploy_targets()
