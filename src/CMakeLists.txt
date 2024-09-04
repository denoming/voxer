include(GNUInstallDirs)
include(GenerateExportHeader)
include(CMakePackageConfigHelpers)

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
    PRIVATE
        Voxer.cpp
        VoxerImpl.cpp
)

target_include_directories(${TARGET}
    PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
           $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

target_link_libraries(${TARGET}
        PRIVATE fmt::fmt
                spdlog::spdlog
                Piper::Phonemize
)

target_compile_features(${TARGET} PUBLIC cxx_std_17)

# Generate export header file with export macros
generate_export_header(${TARGET}
    BASE_NAME VOXER
    EXPORT_FILE_NAME VoxerExport.hpp
)

# Install target files except namelink file
install(TARGETS ${TARGET}
    EXPORT VoxerRuntime
    LIBRARY
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT VoxerRuntime
        NAMELINK_SKIP
    INCLUDES
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install target namelink file
install(TARGETS ${TARGET}
    EXPORT VoxerDevelopment
    LIBRARY
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT VoxerDevelopment
        NAMELINK_ONLY
    ARCHIVE
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT VoxerDevelopment
    INCLUDES
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install package config file
install(EXPORT VoxerRuntime
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/voxer
    NAMESPACE My::
    COMPONENT VoxerRuntime
    FILE VoxerRuntime.cmake
)

# Install package config file
install(EXPORT VoxerDevelopment
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/voxer
    NAMESPACE My::
    COMPONENT VoxerDevelopment
    FILE VoxerDevelopment.cmake
)

# Install library header files
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/
    TYPE INCLUDE
    COMPONENT VoxerDevelopment
    FILES_MATCHING
        PATTERN *.hpp
)

# Install export header file
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/CalculatorExport.hpp
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/voxer
    COMPONENT VoxerDevelopment
)

# Generate package config and version file
configure_package_config_file(
    ${PROJECT_SOURCE_DIR}/config/VoxerConfig.cmake.in
    VoxerConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/voxer
    NO_SET_AND_CHECK_MACRO
    NO_CHECK_REQUIRED_COMPONENTS_MACRO
)
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/VoxerConfigVersion.cmake
    COMPATIBILITY SameMajorVersion
)

# Install package config and version file
install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/VoxerConfig.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/voxer
    COMPONENT VoxerDevelopment
)
install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/VoxerConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/voxer
    COMPONENT VoxerDevelopment
)
