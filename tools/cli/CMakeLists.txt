# SPDX-License-Identifier: MIT
#
# (c) Copyright 2024
# Denys Asauliak, denoming@gmail.com

include(GNUInstallDirs)

add_executable(voxer-cli "")
add_executable(Voxer::Voxer::Cli ALIAS voxer-cli)

target_sources(voxer-cli
    PRIVATE
        src/main.cpp
        src/DataOutputStream.cpp
)

target_link_libraries(voxer-cli
    PRIVATE cxxopts::cxxopts
            fmt::fmt
            Voxer::Voxer
)

if(VOXER_ENABLE_FORMATTED)
    target_sources(voxer-cli PRIVATE src/FormattedDataOutputStream.cpp)
endif()

target_compile_features(voxer-cli PRIVATE cxx_std_23)

install(
    TARGETS voxer-cli
    COMPONENT Voxer_Bin
)
