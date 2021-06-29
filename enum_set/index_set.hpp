#ifndef ENUM_SET_INDEX_SET_HPP
#define ENUM_SET_INDEX_SET_HPP

#include <enum_set/integer_set.hpp>
#include <enum_set/standard_types.hpp>

namespace enum_set
{

/// Representation of an index as a first class type.
template <size_t Index>
using index = value<size_t, Index>;

/// Creates a `value_set` type of `Size` consecutive indices.
/// Mimics the standard library meta function `std::make_index_sequence`.
template <size_t Size>
using make_index_set = make_integer_set<size_t, Size>;

}  // namespace enum_set

#endif // ENUM_SET_INDEX_SET_HPP
