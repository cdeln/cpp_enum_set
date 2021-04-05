#include "testing.hpp"

#include <enum_set/bit_mask.hpp>

#include <type_traits>

using ::enum_set::detail::byte_storage;
using ::enum_set::detail::bit_storage;
using ::enum_set::detail::make_bit_storage;
using ::enum_set::detail::make_byte;

template <size_t N>
using bit_mask = ::enum_set::bit_mask<N>;

TEST_CASE("bit storage requires the expected number of bytes")
{
    STATIC_CHECK(
        (std::is_same<bit_storage<1>, byte_storage<1>>::value),
        "1 bit requires 1 byte storage");
    STATIC_CHECK(
        (std::is_same<bit_storage<8>, byte_storage<1>>::value),
        "8 bits requires 1 byte storage");
    STATIC_CHECK(
        (std::is_same<bit_storage<9>, byte_storage<2>>::value),
        "9 bits requires 2 bytes storage");
    STATIC_CHECK(
        (std::is_same<bit_storage<16>, byte_storage<2>>::value),
        "16 bits requires 2 bytes storage");
    STATIC_CHECK(
        (std::is_same<bit_storage<17>, byte_storage<3>>::value),
        "17 bits requires 3 bytes storage");
    STATIC_CHECK(
        (std::is_same<bit_storage<24>, byte_storage<3>>::value),
        "24 bits requires 3 bytes storage");
    STATIC_CHECK(
        (std::is_same<bit_storage<25>, byte_storage<4>>::value),
        "25 bits requires 4 bytes storage");
    STATIC_CHECK(
        (std::is_same<bit_storage<32>, byte_storage<4>>::value),
        "32 bits requires 4 bytes storage");
    STATIC_CHECK(
        (std::is_same<bit_storage<33>, byte_storage<5>>::value),
        "33 bits requires 5 bytes storage");
}

TEST_CASE("make byte from bools returns the expected unsigned 8-bit integer")
{
    STATIC_CHECK(
        make_byte(false) == 0,
        "Making a byte from false yields zero");
    STATIC_CHECK(
        make_byte(false, false) == 0,
        "Making a byte from false yields zero");
    STATIC_CHECK(
        make_byte(false, false, false) == 0,
        "Making a byte from false yields zero");
    STATIC_CHECK(
        make_byte(false, false, false, false) == 0,
        "Making a byte from false yields zero");
    STATIC_CHECK(
        make_byte(false, false, false, false, false) == 0,
        "Making a byte from false yields zero");
    STATIC_CHECK(
        make_byte(false, false, false, false, false, false) == 0,
        "Making a byte from false yields zero");
    STATIC_CHECK(
        make_byte(false, false, false, false, false, false, false) == 0,
        "Making a byte from false yields zero");
    STATIC_CHECK(
        make_byte(false, false, false, false, false, false, false, false) == 0,
        "Making a byte from false yields zero");
    STATIC_CHECK(
        make_byte(true) == 2-1,
        "Making a byte from true yields power of two minus one");
    STATIC_CHECK(
        make_byte(true, true) == 4-1,
        "Making a byte from all true yields power of two minus one");
    STATIC_CHECK(
        make_byte(true, true, true) == 8-1,
        "Making a byte from all true yields power of two minus one");
    STATIC_CHECK(
        make_byte(true, true, true, true) == 16-1,
        "Making a byte from all true yields power of two minus one");
    STATIC_CHECK(
        make_byte(true, true, true, true, true) == 32-1,
        "Making a byte from all true yields power of two minus one");
    STATIC_CHECK(
        make_byte(true, true, true, true, true, true) == 64-1,
        "Making a byte from all true yields power of two minus one");
    STATIC_CHECK(
        make_byte(true, true, true, true, true, true, true) == 128-1,
        "Making a byte from all true yields power of two minus one");
    STATIC_CHECK(
        make_byte(true, true, true, true, true, true, true, true) == 256-1,
        "Making a byte from all true yields power of two minus one");
    STATIC_CHECK(
        make_byte(true, false, true, true, false, false, false, true) == 1 + 4 + 8 + 128,
        "Making a byte from all mixed true and false yields expected value");
}

