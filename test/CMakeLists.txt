# SPDX-License-Identifier: MIT
#
# (c) Copyright 2024
# Denys Asauliak, denoming@gmail.com

set(TARGET voxer-unit-tests)

add_executable(${TARGET} "")

set_target_properties(${TARGET}
    PROPERTIES
        OUTPUT_NAME voxer-unit-tests
)

target_sources(${TARGET}
    PRIVATE src/TextToAudioTest.cpp
)

target_link_libraries(
    ${TARGET}
    PRIVATE Voxer::Voxer
            GTest::gtest_main GTest::gmock_main
)

if(VOXER_ENABLE_FORMATTED)
    target_sources(${TARGET}
        PRIVATE src/TextToFileTest.cpp
                src/TextToPlaybackTest.cpp
    )
target_link_libraries(
    ${TARGET}
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
