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

option(VOXER_ENABLE_FORMATTED "Enable formatted audio data support" ON)
if(VOXER_ENABLE_FORMATTED)
    list(APPEND VOXER_ENABLE_FORMATTED "formatted")
endif()
add_feature_info(
    VOXER_ENABLE_FORMATTED VOXER_ENABLE_FORMATTED "Build project with formatted audio data support"
)

feature_summary(WHAT ALL)
