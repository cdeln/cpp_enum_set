#ifndef ENUM_SET_VALUE_SET_ITERATOR_HPP
#define ENUM_SET_VALUE_SET_ITERATOR_HPP

#include <enum_set/bit_mask.hpp>
#include <enum_set/common.hpp>
#include <enum_set/value_set.hpp>

namespace enum_set
{
namespace detail
{

/// Finds the first set bit at index greater or equal to an offset into a bit_mask.
/// Returns the index of the bit if found, otherwise returns `mask.size()`.
template <size_t N>
constexpr size_t find(bit_mask<N> const& mask, size_t offset) noexcept
{
    while (offset < mask.size())
    {
        if (mask.get(offset))
        {
            return offset;
        }
        offset += 1;
    }
    return std::min(offset, mask.size());
}

}  // namespace detail

/// Represents an read only forward iterator to the elements of a value set.
template <typename Type, Type... Values>
class value_set<Type, Values...>::iterator
{
public:
    /// Pointer to the value set being iterated over.
    /// Changing the pointee invalidates this iterator.
    value_set const* container;

    /// Index of the current member into the `container` pointee.
    size_t index;
public:
    // Adapt to STL iterator interface.
    using iterator_category = std::forward_iterator_tag;
    using difference_type   = ptrdiff_t;
    using value_type        = Type;
    using pointer           = Type*;
    using reference         = Type&;

    // The usual suspects.
    constexpr iterator(iterator const&) noexcept            = default;
    constexpr iterator(iterator&&) noexcept                 = default;
    constexpr iterator& operator=(iterator const&) noexcept = default;
    constexpr iterator& operator=(iterator&&) noexcept      = default;
    ~iterator() noexcept                                    = default;

    /// Constructs a value set iterator from a pointer to the value set to iterate over
    /// and an offset into the underlying bit mask representation of the value set.
    /// The iterator will initially point to the first element with index greater or equal to the
    /// provided offset. If the offset is greater than any index of the elements contained in the
    /// set, including an offset greater or equal to the capacity of the container, then the
    /// iterator will become the end of sequence sentinel iterator.
    constexpr iterator(value_set const* container, size_t offset) noexcept
        : container{container}
        , index{detail::find(container->mask, offset)}
    {
    }

    /// Returns the element pointed to by this iterator.
    /// Although the syntax resembles dereferencing, the element is returned by value.
    /// Dereferencing the end iterator throws an `std::out_of_range` exception.
    constexpr Type operator*() const
    {
        return detail::get_value<Type, Values...>(index);
    }

    /// Increments this iterator.
    /// If there are no more elements left in the iteratee, the iterator becomes equal to end.
    /// Incrementing end has no effects.
    /// Returns a reference to this iterator.
    constexpr iterator& operator++() noexcept
    {
        if (index < sizeof...(Values))
        {
            index = detail::find(container->mask, index + 1);
        }
        return *this;
    }

    /// Checks for equality between two iterators.
    /// Returns `true` if both points to the same element in the same container, otherwise `false`.
    friend constexpr bool
    operator==(iterator const& first, iterator const& second) noexcept
    {
        return (first.container == second.container) && (first.index == second.index);
    }

    /// Checks if two iterators are different.
    /// See `operator==` for details.
    friend constexpr bool
    operator!=(iterator const& first, iterator const& second) noexcept
    {
        return !(first == second);
    }

}; // class value_set::iterator

}  // namespace enum_set

#endif // ENUM_SET_VALUE_SET_ITERATOR_HPP
