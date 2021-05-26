cmake_minimum_required(VERSION 3.14)

project(enum_setExamples CXX)

include(../cmake/project-is-top-level.cmake)

if(PROJECT_IS_TOP_LEVEL)
  find_package(enum_set REQUIRED)
endif()

add_custom_target(run_examples)

function(add_example NAME)
  cmake_parse_arguments(PARSE_ARGV 1 "" "" "" "SOURCES;LIBS")
  if("${_SOURCES}" STREQUAL "")
    set(_SOURCES "${NAME}.cpp")
  endif()
  add_executable("${NAME}" ${_SOURCES})
  target_link_libraries("${NAME}" PRIVATE enum_set::enum_set ${_LIBS})
  add_custom_target(
      "run_${NAME}"
      COMMAND "$<TARGET_FILE:${NAME}>"
      VERBATIM
  )
  add_dependencies(run_examples "run_${NAME}")
endfunction()

add_example(basic_tutorial)
add_example(rationale)
add_example(visitation_example)
# Transitive dependency we get from the find_dependency() command
if(TARGET magic_enum::magic_enum)
  add_example(
      magic_enum_set_example
      SOURCES "${PROJECT_SOURCE_DIR}/magic/magic_enum_set_example.cpp"
      LIBS magic_enum::magic_enum
  )
endif()
