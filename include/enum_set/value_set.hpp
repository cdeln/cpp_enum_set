#ifndef ENUM_SET_VALUE_SET_HPP
#define ENUM_SET_VALUE_SET_HPP

#include <enum_set/common.hpp>
#include <enum_set/standard_types.hpp>
#include <enum_set/type_set.hpp>

#include <utility>

namespace enum_set
{

/// Representation of a value as a first class type.
template <typename Type, Type Value>
struct value;

/// Represents a set of values from a fixed non-empty universe of `Values...`.
/// The class adapts a `type_set` with value semantics by decorating all methods to
/// accept values instead of types (which then maps a `Value` of `Type` to `value<Type, Value>`).
/// Documentation for all methods can be found in `type_set.hpp`, just replace any occurence
/// of a type with value and it should be valid for this class as well.
template <typename Type, Type... Values>
class value_set : public type_set<value<Type, Values>...>
{
    static_assert(sizeof...(Values) > 0, "value set values must be non-empty");
public:
    using base_type = type_set<value<Type, Values>...>;

    /// Forward declaration of an iterator class.
    /// See `value_set_iterator.hpp` for details.
    class iterator;

    // The usual suspects
    constexpr value_set() noexcept                              = default;
    constexpr value_set(value_set const&) noexcept              = default;
    constexpr value_set(value_set&&) noexcept                   = default;
    constexpr value_set& operator=(value_set const&) noexcept   = default;
    constexpr value_set& operator=(value_set&&) noexcept        = default;
    ~value_set() noexcept                                       = default;

    /// Constructs a value set from a `type_set` base type.
    constexpr value_set(base_type const& that) noexcept
        : base_type(that)
    {
    }

    /// Constructs a value set from a set of values.
    /// Providing an invalid value throws an `std::out_of_range` exception.
    /// Complexity is bilinear in the number of `Values...` of the `value_set`
    /// and the number of argument `Types...` provided in the constructor call,
    /// hence, if you use many `Values...`, use this constructor with care.
    /// If you know the values at compile time, use `make` factory method instead.
    template <typename... Types>
    constexpr value_set(Types... values)
        : base_type(detail::index_of_value<Type, Values...>(static_cast<Type>(values))...)
    {
        static_assert(
            detail::all(std::is_convertible<Types, Type>::value...),
            "All values in value_set constructor must be convertible to the value_set value type");
    }

    /// Returns an iterator to the first element in the value set.
    /// The iterator does not point to any underlying value, it only acts as an indicator of the
    /// existence of a certain element in the value set.
    /// See `value_set_iterator` class for details.
    constexpr iterator begin() const noexcept
    {
        return iterator(this, 0);
    }

    /// Returns a sentinel iterator representing the end of this value set.
    /// See `begin()` for details.
    constexpr iterator end() const noexcept
    {
        return iterator(this, sizeof...(Values));
    }

    template <Type Value>
    static constexpr size_t index() noexcept
    {
        return base_type::template index<value<Type, Value>>();
    }

    template <Type Value>
    static constexpr value_set make() noexcept
    {
        return value_set(base_type::template make<value<Type, Value>>());
    }

    template <Type Value>
    constexpr bool has() const noexcept
    {
        return base_type::template has<value<Type, Value>>();
    }

    template <Type Value>
    constexpr void add() & noexcept
    {
        static_assert(index<Value>() < sizeof...(Values), "Invalid value for value set");
        base_type::template add<value<Type, Value>>();
    }

    template <Type Value>
    constexpr void remove() & noexcept
    {
        static_assert(index<Value>() < sizeof...(Values), "Invalid value for value set");
        base_type::template remove<value<Type, Value>>();
    }

    constexpr value_set operator~ () const noexcept
    {
        return value_set(base_type::operator~());
    }

    friend constexpr value_set
    operator| (value_set const& lhs, value_set const& rhs) noexcept
    {
        return value_set(
                static_cast<base_type const&>(lhs)
              | static_cast<base_type const&>(rhs));
    }

    friend constexpr value_set
    operator& (value_set const& lhs, value_set const& rhs) noexcept
    {
        return value_set(
                static_cast<base_type const&>(lhs)
              & static_cast<base_type const&>(rhs));
    }

    friend constexpr value_set
    operator/ (value_set const& lhs, value_set const& rhs) noexcept
    {
        return value_set(
                static_cast<base_type const&>(lhs)
              / static_cast<base_type const&>(rhs));
    }

    friend constexpr value_set
    operator^ (value_set const& lhs, value_set const& rhs)
    {
        return value_set(
                static_cast<base_type const&>(lhs)
              ^ static_cast<base_type const&>(rhs));
    }

    constexpr value_set&
    operator|= (value_set const& that) & noexcept
    {
        return static_cast<value_set&>(base_type::operator|=(that));
    }

    constexpr value_set&
    operator&= (value_set const& that) & noexcept
    {
        return static_cast<value_set&>(base_type::operator&=(that));
    }

    constexpr value_set&
    operator/= (value_set const& that) & noexcept
    {
        return static_cast<value_set&>(base_type::operator/=(that));
    }

    constexpr value_set&
    operator^= (value_set const& that) & noexcept
    {
        return static_cast<value_set&>(base_type::operator^=(that));
    }

}; // class value_set

}  // namespace enum_set

#include <enum_set/value_set_iterator.hpp>
#include <enum_set/value_set_visitor.hpp>

#endif // ENUM_SET_VALUE_SET_HPP
