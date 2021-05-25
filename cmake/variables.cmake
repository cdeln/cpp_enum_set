# ---- Developer mode ----

# Developer mode enables targets and code paths in the CMake scripts that are
# only relevant for the developer(s) of enum_set
# Targets necessary to build the project must be provided unconditionally, so
# consumers can trivially build and package the project
if(PROJECT_IS_TOP_LEVEL)
  option(enum_set_DEVELOPER_MODE "Enable developer mode" OFF)
endif()

# ---- Warning guard ----

# target_include_directories with the SYSTEM modifier will request the compiler
# to omit warnings from the provided paths, if the compiler supports that
# This is to provide a user experience similar to find_package when
# add_subdirectory or FetchContent is used to consume this project
set(enum_set_warning_guard "")
if(NOT PROJECT_IS_TOP_LEVEL)
  option(
      enum_set_INCLUDES_WITH_SYSTEM
      "Use SYSTEM modifier for enum_set's includes, disabling warnings"
      ON
  )
  mark_as_advanced(enum_set_INCLUDES_WITH_SYSTEM)
  if(enum_set_INCLUDES_WITH_SYSTEM)
    set(enum_set_warning_guard SYSTEM)
  endif()
endif()

# ---- magic_enum ----

# Detect the presence of magic_enum if the variable wasn't defined by the user
# already, so the variable can be automatically set in CI
set(value OFF)
if(NOT DEFINED enum_set_USE_MAGIC_ENUM)
  find_package(magic_enum QUIET)
  if(magic_enum_FOUND)
    set(value ON)
    message(
        STATUS
        "enum_set_USE_MAGIC_ENUM was not defined and magic_enum was found."
        " Building with magic_enum support."
    )
  else()
    message(
        STATUS
        "enum_set_USE_MAGIC_ENUM was not defined and magic_enum was not found."
        " Building without magic_enum support."
    )
  endif()
endif()

option(
    enum_set_USE_MAGIC_ENUM
    "Enable usage of magic_enum. Requires C++17"
    "${value}"
)

# If this project is vendored, then propagate this variable like the installed
# package does
if(NOT PROJECT_IS_TOP_LEVEL)
  set(
      enum_set_HAS_MAGIC_ENUM_SUPPORT
      "${enum_set_USE_MAGIC_ENUM}" PARENT_SCOPE
  )
endif()
