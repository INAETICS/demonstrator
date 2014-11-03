cmake_minimum_required(VERSION 2.8)
project(inaetics-demonstrator C)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CELIX_DIR}/share/celix/cmake/modules")

find_package(CELIX REQUIRED)
include_directories(${CELIX_INCLUDE_DIRS})

add_subdirectory(org.inaetics.demonstrator.api/src/org/inaetics/demonstrator/api)
deploy_targets()
