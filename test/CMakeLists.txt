# SPDX-License-Identifier: MIT
#
# (c) Copyright 2024
# Denys Asauliak, denoming@gmail.com

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
            GTest::gtest_main GTest::gmock_main
)

if(VOXER_ENABLE_FORMATTED)
    target_sources(voxer-unit-tests
        PRIVATE src/TextToFileTest.cpp
                src/TextToPlaybackTest.cpp
    )
target_link_libraries(
    voxer-unit-tests
    PRIVATE PkgConfig::Gst
            PkgConfig::GstApp
)
endif()

if (NOT CMAKE_CROSSCOMPILING)
    gtest_discover_tests(${TARGET} TEST_LIST testList)
    set_property(
        DIRECTORY APPEND
        PROPERTY TEST_INCLUDE_FILES ${CMAKE_CURRENT_LIST_DIR}/setTestsOptions.cmake
    )
endif()
