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

add_executable(voxer-unit-tests "")

set_target_properties(voxer-unit-tests
    PROPERTIES
        OUTPUT_NAME voxer-unit-tests
)

target_sources(voxer-unit-tests
    PRIVATE src/TextToAudioTest.cpp
)

target_link_libraries(
    voxer-unit-tests
    PRIVATE Voxer::Voxer
            GTest::gtest_main
            GTest::gmock_main
)

if(VOXER_ENABLE_FORMATTED)
    target_sources(voxer-unit-tests
        PRIVATE src/TextToFileTest.cpp
                src/TextToPlaybackTest.cpp
    )
    target_link_libraries(voxer-unit-tests
        PRIVATE PkgConfig::Gst
                PkgConfig::GstApp
    )
endif()

if (NOT CMAKE_CROSSCOMPILING)
    gtest_discover_tests(voxer-unit-tests TEST_LIST testList)
    set_property(
        DIRECTORY APPEND
        PROPERTY TEST_INCLUDE_FILES ${CMAKE_CURRENT_LIST_DIR}/setTestsOptions.cmake
    )
endif()
