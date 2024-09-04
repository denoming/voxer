set(TARGET voxer-unit-tests)

add_executable(${TARGET} "")

set_target_properties(${TARGET}
    PROPERTIES
        OUTPUT_NAME voxer-unit-tests
)

target_sources(${TARGET}
    PRIVATE src/test.cpp
)

target_link_libraries(
    ${TARGET}
    PRIVATE Voxer::Voxer
            GTest::gtest_main GTest::gmock_main
)

if (NOT CMAKE_CROSSCOMPILING)
    gtest_discover_tests(${TARGET} TEST_LIST testList)
    set_property(
        DIRECTORY APPEND
        PROPERTY TEST_INCLUDE_FILES ${CMAKE_CURRENT_LIST_DIR}/setTestsOptions.cmake
    )
endif()
