#ifndef ENUM_SET_TYPE_SET_HPP
#define ENUM_SET_TYPE_SET_HPP

#include <enum_set/bit_mask.hpp>
#include <enum_set/common.hpp>
#include <enum_set/standard_types.hpp>

#include <type_traits>

namespace enum_set
{

namespace detail
{

/// Returns a `bit_mask` with bit at index of type `T` in the type list `Ts...` set.
template <typename T, typename... Ts>
constexpr bit_mask<sizeof...(Ts)>
make_bit_mask() noexcept
{
    return {std::is_same<T, Ts>::value...};
}

}  // namespace detail

/// Represents a set of types from a fixed non-empty universe of types `Ts...`.
/// The term element and type are used interchangeably in the documentation of this class.
template <typename... Ts>
class type_set
{
    static_assert(sizeof...(Ts) > 0, "type set types must be non-empty");
protected:
    using mask_type = bit_mask<sizeof...(Ts)>;

    /// Underlying storage for managing the representation of a type set.
    /// Each bit in the bit_mask tells whether a type is contained in the type set or not.
    mask_type mask;

    /// Constructs a type set from a bit mask.
    /// The constructor is private because the bit mask representation is not part of the interface.
    constexpr explicit type_set(mask_type const& mask) noexcept
        : mask{mask}
    {
    }
public:
    // The usual suspects.
    constexpr type_set(type_set const&) noexcept              = default;
    constexpr type_set(type_set&&) noexcept                   = default;
    constexpr type_set& operator=(type_set const&) noexcept   = default;
    constexpr type_set& operator=(type_set&&) noexcept        = default;
    ~type_set() noexcept                                      = default;

    /// Constructs an empty type set.
    constexpr type_set() noexcept
        : mask{}
    {
    }

    /// Constructs a type set by specifying which types to hold.
    /// The specification can be given either through a sequence of bools telling which types to
    /// include or not, or by providing the indices of the types to include.
    /// If using the variant with indices, providing an invalid index throws an `std::out_of_range`.
    /// Providing bools is safe, providing an invalid set of bools is a compile time error.
    template <
        typename... Args,
        typename = std::enable_if_t<
              detail::all(std::is_same<Args, bool>::value...) ||
            ! detail::any(std::is_same<Args, bool>::value...)
        >
    >
    constexpr type_set(Args... args)
        : mask{args...}
    {
    }

    /// Returns the zero based index of type `T` in the set of types `Ts...`.
    /// If `T` is not present in `Ts...` the capacity of the type set is returned.
    template <typename T>
    static constexpr size_t index() noexcept
    {
        return detail::index_of<T, Ts...>::value;
    }

    /// Creates a singleton type set containing a single element `T`.
    template <typename T>
    static constexpr type_set make() noexcept
    {
        static_assert(index<T>() < sizeof...(Ts), "Invalid type for type set");
        return type_set{detail::make_bit_mask<T, Ts...>()};
    }

    /// Checks if the type set contains an element `T`.
    /// Returns `true` if there is such an element, otherwise `false`.
    template <typename T>
    constexpr bool has() const noexcept
    {
        static_assert(index<T>() < sizeof...(Ts), "Invalid type for type set");
        return mask.template get<index<T>()>();
    }

    /// Returns the total number of possible elements the type set can hold.
    static constexpr size_t capacity() noexcept
    {
        return sizeof...(Ts);
    }

    /// Returns the number of elements currently being hold by the type set.
    constexpr size_t size() const noexcept
    {
        return detail::count(has<Ts>()...);
    }

    /// Works like `size()`, but returns a signed integer instead.
    /// Useful if you want to avoid manual static casts between signed and unsigned integers.
    constexpr ptrdiff_t count() const noexcept
    {
        return static_cast<ptrdiff_t>(size());
    }

    /// Checks if the type set is empty.
    /// Returns `true` if the type set holds no elements, otherwise `false`.
    constexpr bool empty() const noexcept
    {
        return !detail::any(has<Ts>()...);
    }

    /// Adds an element `T` to the type set.
    /// If the element already exists, no effects take place.
    template <typename T>
    constexpr void add() & noexcept
    {
        static_assert(index<T>() < sizeof...(Ts), "Invalid type for type set");
        mask.set(index<T>());
    }

    /// Removes the element `T` from the type set.
    /// If the element does not exists, no effects take place.
    template <typename T>
    constexpr void remove() & noexcept
    {
        static_assert(index<T>() < sizeof...(Ts), "Invalid type for type set");
        mask.clear(index<T>());
    }

