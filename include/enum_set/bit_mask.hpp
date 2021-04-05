#ifndef ENUM_SET_BIT_MASK_HPP
#define ENUM_SET_BIT_MASK_HPP

#include <enum_set/common.hpp>
#include <enum_set/standard_types.hpp>

#include <type_traits>
#include <utility>

namespace enum_set
{
namespace detail
{

/// Small wrapper struct around a plain C-array.
/// Used instead of std::array because we need constexpr access operators (fixed in C++17).
template <typename T, size_t N>
struct array
{
    T values[N];

    constexpr T&
    operator[](size_t index)
    {
        return values[index];
    }

    constexpr T const&
    operator[](size_t index) const
    {
        return values[index];
    }
};

/// Declaration of the underlying storage of bytes as an array of `ByteCount` 8-bit integers.
template <size_t ByteCount>
using byte_storage = array<uint8_t, ByteCount>;

/// Implmentation helper for `operator==` below.
template <size_t ByteCount, size_t... Indices>
constexpr bool
equals_impl(byte_storage<ByteCount> const& lhs,
            byte_storage<ByteCount> const& rhs,
            std::index_sequence<Indices...>) noexcept
{
    static_assert(all((Indices < ByteCount)...), "Byte storage out of bounds access");
    return all((lhs[Indices] == rhs[Indices])...);
}

/// Constexpr equality operator for byte_storages (not necessary in C++20).
template <size_t ByteCount>
constexpr bool
operator==(byte_storage<ByteCount> const& lhs,
           byte_storage<ByteCount> const& rhs) noexcept
{
    return equals_impl(lhs, rhs, std::make_index_sequence<ByteCount>());
}

/// Type factory for building the storage type holding `BitCount` bits.
template <size_t BitCount>
struct bit_storage_factory
{
    static_assert(BitCount > 0, "Number of bits must be a strictly positive number");
    using type = byte_storage<1 + (BitCount - 1)/8>;
};

/// Declaration of the underlying storage of `BitCount` bits.
/// The actual number of bytes is calculcated from the number of bits.
template <size_t BitCount>
using bit_storage = typename bit_storage_factory<BitCount>::type;

/// Creates a byte with bits set according to the arguments `b0, b1, ..., b7`.
constexpr uint8_t
make_byte(bool b0, bool b1, bool b2, bool b3, bool b4, bool b5, bool b6, bool b7) noexcept
{
    return (b0 ? (1 << 0) : 0)
         | (b1 ? (1 << 1) : 0)
         | (b2 ? (1 << 2) : 0)
         | (b3 ? (1 << 3) : 0)
         | (b4 ? (1 << 4) : 0)
         | (b5 ? (1 << 5) : 0)
         | (b6 ? (1 << 6) : 0)
         | (b7 ? (1 << 7) : 0);
}

/// Overload of `make_byte` for 7 bits.
constexpr uint8_t
make_byte(bool b0, bool b1, bool b2, bool b3, bool b4, bool b5, bool b6) noexcept
{
    return make_byte(b0, b1, b2, b3, b4, b5, b6, false);
}

/// Overload of `make_byte` for 6 bits.
constexpr uint8_t
make_byte(bool b0, bool b1, bool b2, bool b3, bool b4, bool b5) noexcept
{
    return make_byte(b0, b1, b2, b3, b4, b5, false, false);
}

/// Overload of `make_byte` for 5 bits.
constexpr uint8_t
make_byte(bool b0, bool b1, bool b2, bool b3, bool b4) noexcept
{
    return make_byte(b0, b1, b2, b3, b4, false, false, false);
}

/// Overload of `make_byte` for 4 bits.
constexpr uint8_t
make_byte(bool b0, bool b1, bool b2, bool b3) noexcept
{
    return make_byte(b0, b1, b2, b3, false, false, false, false);
}

/// Overload of `make_byte` for 3 bits.
constexpr uint8_t
make_byte(bool b0, bool b1, bool b2) noexcept
{
    return make_byte(b0, b1, b2, false, false, false, false, false);
}

/// Overload of `make_byte` for 2 bits.
constexpr uint8_t
make_byte(bool b0, bool b1) noexcept
{
    return make_byte(b0, b1, false, false, false, false, false, false);
}

/// Overload of `make_byte` for 1 bit.
constexpr uint8_t
make_byte(bool b0) noexcept
{
    return make_byte(b0, false, false, false, false, false, false, false);
}

/// Factory for constructing a byte at a byte index and booleans specifying which bits to set.
/// Walks through consecutive chunks of 8 bits until the chunk at the byte index is found.
/// Once the chunk is found, the appropriate bits will be set and the byte is returned.
template <size_t ByteIndex>
struct byte_factory
{
    template <typename... Bools>
    static constexpr uint8_t
    make(bool, bool, bool, bool, bool, bool, bool, bool, Bools... bits) noexcept
    {
        return byte_factory<ByteIndex - 1>::make(bits...);
    }
};

/// Specialization of `byte_factory` for byte index 0 (that is, the chunk of bits has been found).
template <>
struct byte_factory<0>
{
    /// Base case when there are atleast 8 bits left.
    /// Construct a byte from the first 8 bits and discard the rest.
    template <typename... Bools>
    static constexpr uint8_t
    make(bool b0, bool b1, bool b2, bool b3, bool b4, bool b5, bool b6, bool b7, Bools...) noexcept
    {
        return make_byte(b0, b1, b2, b3, b4, b5, b6, b7);
    }

