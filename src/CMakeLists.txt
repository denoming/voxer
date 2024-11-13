# SPDX-License-Identifier: MIT
#
# (c) Copyright 2024
# Denys Asauliak, denoming@gmail.com

include(GNUInstallDirs)
include(GenerateExportHeader)

add_library(voxer SHARED)
add_library(Voxer::Voxer ALIAS voxer)

set_target_properties(voxer
    PROPERTIES
        OUTPUT_NAME voxer
        EXPORT_NAME Voxer
)

set_target_properties(voxer
    PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN YES
        COMPATIBLE_INTERFACE_BOOL FLOAT_SUPPORT
        INTERFACE_FLOAT_SUPPORT YES
)

target_sources(voxer
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS "${PROJECT_SOURCE_DIR}/include"
                  "${CMAKE_CURRENT_BINARY_DIR}"
        FILES "${PROJECT_SOURCE_DIR}/include/voxer/Voxer.hpp"
              "${PROJECT_SOURCE_DIR}/include/voxer/Types.hpp"
              "${PROJECT_SOURCE_DIR}/include/voxer/DataHandler.hpp"
              "${CMAKE_CURRENT_BINARY_DIR}/voxer/VoxerExport.hpp"
    PRIVATE
        Voxer.cpp
        VoxerImpl.cpp
        Model.cpp
        Synthesizer.cpp
        SynthesizerEngine.cpp
        PhonemesMatcher.cpp
        DataHandler.cpp
        DataSaver.cpp
        TextTranslator.cpp
)

target_include_directories(voxer
    PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
           $<BUILD_INTERFACE:${VOXER_STAGE_DIR}>
)

target_link_libraries(voxer
        PRIVATE Voxer::Version
                fmt::fmt
                spdlog::spdlog
                nlohmann_json::nlohmann_json
                utf8cpp::utf8cpp
                uni-algo::uni-algo
                OnnxRuntime::OnnxRuntime
                PkgConfig::eSpeakNg
)

if(VOXER_ENABLE_FORMATTED)
    target_sources(voxer
        PUBLIC
            FILE_SET HEADERS
            FILES "${PROJECT_SOURCE_DIR}/include/voxer/FormattedDataHandler.hpp"
                  "${PROJECT_SOURCE_DIR}/include/voxer/FormattedDataSaver.hpp"
        PRIVATE FormattedDataHandler.cpp
                FormattedDataSaver.cpp
    )
    target_link_libraries(voxer PRIVATE SndFile::sndfile)
endif()

target_compile_features(voxer PUBLIC cxx_std_20)

generate_export_header(voxer
    BASE_NAME VOXER
    EXPORT_FILE_NAME voxer/VoxerExport.hpp
)

install(TARGETS voxer
    EXPORT VoxerTargets
    LIBRARY
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT Voxer_Runtime
        NAMELINK_COMPONENT Voxer_Development
    FILE_SET HEADERS
)

install(TARGETS voxer
    FILE_SET HEADERS
    COMPONENT Voxer_Development
    INCLUDES
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT VoxerTargets
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/voxer
        NAMESPACE Voxer::
        COMPONENT Voxer_Development
        FILE VoxerTargets.cmake
)
