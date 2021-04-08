#ifndef ENUM_SET_COMMON_HPP
#define ENUM_SET_COMMON_HPP

#include <enum_set/standard_types.hpp>

#include <stdexcept>
#include <type_traits>

namespace enum_set
{
namespace detail
{

/// Base case of `all(bool...)` below.
constexpr bool all() noexcept
{
    return true;
}

/// Returns `true` if all arguments are `true`, otherwise `false`.
/// Can be replaced by a built in fold expression in C++17.
template <typename... Bools>
constexpr bool all(bool first, Bools... rest) noexcept
{
    return first && all(rest...);
}

/// Base case of `any(bool...)` below.
constexpr bool any() noexcept
{
    return false;
}

/// Returns `true` if any argument is `true`, otherwise `false`.
/// Can be replaced by a built in fold expression in C++17.
template <typename... Bools>
constexpr bool any(bool first, Bools... rest) noexcept
{
    return first || any(rest...);
}

/// Base case for `count(bool...)` below.
constexpr size_t count() noexcept
{
    return 0;
}

/// Returns the number of trues in a set of bools.
template <typename... Bools>
constexpr size_t count(bool first, Bools... rest) noexcept
{
    return (first ? 1 : 0) + count(rest...);
}

/// Type trait for getting the index of a type `T` in a list of types `[U, Us...]`.
/// If the type `T` is not present in the type list `[U, Us...]`,
/// then the index is defined as the length of the type list.
/// If there exists duplicate entries, the result is the index of the first occurence of `T`.
template <typename T, typename U, typename... Us>
struct index_of
{
    static constexpr size_t value
    {
        std::is_same<T, U>::value
            ? 0
            : 1 + index_of<T, Us...>::value
    };
};

/// Base case of `index_of` for a type list of length 1.
template <typename T, typename U>
struct index_of<T, U>
{
    static constexpr size_t value{std::is_same<T, U>::value ? 0 : 1};
};

/// Base case for `index_of_value` below when the list of values is empty.
template <typename Type>
constexpr size_t index_of_value(Type) noexcept
{
    return 0;
}

/// Returns the index of a `value` in a compile time list of values `[First, Rest...]`.
/// If the `value` is not present in the list, the length of the list is returned.
template <typename Type, Type First, Type... Rest>
constexpr size_t index_of_value(Type value) noexcept
{
    if (value == First)
    {
        return 0;
    }
    return 1 + index_of_value<Type, Rest...>(value);
}

/// Base case for `get_value` below when the list of values is empty.
/// Calling this indicates a programming error and throws an exception.
template <typename Type>
constexpr Type get_value(size_t)
{
    throw std::out_of_range("value_set get_value out of range");
}

/// Gets the value at `index` in the compile time list of values `[First, Rest...]`.
/// If the `index` is out of bounds, an exception is thrown.
/// Complexity is linear in the length of the list of values.
template <typename Type, Type First, Type... Rest>
constexpr Type get_value(size_t index)
{
    return (index == 0) ? First : get_value<Type, Rest...>(index - 1);
}

}  // namespace detail
}  // namespace enum_set

#endif // ENUM_SET_COMMON_HPP
