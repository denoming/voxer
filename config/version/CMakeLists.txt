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

add_library(voxer-version STATIC)
add_library(Voxer::Version ALIAS voxer-version)

find_package(Git)
if (Git_FOUND)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse --short=8 HEAD
        RESULT_VARIABLE result
        OUTPUT_VARIABLE voxer_GIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
endif()

target_sources(voxer-version
    PRIVATE voxer/Version.hpp
            ${CMAKE_CURRENT_BINARY_DIR}/Version.cpp
)

target_include_directories(voxer-version
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_compile_features(voxer-version PUBLIC cxx_std_23)

configure_file(voxer/Version.cpp.in Version.cpp @ONLY)