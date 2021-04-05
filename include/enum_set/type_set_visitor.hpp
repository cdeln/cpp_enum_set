#ifndef ENUM_SET_TYPE_SET_VISITOR_HPP
#define ENUM_SET_TYPE_SET_VISITOR_HPP

#include <enum_set/type_set.hpp>

namespace enum_set
{
namespace detail
{

/// Represents a type together with a boolean that indicates the presence of the type in a type set.
template <typename T>
struct type_indicator
{
    using type = T;
    bool is_present;
};

/// Static for loop that executes a visitor function object on a set of types.
template <typename T, typename... Ts>
struct type_visitor_for
{
    template <class Visitor>
    constexpr static void each(
            Visitor&& visitor,
            type_indicator<T> current,
            type_indicator<Ts>... rest)
    {
        type_visitor_for<T    >::each(std::forward<Visitor>(visitor), current);
        type_visitor_for<Ts...>::each(std::forward<Visitor>(visitor), rest...);
    }
};

/// Base case for `type_visitor_for` when there is only one type left.
template <typename T>
struct type_visitor_for<T>
{
    template <class Visitor>
    constexpr static void each(Visitor&& visitor, type_indicator<T> indicator)
    {
        if (indicator.is_present)
        {
            visitor.template operator()<T>();
        }
    }
};

}  // namespace detail

/// Visits all types in a type set using a `Visitor` function object.
/// The `Visitor` needs to implement `template <typename T> ? operator()()` for all `T` in `Ts...`.
template <class Visitor, typename... Ts>
constexpr void visit(Visitor&& visitor, type_set<Ts...> const& types)
{
    detail::type_visitor_for<Ts...>::each(
        std::forward<Visitor>(visitor),
        detail::type_indicator<Ts>{types.template has<Ts>()}...);
}

}  // namespace enum_set

#endif // ENUM_SET_TYPE_SET_VISITOR_HPP
