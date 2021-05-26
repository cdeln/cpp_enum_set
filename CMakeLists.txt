cmake_minimum_required(VERSION 3.14)

include(cmake/in-source-guard.cmake)

project(
    enum_set
    VERSION 0.1.0
    DESCRIPTION "A library of type safe sets over fixed size collections of types or values, including methods for accessing, modifying, visiting and iterating over those."
    HOMEPAGE_URL "https://github.com/cdeln/cpp_enum_set"
    LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Declare library ----

add_library(enum_set_enum_set INTERFACE)
add_library(enum_set::enum_set ALIAS enum_set_enum_set)

set_property(
    TARGET enum_set_enum_set PROPERTY
    EXPORT_NAME enum_set
)

target_include_directories(
    enum_set_enum_set
    ${enum_set_warning_guard}
    INTERFACE
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

# The following properties are only applied during build time
# For details, check the config file
set(std cxx_std_14)
if(enum_set_USE_MAGIC_ENUM)
  set(std cxx_std_17)
  find_package(magic_enum REQUIRED)
  target_link_libraries(
      enum_set_enum_set
      INTERFACE
      "$<BUILD_INTERFACE:magic_enum::magic_enum>"
  )
endif()

target_compile_features(
    enum_set_enum_set
    INTERFACE
    "$<BUILD_INTERFACE:${std}>"
)

# ---- Install rules ----

include(cmake/install-rules.cmake)

# ---- Examples ----

if(PROJECT_IS_TOP_LEVEL)
  option(BUILD_EXAMPLES "Build examples tree." "${enum_set_DEVELOPER_MODE}")
  if(BUILD_EXAMPLES)
    add_subdirectory(example)
  endif()
endif()

# ---- Developer mode ----

if(NOT enum_set_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of enum_set"
  )
endif()

include(CTest)
if(BUILD_TESTING)
  add_subdirectory(test)
endif()

option(BUILD_DOCUMENTATION "Build documentation using Doxygen and m.css" OFF)
if(BUILD_DOCUMENTATION)
  include(cmake/docs.cmake)
endif()
