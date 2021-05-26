cmake_minimum_required(VERSION 3.14)

project(enum_setTests LANGUAGES CXX)

include(../cmake/project-is-top-level.cmake)

if(PROJECT_IS_TOP_LEVEL)
  find_package(enum_set REQUIRED)
  enable_testing()
endif()

find_package(doctest REQUIRED)
include(doctest)

function(create_test NAME)
  cmake_parse_arguments(PARSE_ARGV 1 "" "" "" "SOURCES;LIBS")
  if("${_SOURCES}" STREQUAL "")
    set(_SOURCES "${NAME}.cpp")
  endif()
  add_executable("${NAME}" ${_SOURCES})
  target_include_directories("${NAME}" PRIVATE "${PROJECT_SOURCE_DIR}/include")
  target_compile_definitions(
      "${NAME}"
      PRIVATE
      DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
  )
  target_link_libraries(
      "${NAME}"
      PRIVATE
      enum_set::enum_set
      doctest::doctest
      ${_LIBS}
  )
  doctest_discover_tests("${NAME}")
endfunction()

create_test(test_bit_mask)
create_test(test_common)
create_test(test_enum_set)
create_test(test_index_set)
create_test(test_iterator)
create_test(test_type_set)
create_test(test_value_set)
# Transitive dependency we get from the find_dependency() command
if(TARGET magic_enum::magic_enum)
  create_test(
      test_magic_enum_set
      SOURCES "${PROJECT_SOURCE_DIR}/magic/test_magic_enum_set.cpp"
      LIBS magic_enum::magic_enum
  )
endif()
