# SPDX-License-Identifier: MIT
#
# (c) Copyright 2024
# Denys Asauliak, denoming@gmail.com

include(FeatureSummary)

option(VOXER_ENABLE_TESTS "Enable unit tests" ON)
if(VOXER_ENABLE_TESTS)
    list(APPEND VCPKG_MANIFEST_FEATURES "tests")
endif()
add_feature_info(
    VOXER_ENABLE_TESTS VOXER_ENABLE_TESTS "Build project with enabled unit tests"
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