TEST_CASE("bit mask variadic bool constructor work as expected")
{
    constexpr bit_mask<9> mask = {false, true, false, false, true, true, false, true, false};
    STATIC_CHECK(!mask.get<0>(), "Bit mask with bits 1, 4, 5, and 7 set is not high at 0");
    STATIC_CHECK( mask.get<1>(), "Bit mask with bits 1, 4, 5, and 7 set is high at 1");
    STATIC_CHECK(!mask.get<2>(), "Bit mask with bits 1, 4, 5, and 7 set is not high at 2");
    STATIC_CHECK(!mask.get<3>(), "Bit mask with bits 1, 4, 5, and 7 set is not high at 3");
    STATIC_CHECK( mask.get<4>(), "Bit mask with bits 1, 4, 5, and 7 set is high at 4");
    STATIC_CHECK( mask.get<5>(), "Bit mask with bits 1, 4, 5, and 7 set is high at 5");
    STATIC_CHECK(!mask.get<6>(), "Bit mask with bits 1, 4, 5, and 7 set is not high at 6");
    STATIC_CHECK( mask.get<7>(), "Bit mask with bits 1, 4, 5, and 7 set is high at 7");
    STATIC_CHECK(!mask.get<8>(), "Bit mask with bits 1, 4, 5, and 7 set is not high at 8");
}

TEST_CASE("bit mask variadic index constructor with valid indices works as expected")
{
    constexpr bit_mask<9> mask = {3, 5, 7};
    STATIC_CHECK(!mask.get<0>(), "Bit mask defined at indices 3, 5 and 7 is not high at index 0");
    STATIC_CHECK(!mask.get<1>(), "Bit mask defined at indices 3, 5 and 7 is not high at index 1");
    STATIC_CHECK(!mask.get<2>(), "Bit mask defined at indices 3, 5 and 7 is not high at index 2");
    STATIC_CHECK( mask.get<3>(), "Bit mask defined at indices 3, 5 and 7 is high at index 3");
    STATIC_CHECK(!mask.get<4>(), "Bit mask defined at indices 3, 5 and 7 is not high at index 4");
    STATIC_CHECK( mask.get<5>(), "Bit mask defined at indices 3, 5 and 7 is high at index 5");
    STATIC_CHECK(!mask.get<6>(), "Bit mask defined at indices 3, 5 and 7 is not high at index 6");
    STATIC_CHECK( mask.get<7>(), "Bit mask defined at indices 3, 5 and 7 is high at index 7");
    STATIC_CHECK(!mask.get<8>(), "Bit mask defined at indices 3, 5 and 7 is not high at index 8");
}

TEST_CASE("bit mask variadic index constructor with an invalid index throws exception")
{
    CHECK_THROWS(bit_mask<9>{9});
}

TEST_CASE("bit mask size method returns the number of bits")
{
    STATIC_CHECK(bit_mask<1>{}.size() == 1, "Size of mask with size 1 returns 1");
    STATIC_CHECK(bit_mask<2>{}.size() == 2, "Size of mask with size 2 returns 2");
    STATIC_CHECK(bit_mask<3>{}.size() == 3, "Size of mask with size 3 returns 3");
    STATIC_CHECK(bit_mask<4>{}.size() == 4, "Size of mask with size 4 returns 4");
    STATIC_CHECK(bit_mask<5>{}.size() == 5, "Size of mask with size 5 returns 5");
    STATIC_CHECK(bit_mask<6>{}.size() == 6, "Size of mask with size 6 returns 6");
    STATIC_CHECK(bit_mask<7>{}.size() == 7, "Size of mask with size 7 returns 7");
    STATIC_CHECK(bit_mask<8>{}.size() == 8, "Size of mask with size 8 returns 8");
    STATIC_CHECK(bit_mask<9>{}.size() == 9, "Size of mask with size 9 returns 9");
}