    /// Erases all elements from the type set.
    constexpr void clear() noexcept
    {
        mask = {};
    }

    /// Returns the set complement of a type set.
    /// The set complement contains all elements not contained in the original type set.
    constexpr type_set
    operator~ () const noexcept
    {
        return type_set(mask_type{!has<Ts>()...});
    }

    /// Checks for equality between two type sets.
    /// Returns `true` if the first contains the same elements as the second, otherwise `false`.
    friend constexpr bool
    operator== (type_set const& first, type_set const& second) noexcept
    {
        return detail::all((first.has<Ts>() == second.has<Ts>())...);
    }

    /// Checks for inequality between two type sets.
    /// See `operator==` for details.
    friend constexpr bool
    operator!= (type_set const& first, type_set const& second) noexcept
    {
        return !(first == second);
    }

    /// Returns the set union of two type sets.
    /// The set union contains the elements contained in either two type sets.
    friend constexpr type_set
    operator| (type_set const& first, type_set const& second) noexcept
    {
        return type_set(mask_type{(first.has<Ts>() || second.has<Ts>())...});
    }

    /// Returns the set intersection of two type sets.
    /// The set intersection contains the elements contained in both type sets.
    friend constexpr type_set
    operator& (type_set const& first, type_set const& second) noexcept
    {
        return type_set(mask_type{(first.has<Ts>() && second.has<Ts>())...});
    }

    /// Returns the set difference between two type sets.
    /// The set difference contains all elements in the first set that are not in the second.
    friend constexpr type_set
    operator/ (type_set const& first, type_set const& second) noexcept
    {
        return first & ~second;
    }

    /// Returns the symmetric set difference between two type sets.
    /// The result contains all elements in contained in either two type sets but not in both.
    friend constexpr type_set
    operator^ (type_set const& first, type_set const& second) noexcept
    {
        return (first / second) | (second / first);
    }

    /// Checks if a type set is a subset of another.
    /// A type set is considered a subset of itself.
    /// Returns `true` of the first type set if a subset of the second, otherwise `false`.
    friend constexpr bool
    operator<= (type_set const& first, type_set const& second) noexcept
    {
        return detail::all(((first.has<Ts>() && second.has<Ts>()) || !first.has<Ts>())...);
    }

    /// Checks if a type set is a superset of another.
    /// A type set is considered a superset of itself.
    /// Returns `true` of the first type set if a superset of the second, otherwise `false`.
    friend constexpr bool
    operator>= (type_set const& first, type_set const& second) noexcept
    {
        return second <= first;
    }

    /// Checks if a type set is a strict subset of another.
    /// A type set is not considered a strict subset of itself.
    /// Returns `true` of the first type set if a strict subset of the second, otherwise `false`.
    friend constexpr bool
    operator< (type_set const& first, type_set const& second) noexcept
    {
        return (first <= second) && (first != second);
    }

    /// Checks if a type set is a strict superset of another.
    /// A type set is not considered a strict superset of itself.
    /// Returns `true` of the first type set if a strict superset of the second, otherwise `false`.
    friend constexpr bool
    operator> (type_set const& first, type_set const& second) noexcept
    {
        return second < first;
    }

    /// Adds all elements of another type set to this.
    /// Works like an in place set union, see `operator|` for details.
    /// Returns a reference to this type set.
    constexpr type_set&
    operator|= (type_set const& another) & noexcept
    {
        return *this = *this | another;
    }

    /// Restricts all elements in this type set to those contained in another type set.
    /// Works like an in place set intersection. See `operator&` for details.
    /// Returns a reference to this type set.
    constexpr type_set&
    operator&= (type_set const& another) & noexcept
    {
        return *this = *this & another;
    }

    /// Removes all elements in this type set that are contained in another type set.
    /// Works like an in place set difference. See `operator/` for details.
    /// Returns a reference to this type set.
    constexpr type_set&
    operator/= (type_set const& another) & noexcept
    {
        return *this = *this / another;
    }

    /// Removes all elements in this type set that are contained in another type set,
    /// and adds all elements from another type set that are not contained in this type set.
    /// Works like an in place symmetric set difference. See `operator^` for details.
    /// Returns a reference to this type set.
    constexpr type_set&
    operator^= (type_set const& other) & noexcept
    {
        return *this = *this ^ other;
    }

}; // class type_set

}  // namespace enum_set

#include <enum_set/type_set_visitor.hpp>

#endif // ENUM_SET_TYPE_SET_HPP
