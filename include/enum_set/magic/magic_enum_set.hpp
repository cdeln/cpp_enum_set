#ifndef ENUM_SET_MAGIC_ENUM_SET_HPP
#define ENUM_SET_MAGIC_ENUM_SET_HPP

#include <enum_set/enum_set.hpp>
#include <enum_set/standard_types.hpp>

#include <magic_enum.hpp>

namespace enum_set
{
namespace detail
{

/// Wrapper around `magic_enum::enum_values`,
/// as to explicitly show that this is an implementation detail.
template <typename Enum>
constexpr auto enum_values() noexcept
{
    return ::magic_enum::enum_values<Enum>();
}

}  // namespace detail

/// Factory for building a `value_set` for an enumeration type `Enum`.
/// See magic enum for possible limitations.
template <typename Enum>
struct magic_enum_set_factory
{
    static constexpr auto values = detail::enum_values<Enum>();
    static constexpr auto indices = std::make_index_sequence<values.size()>();

    template <size_t... Indices>
    static constexpr auto make_type(std::index_sequence<Indices...>) noexcept
        -> value_set<Enum, values[Indices]...>;

    using type = decltype(make_type(indices));
};

/// Creates a `value_set` for an enumeration type `Enum`.
/// See magic enum for possible limitations.
template <typename Enum>
using make_magic_enum_set = typename magic_enum_set_factory<Enum>::type;

}  // namespace enum_set

#endif // ENUM_SET_MAGIC_ENUM_SET_HPP
