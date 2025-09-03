# Copyright 2025 Denys Asauliak
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

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
           $<BUILD_INTERFACE:${VOXER_STAGING_DIR}>
)

target_link_libraries(voxer
        PRIVATE Voxer::Version
                fmt::fmt
                spdlog::spdlog
                nlohmann_json::nlohmann_json
                utf8cpp
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
    target_link_libraries(voxer PRIVATE PkgConfig::SndFile)
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
        COMPONENT Voxer_Lib
        NAMELINK_COMPONENT Voxer_DevLib
    FILE_SET HEADERS
)

install(TARGETS voxer
    FILE_SET HEADERS
    COMPONENT Voxer_DevLib
    INCLUDES
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT VoxerTargets
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/voxer
        NAMESPACE Voxer::
        COMPONENT Voxer_DevLib
        FILE VoxerTargets.cmake
)
