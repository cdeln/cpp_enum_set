#ifndef ENUM_SET_ENUM_SET_HPP
#define ENUM_SET_ENUM_SET_HPP

#include <enum_set/integer_set.hpp>
#include <enum_set/standard_types.hpp>

#include <type_traits>
#include <utility>

namespace enum_set
{
namespace detail
{

/// Factory for `make_enum_sequence` meta function below.
/// `Enum` must be of enumeration type.
template <typename Enum, Enum Last>
struct enum_sequence_factory
{
    static_assert(std::is_enum<Enum>::value,
                  "enum_sequence_factory can only be instantiated with an enumeration type");

    using arithmetic_value_type = std::underlying_type_t<Enum>;

    using make_arithmetic_sequence =
        std::make_integer_sequence<
            arithmetic_value_type,
            static_cast<arithmetic_value_type>(Last) + 1
        >;

    template <arithmetic_value_type... Values>
    static std::integer_sequence<Enum, static_cast<Enum>(Values)...>
    make_type(std::integer_sequence<arithmetic_value_type, Values...>);

    using type = decltype(make_type(make_arithmetic_sequence()));
};

/// Creates a sequence of values of an enumeration type ranging from 0 to (including) `Last`.
template <typename Enum, Enum Last>
using make_enum_sequence = typename enum_sequence_factory<Enum, Last>::type;

}  // namespace detail

/// Factory for building a `value_set` for an enumeration type `Enum`, terminated by `Last` value.
/// The enumerations of the `Enum = enum [class] { E_0, E_1, ..., E_N }` is assumed to have default
/// values so that the values of the underlying type is `{0, 1, ..., N }`.
/// Performs the map
///
///     [Enum, Last] -> value_set<Enum, E_0, E_1, ..., E_N>
///
/// `Enum` must be of enumeration type.
/// `Last` must be the last enumeration in the `Enum`.
template <typename Enum, Enum Last>
struct enum_set_factory
{
    template <Enum... Values>
    static constexpr value_set<Enum, Values...>
    make_type(std::integer_sequence<Enum, Values...>);

    using type = decltype(make_type(detail::make_enum_sequence<Enum, Last>()));
};

/// Creates a `value_set` for an enumeration type `Enum`, terminated by `Last` value.
/// See `enum_set_factory` for details.
template <typename Enum, Enum Last>
using make_enum_set = typename enum_set_factory<Enum, Last>::type;

}  // namespace enum_set

#endif // ENUM_SET_ENUM_SET_HPP
