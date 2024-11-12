# SPDX-License-Identifier: MIT
#
# (c) Copyright 2024
# Denys Asauliak, denoming@gmail.com

#.rst:
# FindOnnxRuntime
# ---------------
#
# Find the Onnx runtime
#
# Targets
# ^^^^^^^
#
# If successful, the following import target is created.
#
# ``OnnxRuntime::OnnxRuntime``
#
# Cache variables
# ^^^^^^^^^^^^^^^
#
# The following cache variable may also be set to assist/control the operation of this module:
#
# ``OnnxRuntime_ROOT_DIR``
#  The root to search for Onnx runtime.
#

include(FeatureSummary)
set_package_properties(
    OnnxRuntime PROPERTIES
    URL "https://onnxruntime.ai/"
    DESCRIPTION "Machine learning runtime"
)

set(OnnxRuntime_ROOT_DIR "${OnnxRuntime_ROOT_DIR}"
    CACHE PATH "Root to search Onnx Runtime Library")

find_package(PkgConfig)
pkg_check_modules(PC_OnnxRuntime QUIET libonnxruntime)

find_library(OnnxRuntime_LIBRARY
    NAMES onnxruntime
    PATHS ${OnnxRuntime_ROOT_DIR}
    PATH_SUFFIXES lib lib64
    HINTS ${PC_OnnxRuntime_LIBRARY_DIRS}
)
find_path(OnnxRuntime_INCLUDE_DIR
    NAMES onnxruntime_cxx_api.h
    PATHS ${OnnxRuntime_ROOT_DIR}
    PATH_SUFFIXES onnxruntime include include/onnxruntime
    HINTS ${PC_OnnxRuntime_INCLUDE_DIRS}
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(OnnxRuntime
    REQUIRED_VARS OnnxRuntime_INCLUDE_DIR
                  OnnxRuntime_LIBRARY
)

if(OnnxRuntime_FOUND)
    set(OnnxRuntime_INCLUDE_DIRS ${OnnxRuntime_INCLUDE_DIR})
    set(OnnxRuntime_LIBRARIES "${OnnxRuntime_LIBRARY}")
    if(NOT TARGET OnnxRuntime::OnnxRuntime)
        add_library(OnnxRuntime::OnnxRuntime UNKNOWN IMPORTED)
    endif()
    set_target_properties(OnnxRuntime::OnnxRuntime
        PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${OnnxRuntime_INCLUDE_DIRS}"
    )
    set_target_properties(OnnxRuntime::OnnxRuntime
        PROPERTIES
            IMPORTED_LINK_INTERFACE_LANGUAGES "C"
            IMPORTED_LOCATION "${OnnxRuntime_LIBRARY}"
    )
    mark_as_advanced(OnnxRuntime_INCLUDE_DIR OnnxRuntime_LIBRARY)
endif()

mark_as_advanced(OnnxRuntime_ROOT_DIR)