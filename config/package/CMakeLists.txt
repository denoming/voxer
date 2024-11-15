include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

configure_package_config_file(
        ${CMAKE_CURRENT_LIST_DIR}/VoxerConfig.cmake.in
        VoxerConfig.cmake
    INSTALL_DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/cmake/voxer
    NO_SET_AND_CHECK_MACRO
    NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/VoxerConfigVersion.cmake
    COMPATIBILITY SameMajorVersion
)

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/VoxerConfig.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/voxer
    COMPONENT Voxer_DevLib
)

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/VoxerConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/voxer
    COMPONENT Voxer_DevLib
)