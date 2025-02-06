# SPDX-License-Identifier: MIT
#
# (c) Copyright 2024
# Denys Asauliak, denoming@gmail.com

cmake_minimum_required(VERSION 3.25.1)

include(cmake/ProjectOptions.cmake)

project(voxer VERSION 0.2.4
              HOMEPAGE_URL "Voxer Library")

include(cmake/ProjectConfigs.cmake)
if (PROJECT_IS_TOP_LEVEL)
    include(cmake/ProjectInstall.cmake)
endif()
include(cmake/ProjectPackaging.cmake)

add_subdirectory(config)
add_subdirectory(src)
if(VOXER_ENABLE_TESTS)
    add_subdirectory(test)
endif()
add_subdirectory(tools)
