# Copyright 2025 Denys Asauliak
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

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