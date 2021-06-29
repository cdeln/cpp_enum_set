#ifndef ENUM_SET_VALUE_SET_VISITOR_HPP
#define ENUM_SET_VALUE_SET_VISITOR_HPP

#include <enum_set/value_set.hpp>

namespace enum_set
{
namespace detail
{

/// Represents a value together with a bool that indicates the presence of the value in a value set.
template <typename Type, Type Value>
struct value_indicator
{
    Type value = Value;
    bool is_present;
};

/// Static for loop that executes a visitor function object on a set of values.
template <typename Type, Type Value, Type... Values>
struct value_visitor_for
{
    template <class Visitor>
    constexpr static void each(
            Visitor&& visitor,
            value_indicator<Type, Value> current,
            value_indicator<Type, Values>... rest)
    {
        value_visitor_for<Type, Value    >::each(std::forward<Visitor>(visitor), current);
        value_visitor_for<Type, Values...>::each(std::forward<Visitor>(visitor), rest...);
    }
};

/// Base case of `value_visitor_for` when there is only one value left.
template <typename Type, Type Value>
struct value_visitor_for<Type, Value>
{
    template <class Visitor>
    constexpr static void each(Visitor&& visitor, value_indicator<Type, Value> indicator)
    {
        if (indicator.is_present)
        {
            visitor.template operator()<Value>();
        }
    }
};

}  // namespace detail

/// Visits all values in a value set using a `Visitor`.
/// The `Visitor` needs to implement `template <Value> ? operator()()` for all values in the set.
template <class Visitor, typename Type, Type... Values>
constexpr void visit(Visitor&& visitor, value_set<Type, Values...> const& values)
{
    detail::value_visitor_for<Type, Values...>::each(
        std::forward<Visitor>(visitor),
        detail::value_indicator<Type, Values>{Values, values.template has<Values>()}...);
}

}  // namespace enum_set

#endif // ENUM_SET_VALUE_SET_VISITOR_HPP
