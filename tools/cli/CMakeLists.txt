# SPDX-License-Identifier: MIT
#
# (c) Copyright 2024
# Denys Asauliak, denoming@gmail.com

include(GNUInstallDirs)

set(TARGET voxer-cli)

add_executable(${TARGET} "")
add_executable(Voxer::Voxer::Cli ALIAS ${TARGET})

target_sources(${TARGET}
    PRIVATE
        src/main.cpp
        src/DataOutputStream.cpp
)

target_link_libraries(${TARGET}
    PRIVATE cxxopts::cxxopts
            fmt::fmt
            Voxer::Voxer
)

if(VOXER_ENABLE_FORMATTED)
    target_sources(${TARGET} PRIVATE src/FormattedDataOutputStream.cpp)
endif()

target_compile_features(${TARGET} PRIVATE cxx_std_23)

install(
    TARGETS ${TARGET}
    COMPONENT Voxer_Bin
)