TEST_CASE("bit mask equality operator")
{
    constexpr bit_mask<9> A(true, false, true, true, false, false, false, true, true);
    constexpr bit_mask<9> B(true, false, true, true, false, false, false, true, true);
    constexpr bit_mask<9> C(true, false, true, true, false, false, false, true, false);
    constexpr bit_mask<9> D(true, false, true, true, false, false, false, false, true);
    STATIC_CHECK(A == A, "bit_mask equal itself");
    STATIC_CHECK(A == B, "bit_mask equals another bit_mask with same bit values");
    STATIC_CHECK(A != C, "bit_mask does not equal a different bit_mask");
    STATIC_CHECK(A != D, "bit_mask does not equal a different bit_mask");
    STATIC_CHECK(C != D, "bit_mask does not equal a different bit_mask");
}

TEST_CASE("constexpr constructed bit mask is the same as dynamically constructed bit_mask")
{
    constexpr bit_mask<9> constexpr_mask;
    const bit_mask<9> dynamic_mask;
    CHECK(constexpr_mask == dynamic_mask);
}

TEST_CASE("bit mask default constructor creates a bit mask with all zeros")
{
    constexpr bit_mask<9> mask;
    STATIC_CHECK(!mask.get<0>(), "Default constructed bit_mask is all zeros");
    STATIC_CHECK(!mask.get<1>(), "Default constructed bit_mask is all zeros");
    STATIC_CHECK(!mask.get<2>(), "Default constructed bit_mask is all zeros");
    STATIC_CHECK(!mask.get<3>(), "Default constructed bit_mask is all zeros");
    STATIC_CHECK(!mask.get<4>(), "Default constructed bit_mask is all zeros");
    STATIC_CHECK(!mask.get<5>(), "Default constructed bit_mask is all zeros");
    STATIC_CHECK(!mask.get<6>(), "Default constructed bit_mask is all zeros");
    STATIC_CHECK(!mask.get<7>(), "Default constructed bit_mask is all zeros");
    STATIC_CHECK(!mask.get<8>(), "Default constructed bit_mask is all zeros");
}

TEST_CASE("bit mask get method gets the correct bits")
{
    constexpr bit_mask<9> mask{true, false, true, true, false, false, false, true, true};
    STATIC_CHECK( mask.get<0>(), "Mask defined at 0, 2, 3, 7 and 8 is set at 0");
    STATIC_CHECK(!mask.get<1>(), "Mask defined at 0, 2, 3, 7 and 8 is not set at 1");
    STATIC_CHECK( mask.get<2>(), "Mask defined at 0, 2, 3, 7 and 8 is set at 2");
    STATIC_CHECK( mask.get<3>(), "Mask defined at 0, 2, 3, 7 and 8 is set at 3");
    STATIC_CHECK(!mask.get<4>(), "Mask defined at 0, 2, 3, 7 and 8 is not set at 4");
    STATIC_CHECK(!mask.get<5>(), "Mask defined at 0, 2, 3, 7 and 8 is not set at 5");
    STATIC_CHECK(!mask.get<6>(), "Mask defined at 0, 2, 3, 7 and 8 is not set at 6");
    STATIC_CHECK( mask.get<7>(), "Mask defined at 0, 2, 3, 7 and 8 is set at 7");
    STATIC_CHECK( mask.get<8>(), "Mask defined at 0, 2, 3, 7 and 8 is set at 8");
}

TEST_CASE("bit mask get method throws exception for invalid index")
{
    bit_mask<9> mask;
    CHECK_THROWS(mask.get(9));
}