    /// Base case when there are less than 8 bits left.
    /// Construct a byte from the available bits and leave the rest unset.
    static constexpr uint8_t
    make(bool b0 = false,
         bool b1 = false,
         bool b2 = false,
         bool b3 = false,
         bool b4 = false,
         bool b5 = false,
         bool b6 = false) noexcept
    {
        return make_byte(b0, b1, b2, b3, b4, b5, b6, false);
    }
};

/// Creates a byte storage from a set of booleans specifying which bits to set.
/// The number of `Bools...` must not require more bytes than there are available `ByteIndices...`.
/// See `byte_factory::make` for details on how each byte is set.
template <size_t... ByteIndices, typename... Bools>
constexpr byte_storage<sizeof...(ByteIndices)>
make_bytes(std::index_sequence<ByteIndices...>, Bools... bits) noexcept
{
    return {byte_factory<ByteIndices>::make(bits...)...};
}

/// Creates a bit storage from a set of booleans specifying which bits to be set.
template <typename... Bools>
constexpr bit_storage<sizeof...(Bools)>
make_bit_storage(Bools... bits) noexcept
{
    constexpr size_t byte_count = 1 + (sizeof...(Bools) - 1)/8;
    constexpr auto indices = std::make_index_sequence<byte_count>();
    return make_bytes(indices, bits...);
}

}  // namespace detail

/// Representation of a contiguous array of `Size` bits.
template <size_t Size>
class bit_mask final
{
private:
    detail::bit_storage<Size> storage;
public:
    // The usual suspects.
    constexpr bit_mask(bit_mask const&) noexcept            = default;
    constexpr bit_mask(bit_mask&&) noexcept                 = default;
    constexpr bit_mask& operator=(bit_mask const&) noexcept = default;
    constexpr bit_mask& operator=(bit_mask&&) noexcept      = default;
    ~bit_mask() noexcept                                    = default;

    /// Constructs an empty bit_mask.
    constexpr bit_mask() noexcept
        : storage{}
    {
    }

