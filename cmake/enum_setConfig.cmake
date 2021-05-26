# When installed with magic_enum support, the logic here allows the lack of
# magic_enum to gracefully fall back to not using it and operate only in C++14
# mode, without the magic_enum dependency

set(enum_set_HAS_MAGIC_ENUM_SUPPORT "@enum_set_USE_MAGIC_ENUM@")

if(enum_set_HAS_MAGIC_ENUM_SUPPORT)
  include(CMakeFindDependencyMacro)
  find_dependency(magic_enum)
endif()

include("${CMAKE_CURRENT_LIST_DIR}/enum_setTargets.cmake")

if(enum_set_HAS_MAGIC_ENUM_SUPPORT AND magic_enum_FOUND)
  set_target_properties(
      enum_set::enum_set PROPERTIES
      INTERFACE_COMPILE_FEATURES cxx_std_17
      INTERFACE_LINK_LIBRARIES magic_enum::magic_enum
  )
else()
  set_property(
      TARGET enum_set::enum_set PROPERTY
      INTERFACE_COMPILE_FEATURES cxx_std_14
  )
endif()
