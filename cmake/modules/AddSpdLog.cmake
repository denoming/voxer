# SPDX-License-Identifier: MIT
#
# (c) Copyright 2024
# Denys Asauliak, denoming@gmail.com

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