TEST_CASE("bit mask set method sets the correct bits")
{
    bit_mask<9> mask{true, false, true, true, false, false, false, true, true};
    mask.set<1>();
    CHECK( mask.get<0>());
    CHECK( mask.get<1>());
    CHECK( mask.get<2>());
    CHECK( mask.get<3>());
    CHECK(!mask.get<4>());
    CHECK(!mask.get<5>());
    CHECK(!mask.get<6>());
    CHECK( mask.get<7>());
    CHECK( mask.get<8>());
    mask.set<4>();
    CHECK( mask.get<0>());
    CHECK( mask.get<1>());
    CHECK( mask.get<2>());
    CHECK( mask.get<3>());
    CHECK( mask.get<4>());
    CHECK(!mask.get<5>());
    CHECK(!mask.get<6>());
    CHECK( mask.get<7>());
    CHECK( mask.get<8>());
    mask.set<6>();
    CHECK( mask.get<0>());
    CHECK( mask.get<1>());
    CHECK( mask.get<2>());
    CHECK( mask.get<3>());
    CHECK( mask.get<4>());
    CHECK(!mask.get<5>());
    CHECK( mask.get<6>());
    CHECK( mask.get<7>());
    CHECK( mask.get<8>());
    mask.set(5);
    CHECK( mask.get<0>());
    CHECK( mask.get<1>());
    CHECK( mask.get<2>());
    CHECK( mask.get<3>());
    CHECK( mask.get<4>());
    CHECK( mask.get<5>());
    CHECK( mask.get<6>());
    CHECK( mask.get<7>());
    CHECK( mask.get<8>());
}

TEST_CASE("bit mask set at invalid index throws exception")
{
    bit_mask<9> mask;
    CHECK_THROWS(mask.set(9));
}

TEST_CASE("bit mask clear method clears the correct bits")
{
    bit_mask<9> mask{true, false, true, true, false, false, false, true, true};
    mask.clear<0>();
    CHECK(!mask.get<0>());
    CHECK(!mask.get<1>());
    CHECK( mask.get<2>());
    CHECK( mask.get<3>());
    CHECK(!mask.get<4>());
    CHECK(!mask.get<5>());
    CHECK(!mask.get<6>());
    CHECK( mask.get<7>());
    CHECK( mask.get<8>());
    mask.clear<2>();
    CHECK(!mask.get<0>());
    CHECK(!mask.get<1>());
    CHECK(!mask.get<2>());
    CHECK( mask.get<3>());
    CHECK(!mask.get<4>());
    CHECK(!mask.get<5>());
    CHECK(!mask.get<6>());
    CHECK( mask.get<7>());
    CHECK( mask.get<8>());
    mask.clear<3>();
    CHECK(!mask.get<0>());
    CHECK(!mask.get<1>());
    CHECK(!mask.get<2>());
    CHECK(!mask.get<3>());
    CHECK(!mask.get<4>());
    CHECK(!mask.get<5>());
    CHECK(!mask.get<6>());
    CHECK( mask.get<7>());
    CHECK( mask.get<8>());
    mask.clear<7>();
    CHECK(!mask.get<0>());
    CHECK(!mask.get<1>());
    CHECK(!mask.get<2>());
    CHECK(!mask.get<3>());
    CHECK(!mask.get<4>());
    CHECK(!mask.get<5>());
    CHECK(!mask.get<6>());
    CHECK(!mask.get<7>());
    CHECK( mask.get<8>());
    mask.clear<8>();
    CHECK(!mask.get<0>());
    CHECK(!mask.get<1>());
    CHECK(!mask.get<2>());
    CHECK(!mask.get<3>());
    CHECK(!mask.get<4>());
    CHECK(!mask.get<5>());
    CHECK(!mask.get<6>());
    CHECK(!mask.get<7>());
    CHECK(!mask.get<8>());
}

TEST_CASE("bit mask clear method throws exception for invalid index")
{
    bit_mask<9> mask;
    CHECK_THROWS(mask.clear(9));
}
