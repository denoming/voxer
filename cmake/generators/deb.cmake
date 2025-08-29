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

set(CPACK_DEB_COMPONENT_INSTALL ON)

set(CPACK_DEBIAN_ENABLE_COMPONENT_DEPENDS ON)
set(CPACK_DEBIAN_PACKAGE_SECTION "sound")
if(DEFINED VCPKG_INSTALLED_DIR)
    list(APPEND CPACK_DEBIAN_PACKAGE_SHLIBDEPS_PRIVATE_DIRS "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib")
endif()

# Runtime package configuration
set(CPACK_DEBIAN_VOXER_LIB_PACKAGE_NAME "libvoxer")
set(CPACK_DEBIAN_VOXER_LIB_FILE_NAME DEB-DEFAULT)
if (CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    set(CPACK_DEBIAN_VOXER_LIB_DEBUGINFO_PACKAGE ON)
endif()
string(CONCAT CPACK_DEBIAN_VOXER_LIB_PACKAGE_DEPENDS
    "espeak-ng (>= 1.51+dfsg), "
    "espeak-ng-data (>= 1.51+dfsg)"
)
if(VOXER_VOICES_DOWNLOAD)
    set(CPACK_DEBIAN_PACKAGE_RECOMMENDS "libvoxer-data")
endif()

# Development package configuration
set(CPACK_DEBIAN_VOXER_DEVLIB_PACKAGE_NAME "libvoxer-dev")
set(CPACK_DEBIAN_VOXER_DEVLIB_FILE_NAME DEB-DEFAULT)

# Data package configuration
set(CPACK_DEBIAN_VOXER_DATA_PACKAGE_NAME "libvoxer-data")
set(CPACK_DEBIAN_VOXER_DATA_FILE_NAME DEB-DEFAULT)
set(CPACK_DEBIAN_VOXER_DATA__PACKAGE_MULTIARCH "allowed")

# Bin package configuration
set(CPACK_DEBIAN_VOXER_BIN_PACKAGE_NAME "voxer")
set(CPACK_DEBIAN_VOXER_BIN_FILE_NAME DEB-DEFAULT)
set(CPACK_DEBIAN_VOXER_BIN_PACKAGE_DEPENDS "libvoxer")