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

include(FeatureSummary)

option(VOXER_ENABLE_CODE_FORMAT "Enable code formatting" ON)
add_feature_info(
    VOXER_ENABLE_CODE_FORMAT VOXER_ENABLE_CODE_FORMAT "Enable code formatting support"
)

option(VOXER_ENABLE_TESTS "Enable unit tests" OFF)
if(VOXER_ENABLE_TESTS)
    list(APPEND VCPKG_MANIFEST_FEATURES "tests")
endif()
add_feature_info(
    VOXER_ENABLE_TESTS VOXER_ENABLE_TESTS "Build project with enabled unit tests"
)

option(VOXER_ENABLE_CLI "Enable build CLI" OFF)
if(VOXER_ENABLE_CLI)
    list(APPEND VCPKG_MANIFEST_FEATURES "cli")
endif()
add_feature_info(
    VOXER_ENABLE_CLI VOXER_ENABLE_CLI "Build project with enabled CLI"
)

option(VOXER_ENABLE_FORMATTED "Enable formatted audio data support" ON)
if(VOXER_ENABLE_FORMATTED)
    list(APPEND VCPKG_MANIFEST_FEATURES "formatted")
endif()
add_feature_info(
    VOXER_ENABLE_FORMATTED VOXER_ENABLE_FORMATTED "Build project with enabled formatted audio data support"
)

option(VOXER_VOICES_DOWNLOAD "Enable voices downloading" OFF)
add_feature_info(
    VOXER_VOICES_DOWNLOAD VOXER_VOICES_DOWNLOAD "Download voices model and configuration"
)

feature_summary(WHAT ALL)
