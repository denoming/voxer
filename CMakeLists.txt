cmake_minimum_required(VERSION 3.28)

project(voxer VERSION 0.0.1)

include(cmake/ProjectOptions.cmake)
include(cmake/ProjectConfigs.cmake)
include(cmake/ProjectInstall.cmake)

add_subdirectory(src)
if(VOXER_ENABLE_TESTS)
    add_subdirectory(test)
endif()
