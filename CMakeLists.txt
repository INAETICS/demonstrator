cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0002 OLD)

project(inaetics-demonstrator C)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CELIX_DIR}/share/celix/cmake/modules")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wno-long-long")

find_package(CELIX REQUIRED)
include_directories(${CELIX_INCLUDE_DIRS})

include_directories(org.inaetics.demonstrator.api/include)

add_subdirectory(org.inaetics.demonstrator.celix.implementation demonstrator)
deploy_targets()
