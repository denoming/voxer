# SPDX-License-Identifier: MIT
#
# (c) Copyright 2024
# Denys Asauliak, denoming@gmail.com

cmake_minimum_required(VERSION 3.25.1)

include(cmake/ProjectOptions.cmake)

project(voxer VERSION 0.2.1)

include(cmake/ProjectConfigs.cmake)
include(cmake/ProjectInstall.cmake)

add_subdirectory(config)
add_subdirectory(src)
if(VOXER_ENABLE_TESTS)
    add_subdirectory(test)
endif()
