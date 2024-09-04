include(FeatureSummary)

option(VOXER_ENABLE_TESTS "Enable testing" ON)
if(ENABLE_TESTS)
    list(APPEND VCPKG_MANIFEST_FEATURES "tests")
endif()
add_feature_info(
    ENABLE_TESTS VOXER_ENABLE_TESTS "Build project with tests"
)

feature_summary(WHAT ALL)
