if(PROJECT_IS_TOP_LEVEL)
  set(CMAKE_INSTALL_INCLUDEDIR include/enum_set CACHE PATH "")
endif()

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

set(filter PATTERN magic EXCLUDE)
if(enum_set_USE_MAGIC_ENUM)
  set(filter "")
endif()

install(
    DIRECTORY "${PROJECT_SOURCE_DIR}/enum_set/"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    COMPONENT enum_set_Development
    ${filter}
)

install(
    TARGETS enum_set_enum_set
    EXPORT enum_setTargets
    INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

write_basic_package_version_file(
    enum_setConfigVersion.cmake
    COMPATIBILITY SameMajorVersion
    ARCH_INDEPENDENT
)

configure_file(
    "${PROJECT_SOURCE_DIR}/cmake/enum_setConfig.cmake"
    "${PROJECT_BINARY_DIR}/enum_setConfig.cmake"
    @ONLY
)

# Allow package maintainers to freely override the path for the configs
set(
    enum_set_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/enum_set"
    CACHE STRING "CMake package config location relative to the install prefix"
)
mark_as_advanced(enum_set_INSTALL_CMAKEDIR)

install(
    FILES
    "${PROJECT_BINARY_DIR}/enum_setConfig.cmake"
    "${PROJECT_BINARY_DIR}/enum_setConfigVersion.cmake"
    DESTINATION "${enum_set_INSTALL_CMAKEDIR}"
    COMPONENT enum_set_Development
)

install(
    EXPORT enum_setTargets
    NAMESPACE enum_set::
    DESTINATION "${enum_set_INSTALL_CMAKEDIR}"
    COMPONENT enum_set_Development
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
