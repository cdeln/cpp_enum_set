#include "testing.hpp"

#include <enum_set/index_set.hpp>

#include <algorithm>
#include <array>
#include <iterator>
#include <vector>

using testset =
    ::enum_set::value_set<
        size_t, 0, 2, 4, 6, 8, 1, 3, 5, 7, 9
    >;

namespace
{

constexpr auto universe
  = testset::make<0>()
  | testset::make<1>()
  | testset::make<2>()
  | testset::make<3>()
  | testset::make<4>()
  | testset::make<5>()
  | testset::make<6>()
  | testset::make<7>()
  | testset::make<8>()
  | testset::make<9>()
  ;

}  // namespace

namespace enum_set
{

TEST_CASE("find returns the first set bit greater or equal to the offset")
{
    constexpr ::enum_set::bit_mask<5> x = {true, false, true, false, true};
    STATIC_CHECK(detail::find(x, 0) == 0, "Finds bit at zero offset");
    STATIC_CHECK(detail::find(x, 1) == 2, "Finds first bit after offset");
    STATIC_CHECK(detail::find(x, 2) == 2, "Finds bit at non-zero offset");
    STATIC_CHECK(detail::find(x, 3) == 4, "Finds last bit if there is no other bit before");
    STATIC_CHECK(detail::find(x, 4) == 4, "Finds last bit if offset is at last bit");
    STATIC_CHECK(detail::find(x, 5) == 5, "Returns size of bit_mask for offset beyond last bit");
    STATIC_CHECK(detail::find(x, 6) == 5, "Returns size of bit_mask for offset beyond last bit");
}

TEST_CASE("iterator initially points to first element with id greater or equal to offset")
{
    testset x = testset::make<1>();
    testset::iterator it0(&x, 0);
    testset::iterator it2(&x, 1);
    testset::iterator it4(&x, 2);
    testset::iterator it6(&x, 3);
    testset::iterator it8(&x, 4);
    testset::iterator it1(&x, 5);
    testset::iterator it3(&x, 6);
    testset::iterator it5(&x, 7);
    testset::iterator it7(&x, 8);
    testset::iterator it9(&x, 9);
    CHECK(*it0 == 1);
    CHECK(*it2 == 1);
    CHECK(*it4 == 1);
    CHECK(*it6 == 1);
    CHECK(*it8 == 1);
    CHECK(*it1 == 1);
    CHECK(it3 == x.end());
    CHECK(it5 == x.end());
    CHECK(it7 == x.end());
    CHECK(it9 == x.end());
}

TEST_CASE("iterator constructed with offset greater than any element equals end")
{
    testset x = testset::make<1>();
    CHECK(x.capacity() == 10);
    testset::iterator it1(&x, 5);
    testset::iterator it3(&x, 6);
    testset::iterator it9(&x,  x.capacity() - 1);
    testset::iterator end1(&x, x.capacity());
    testset::iterator end2(&x, x.capacity() + 1);
    CHECK(it1 != x.end());
    CHECK(it3 == x.end());
    CHECK(it9 == x.end());
    CHECK(end1 == x.end());
    CHECK(end2 == x.end());
}

TEST_CASE("iterators are equal if they point to the same element in the same type set")
{
    constexpr std::size_t pivot = 5;
    testset x = testset::make<pivot>();
    testset::iterator i(&x, pivot);
    testset::iterator j(&x, pivot);
    CHECK(i == j);
}

TEST_CASE("iterators are not equal if they point to different type sets")
{
    constexpr std::size_t pivot = 5;
    testset x = testset::make<pivot>();
    testset y = testset::make<pivot>();
    testset::iterator i(&x, pivot);
    testset::iterator j(&y, pivot);
    CHECK(i != j);
}

TEST_CASE("iterators are not equal if they point to different elements")
{
    constexpr std::size_t pivot = 5;
    testset x = universe;
    testset::iterator i0(&x, pivot - 1);
    testset::iterator i1(&x, pivot);
    testset::iterator i2(&x, pivot + 1);
    CHECK(i0 != i1);
    CHECK(i0 != i2);
    CHECK(i1 != i2);
}

TEST_CASE("iterator increment operator increments as expected")
{
    constexpr auto X
        = testset::make<0>()
        | testset::make<5>()
        | testset::make<9>()
        ;
    auto it = X.begin();
    CHECK(*it == 0);
    CHECK(*(++it) == 5);
    CHECK(*(++it) == 9);
    CHECK(++it == X.end());
    CHECK(++it == X.end());
    CHECK_THROWS(*it);
}

TEST_CASE("iterator works with range based for loops")
{
    constexpr auto X
        = testset::make<0>()
        | testset::make<5>()
        | testset::make<9>()
        ;
    std::array<int, 10> count{};
    for (auto x : X)
    {
        count.at(x) += 1;
    }
    CHECK(count.at(0) == 1);
    CHECK(count.at(1) == 0);
    CHECK(count.at(2) == 0);
    CHECK(count.at(3) == 0);
    CHECK(count.at(4) == 0);
    CHECK(count.at(5) == 1);
    CHECK(count.at(6) == 0);
    CHECK(count.at(7) == 0);
    CHECK(count.at(8) == 0);
    CHECK(count.at(9) == 1);
}

TEST_CASE("copy from enum set to vector using std copy works as expected")
{
    std::vector<int> result;
    std::copy(universe.begin(), universe.end(), std::back_inserter(result));
    CHECK(result.size() == 10);
    CHECK(result.at(0) == 0);
    CHECK(result.at(1) == 2);
    CHECK(result.at(2) == 4);
    CHECK(result.at(3) == 6);
    CHECK(result.at(4) == 8);
    CHECK(result.at(5) == 1);
    CHECK(result.at(6) == 3);
    CHECK(result.at(7) == 5);
    CHECK(result.at(8) == 7);
    CHECK(result.at(9) == 9);
}

} // namespace enum_set
