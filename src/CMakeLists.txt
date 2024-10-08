# SPDX-License-Identifier: MIT
#
# (c) Copyright 2024
# Denys Asauliak, denoming@gmail.com

include(GNUInstallDirs)
include(GenerateExportHeader)

set(TARGET voxer)

add_library(${TARGET} SHARED)
add_library(Voxer::Voxer ALIAS ${TARGET})

set_target_properties(${TARGET}
    PROPERTIES
        OUTPUT_NAME voxer
        EXPORT_NAME Voxer
)

set_target_properties(${TARGET}
    PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN YES
        COMPATIBLE_INTERFACE_BOOL FLOAT_SUPPORT
        INTERFACE_FLOAT_SUPPORT YES
)

target_sources(${TARGET}
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
        SpeakNgLoader.cpp
        DataHandler.cpp
)

target_include_directories(${TARGET}
    PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
           $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

target_link_libraries(${TARGET}
        PRIVATE fmt::fmt
                spdlog::spdlog
                nlohmann_json::nlohmann_json
                utf8::cpp utf8cpp::utf8cpp
                Piper::Phonemize
)

if(VOXER_ENABLE_FORMATTED)
    target_sources(${TARGET}
        PUBLIC
            FILE_SET HEADERS
            FILES "${PROJECT_SOURCE_DIR}/include/voxer/FormattedDataHandler.hpp"
                  "${PROJECT_SOURCE_DIR}/include/voxer/FormattedDataSaver.hpp"
        PRIVATE FormattedDataHandler.cpp
                FormattedDataSaver.cpp
    )
    target_link_libraries(${TARGET} PRIVATE SndFile::sndfile)
endif()

target_compile_features(${TARGET} PUBLIC cxx_std_20)

generate_export_header(${TARGET}
    BASE_NAME VOXER
    EXPORT_FILE_NAME voxer/VoxerExport.hpp
)

install(TARGETS ${TARGET}
    EXPORT VoxerTargets
    LIBRARY
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT VoxerRuntime
        NAMELINK_COMPONENT VoxerDevelopment
    FILE_SET HEADERS
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT VoxerTargets
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/voxer
        NAMESPACE Voxer::
        COMPONENT VoxerRuntime
        FILE VoxerTargets.cmake
)
