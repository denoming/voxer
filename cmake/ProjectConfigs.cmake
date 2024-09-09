# SPDX-License-Identifier: MIT
#
# (c) Copyright 2024
# Denys Asauliak, denoming@gmail.com

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/modules")

if (EXISTS "$ENV{HOME}/.local")
    list(APPEND CMAKE_PREFIX_PATH $ENV{HOME}/.local)
endif()
if (DEFINED CMAKE_TOOLCHAIN_FILE)
    message(STATUS "Toolchain file ${CMAKE_TOOLCHAIN_FILE}")
endif()

include(BuildType)
include(BuildLocation)
include(BuildOptions)

if (VOXER_ENABLE_TESTS)
    enable_testing()
    include(AddGoogleTest)
    message(VERBOSE "Building with tests")
else()
    message(VERBOSE "Building without tests")
endif()

include(AddPiperPhonemize)
include(AddFmt)
include(AddSpdLog)
include(AddNlohmannJson)
include(AddUtfCpp)
if(VOXER_ENABLE_FILE_SAVING)
    include(AddLibSndFile)
endif()
