set(CPACK_DEB_COMPONENT_INSTALL ON)

set(CPACK_DEBIAN_ENABLE_COMPONENT_DEPENDS ON)
set(CPACK_DEBIAN_PACKAGE_SECTION "sound")
if(DEFINED VCPKG_INSTALLED_DIR)
    list(APPEND CPACK_DEBIAN_PACKAGE_SHLIBDEPS_PRIVATE_DIRS "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib")
endif()

# Runtime package configuration
set(CPACK_DEBIAN_VOXER_RUNTIME_PACKAGE_NAME "libvoxer")
set(CPACK_DEBIAN_VOXER_RUNTIME_FILE_NAME DEB-DEFAULT)
if (CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    set(CPACK_DEBIAN_VOXER_RUNTIME_DEBUGINFO_PACKAGE ON)
endif()
set(CPACK_DEBIAN_VOXER_RUNTIME_PACKAGE_DEPENDS "espeak-ng-data (>= 1.51+dfsg)")
set(CPACK_DEBIAN_VOXER_RUNTIME_PACKAGE_SHLIBDEPS ON)
set(CPACK_DEBIAN_VOXER_RUNTIME_PACKAGE_GENERATE_SHLIBS ON)
if(VOXER_VOICES_DOWNLOAD)
    set(CPACK_DEBIAN_PACKAGE_RECOMMENDS "libvoxer-data")
endif()

# Development package configuration
set(CPACK_DEBIAN_VOXER_DEVELOPMENT_PACKAGE_NAME "libvoxer-dev")
set(CPACK_DEBIAN_VOXER_DEVELOPMENT_FILE_NAME DEB-DEFAULT)
set(CPACK_DEBIAN_VOXER_DEVELOPMENT_PACKAGE_SHLIBDEPS ON)
set(CPACK_DEBIAN_VOXER_DEVELOPMENT_PACKAGE_GENERATE_SHLIBS ON)

# Data package configuration
set(CPACK_DEBIAN_VOXER_DATA_PACKAGE_NAME "libvoxer-data")
set(CPACK_DEBIAN_VOXER_DATA_FILE_NAME DEB-DEFAULT)
set(CPACK_DEBIAN_VOXER_DATA__PACKAGE_MULTIARCH "allowed")
