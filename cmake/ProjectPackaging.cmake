include(CPackComponent)

# Set default packaging install prefix path
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
endif()

set(CPACK_PACKAGE_NAME "voxer")
set(CPACK_PACKAGE_CONTACT "maintainer@denoming.com")
set(CPACK_PACKAGE_VENDOR "DENOMING")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Voxer Project")
set(CPACK_PACKAGE_INSTALL_DIRECTORY ${CPACK_PACKAGE_NAME})
set(CPACK_COMPONENTS_ALL
    Voxer_Lib
    Voxer_DevLib
)
set(CPACK_COMPONENTS_GROUPING ONE_PER_GROUP)
set(CPACK_VERBATIM_VARIABLES YES)
set(CPACK_SOURCE_GENERATOR "TGZ")
set(CPACK_SOURCE_IGNORE_FILES
    /\\.git/
    /\\.idea/
    /build.*/
)

set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})

if(EXISTS "${PROJECT_SOURCE_DIR}/DESCRIPTION.md")
    set(CPACK_PACKAGE_DESCRIPTION_FILE "${PROJECT_SOURCE_DIR}/DESCRIPTION.md")
endif()
if(EXISTS "${PROJECT_SOURCE_DIR}/WELCOME.md")
    set(CPACK_RESOURCE_FILE_WELCOME "${PROJECT_SOURCE_DIR}/WELCOME.md")
endif()
if (EXISTS "${PROJECT_SOURCE_DIR}/LICENSE.md")
    set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE.md")
endif()
if (EXISTS "${PROJECT_SOURCE_DIR}/README.md")
    set(CPACK_RESOURCE_FILE_README "${PROJECT_SOURCE_DIR}/README.md")
endif()

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/config/generators")
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(CPACK_GENERATOR "TGZ;DEB")
    include(tgzFormat)
    include(debFormat)
else()
    set(CPACK_GENERATOR "TGZ")
    include(tgzFormat)
endif()

cpack_add_component(Voxer_Lib
    DISPLAY_NAME Runtime
    DESCRIPTION "Voxer runtime library"
    REQUIRED
)

cpack_add_component(Voxer_DevLib
    DISPLAY_NAME Development
    DESCRIPTION "Voxer develop library"
    DEPENDS Voxer_Lib
    REQUIRED
)

if(VOXER_VOICES_DOWNLOAD)
    list(APPEND CPACK_COMPONENTS_ALL Voxer_Data)
    cpack_add_component(Voxer_Data
        DISPLAY_NAME Data
        DESCRIPTION "Voxer data files"
        DEPENDS Voxer_Lib
        REQUIRED
    )
endif()

if(CPACK_COMPONENTS_ALL)
    list(APPEND CPACK_COMPONENTS_ALL Voxer_Bin)
    cpack_add_component(Voxer_Bin
        DISPLAY_NAME Binaries
        DESCRIPTION "Voxer bin files"
        DEPENDS Voxer_Bin
        REQUIRED
    )
endif()

include(CPack)
