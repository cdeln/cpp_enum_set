#ifndef ENUM_SET_INTEGER_SET_HPP
#define ENUM_SET_INTEGER_SET_HPP

#include <enum_set/standard_types.hpp>
#include <enum_set/value_set.hpp>

#include <type_traits>
#include <utility>

namespace enum_set
{

/// Factory for building a `value_set` for a linear sequence of `Integer` types of a certain `Size`.
/// Performs the map
///
///     [Integer, Size] -> value_set<Integer, 0, 1, ..., Size - 1>
///
/// `Integer` must be of integral type.
/// `Size` must be strictly positive.
template <typename Integer, size_t Size>
struct integer_set_factory
{
    static_assert(std::is_integral<Integer>::value, "Type must be an integer");
    static_assert(Size > 0, "Size must be strictly positive");

    // Helper method for unpacking a `std::integer_sequence` into a `value_set`.
    template <Integer... Values>
    static constexpr value_set<Integer, Values...>
    make_type(std::integer_sequence<Integer, Values...>);

    using type = decltype(make_type(std::make_integer_sequence<Integer, Size>()));
};

/// Makes a `value_set` type of `Size` consecutive integer values of type `Integer`.
template <typename Integer, size_t Size>
using make_integer_set = typename integer_set_factory<Integer, Size>::type;

}  // namespace enum_set

#endif // ENUM_SET_INTEGER_SET_HPP
