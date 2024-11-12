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
        src/FormattedDataOutputStream.cpp
)

target_link_libraries(${TARGET}
    PRIVATE cxxopts::cxxopts
            fmt::fmt
            Voxer::Voxer
)

target_compile_features(${TARGET} PRIVATE cxx_std_23)

install(
    TARGETS ${TARGET}
    COMPONENT Voxer_Runtime
)
