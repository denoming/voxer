# SPDX-License-Identifier: MIT
#
# (c) Copyright 2024
# Denys Asauliak, denoming@gmail.com

include(FeatureSummary)

option(VOXER_ENABLE_TESTS "Enable testing" ON)
if(VOXER_ENABLE_TESTS)
    list(APPEND VCPKG_MANIFEST_FEATURES "tests")
endif()
add_feature_info(
    VOXER_ENABLE_TESTS VOXER_ENABLE_TESTS "Build project with tests"
)

option(VOXER_ENABLE_FILE_SAVING "Enable file saving support" ON)
if(VOXER_ENABLE_FILE_SAVING)
    list(APPEND VOXER_ENABLE_FILE_SAVING "files-saving")
endif()
add_feature_info(
    VOXER_ENABLE_FILE_SAVING VOXER_ENABLE_FILE_SAVING "Build project with tests"
)

feature_summary(WHAT ALL)
