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

find_package(spdlog REQUIRED)

if (VOXER_ACTIVE_LOG_LEVEL STREQUAL "None")
    message(VERBOSE "Set spdlog log level to 'None'")
    add_compile_definitions(SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_OFF)
elseif(VOXER_ACTIVE_LOG_LEVEL STREQUAL "Debug")
    message(VERBOSE "Set spdlog log level to 'Debug'")
    add_compile_definitions(SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG)
elseif(VOXER_ACTIVE_LOG_LEVEL STREQUAL "Info")
    message(VERBOSE "Set spdlog log level to 'Info'")
    add_compile_definitions(SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_INFO)
elseif(VOXER_ACTIVE_LOG_LEVEL STREQUAL "Warning")
    message(VERBOSE "Set spdlog log level to 'Warn'")
    add_compile_definitions(SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_WARN)
elseif(VOXER_ACTIVE_LOG_LEVEL STREQUAL "Error")
    message(VERBOSE "Set spdlog log level to 'Error'")
    add_compile_definitions(SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_ERROR)
else()
    message(VERBOSE "Set spdlog log level to 'Info' (by default)")
    add_compile_definitions(SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_INFO)
endif()
