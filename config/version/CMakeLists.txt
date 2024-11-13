set(TARGET voxer-version)

add_library(${TARGET} STATIC)
add_library(Voxer::Version ALIAS ${TARGET})

find_package(Git)
if (Git_FOUND)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse --short=8 HEAD
        RESULT_VARIABLE result
        OUTPUT_VARIABLE voxer_GIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
endif()

target_sources(${TARGET}
    PRIVATE voxer/Version.hpp
            ${CMAKE_CURRENT_BINARY_DIR}/Version.cpp
)

target_include_directories(${TARGET}
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_compile_features(${TARGET} PUBLIC cxx_std_23)

configure_file(voxer/Version.cpp.in Version.cpp @ONLY)