    /// Constructs a bit mask from a set of boolean `bits...` specifying which bits to set.
    /// The number of bools must match the `Size` of the bit mask.
    template <
        typename... Bools,
        typename = std::enable_if_t<
            detail::all(std::is_same<Bools, bool>::value...)
        >
    >
    constexpr bit_mask(Bools... bits) noexcept
        : storage{detail::make_bit_storage(bits...)}
    {
        static_assert(
            sizeof...(Bools) == Size,
            "Number of bits in bit_mask constructor must match the size of the bit_mask");
        static_assert(
            detail::all(std::is_same<Bools, bool>::value...),
            "bit_mask constructor arguments must be bools");
    }

    /// Constructs a bit mask from a set of `indices...` specifying which bits to set.
    /// The `Indices...` must be implicitly convertible to `size_t`,
    /// and none should be a `bool` (that is reserved for `bit_mask(Bools...)` constructor).
    /// Specifying any invalid index throws an `std::out_of_range` exception.
    template <
        typename... Indices,
        std::enable_if_t<
            ! detail::any(std::is_same<Indices, bool>::value...),
            nullptr_t
        > = nullptr
    >
    constexpr bit_mask(Indices... indices)
        : storage{}
    {
        for (auto index : {indices...})
        {
            set(index);
        }
    }

    /// Returns the maximum number of bits the bit mask can represent.
    constexpr size_t size() const noexcept
    {
        return Size;
    }

    /// Tests of a bit at a specified index.
    /// The index must be less than the bit mask size, otherwise an exception is thrown.
    /// Returns `true` if the bit at index is set, otherwise `false`.
    constexpr bool get(size_t index) const
    {
        if (index >= Size)
        {
            throw std::out_of_range("Bit mask get index out of bounds");
        }
        const uint8_t bit = (1 << (index % 8));
        const uint8_t byte = storage[index / 8];
        return ((byte & bit) == 0) ? false : true;
    }

    /// Safe test of a bit at a specified compile time index.
    /// Trying to get a bit out of range triggers a compile time error.
    /// See `get(size_t)` for details.
    template <size_t Index>
    constexpr bool get() const noexcept
    {
        static_assert(Index < Size, "Bit mask get index out of bounds");
        return get(Index);
    }

    /// Sets the bit at a specified index.
    /// The index must be less than the bit mask size, otherwise an exception is thrown.
    constexpr void set(size_t index) &
    {
        if (index >= Size)
        {
            throw std::out_of_range("Bit mask set index out of bounds");
        }
        const uint8_t bit = index % 8;
        uint8_t& byte = storage[index / 8];
        byte |= (1 << bit);
    }

    /// Safe setting of a bit at a specified index.
    /// Trying to set a bit out of range triggers a compile time error.
    template <size_t Index>
    constexpr void set() & noexcept
    {
        static_assert(Index < Size, "Bit mask set index out of bounds");
        set(Index);
    }

    /// Clears the bit at a specified index.
    /// The index must be less than the bit mask size, otherwise an exception is thrown.
    constexpr void clear(size_t index) &
    {
        if (index >= Size)
        {
            throw std::out_of_range("Bit mask clear index out of bounds");
        }
        const uint8_t bit = index % 8;
        uint8_t& byte = storage[index / 8];
        byte &= ~(1 << bit);
    }

    /// Safe clearing of a bit at a specified index.
    /// Trying to clear a bit at out of range triggers a compile time error.
    template <size_t Index>
    constexpr void clear() & noexcept
    {
        static_assert(Index < Size, "Bitmask index our of range");
        clear(Index);
    }

    /// Compares to bit masks.
    /// Returns `true` of all bits are equal, otherwise `false`.
    friend constexpr bool operator==(bit_mask const& lhs, bit_mask const& rhs) noexcept
    {
        return detail::operator==(lhs.storage, rhs.storage);
    }

    /// Compares to bit masks for inequality.
    /// Returns `true` of any bit differs.
    friend constexpr bool operator!=(bit_mask const& lhs, bit_mask const& rhs) noexcept
    {
        return !(lhs == rhs);
    }

}; // class bit_mask

}  // namespace enum_set

#endif // ENUM_SET_BIT_MASK_HPP
