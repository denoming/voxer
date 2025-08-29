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
    FILES ${PROJECT_SOURCE_DIR}/cmake/modules/FindOnnxRuntime.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/voxer
    COMPONENT Voxer_DevLib
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