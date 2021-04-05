#include "testing.hpp"

#include <enum_set/standard_types.hpp>
#include <enum_set/type_set.hpp>
#include <enum_set/value_set.hpp>

#include <type_traits>
#include <utility>
#include <vector>

// explicitly instantiate to make sure the class is well defined
template class ::enum_set::value_set<int, 0, 2, 1>;

using test_set = ::enum_set::value_set<int, 0, 2, 1>;

namespace
{

struct test_fixture
{
    static constexpr auto empty = test_set();
    static constexpr auto x0 = test_set::make<0>();
    static constexpr auto x1 = test_set::make<1>();
    static constexpr auto x2 = test_set::make<2>();
    static constexpr auto all = x0 | x1 | x2;
};

constexpr test_set test_fixture::empty;
constexpr test_set test_fixture::x0;
constexpr test_set test_fixture::x1;
constexpr test_set test_fixture::x2;
constexpr test_set test_fixture::all;

struct test_visitor
{
    std::vector<std::pair<int, size_t>>& result;

    test_visitor(std::vector<std::pair<int, size_t>>& result)
        : result{result} {}

    template <int V>
    void operator()()
    {
        result.push_back(std::make_pair(V, test_set::index<V>()));
    }
};

}  // namespace

namespace enum_set
{

TEST_CASE("base type of value set is as expected")
{
    STATIC_CHECK(
        (std::is_base_of<
            type_set<value<int, 0>>,
            value_set<int, 0>
         >::value),
        "value set of size 1 is a type set of 1 index");
    STATIC_CHECK(
        (std::is_base_of<
            type_set<
                value<int, 0>,
                value<int, 1>
            >,
            value_set<int, 0, 1>
         >::value),
        "value set of size 2 is a type set of 2 indices");
    STATIC_CHECK(
        (std::is_base_of<
            type_set<
                value<int, 0>,
                value<int, 1>,
                value<int, 2>
            >,
            value_set<int, 0, 1, 2>
        >::value),
        "value set of size 3 is a type set of 3 indices");
    STATIC_CHECK(
        (std::is_base_of<
            type_set<
                value<int, 0>,
                value<int, 1>,
                value<int, 2>,
                value<int, 3>
            >,
            value_set<int, 0, 1, 2, 3>
        >::value),
        "value set of size 4 is a type set of 4 indices");
}

TEST_CASE("base class converting constructor works")
{
    test_set::base_type base = test_set::base_type::make<value<int, 1>>();
    test_set derived(base);
    CHECK(!derived.has<0>());
    CHECK(derived.has<1>());
    CHECK(!derived.has<2>());
}

TEST_CASE("variadic constructor works")
{
    constexpr test_set X = {1, 2};
    STATIC_CHECK(!X.has<0>(), "Value set does not contain 0");
    STATIC_CHECK( X.has<1>(), "Value set does contain 1");
    STATIC_CHECK( X.has<2>(), "Value set does contain 2");
}

TEST_CASE("variadic constructor throws exception for invalid value")
{
    CHECK_THROWS(test_set{3});
}

TEST_CASE("size of value set_of specified size is as expected")
{
    STATIC_CHECK(sizeof(value_set<int, 0>) == 1,
                  "Typeset of size 1 requires 1 byte");
    STATIC_CHECK(sizeof(value_set<int, 0, 1, 2, 3, 4, 5, 6, 7>) == 1,
                  "Typeset of size 8 requires 1 byte");
    STATIC_CHECK(sizeof(value_set<int, 0, 1, 2, 3, 4, 5, 6, 7, 8>) == 2,
                  "Typeset of size 9 requires 2 bytes");
}

TEST_CASE_FIXTURE(test_fixture, "value set has the expected elements")
{
    STATIC_CHECK(!empty.has<0>(), "Empty value set does not contain anything");
    STATIC_CHECK(!empty.has<1>(), "Empty value set does not contain anything");
    STATIC_CHECK(!empty.has<2>(), "Empty value set does not contain anything");
    STATIC_CHECK(x0.has<0>(), "Singleton value set contains the expected element");
    STATIC_CHECK(!x0.has<1>(), "Singleton value set contains the expected element");
    STATIC_CHECK(!x0.has<2>(), "Singleton value set contains the expected element");
    STATIC_CHECK(!x1.has<0>(), "Singleton value set contains the expected element");
    STATIC_CHECK(x1.has<1>(), "Singleton value set contains the expected element");
    STATIC_CHECK(!x1.has<2>(), "Singleton value set contains the expected element");
    STATIC_CHECK(!x2.has<0>(), "Singleton value set contains the expected element");
    STATIC_CHECK(!x2.has<1>(), "Singleton value set contains the expected element");
    STATIC_CHECK(x2.has<2>(), "Singleton value set contains the expected element");
}

TEST_CASE("value set index methods returns the expected indices")
{
    STATIC_CHECK(test_set::index<0>() == 0, "Index of 0 is 0");
    STATIC_CHECK(test_set::index<1>() == 2, "Index of 1 is 2");
    STATIC_CHECK(test_set::index<2>() == 1, "Index of 1 is 2");
}

TEST_CASE("value set add method works as expected")
{
    test_set values;
    CHECK(values.empty());
    values.add<0>();
    CHECK(values.has<0>());
    CHECK(!values.has<1>());
    CHECK(!values.has<2>());
    values.add<0>();
    CHECK(values.has<0>());
    CHECK(!values.has<1>());
    CHECK(!values.has<2>());
    values.add<2>();
    CHECK(values.has<0>());
    CHECK(!values.has<1>());
    CHECK(values.has<2>());
    values.add<1>();
    CHECK(values.has<0>());
    CHECK(values.has<1>());
    CHECK(values.has<2>());
}

TEST_CASE_FIXTURE(test_fixture, "value set remove method works as expected")
{
    test_set values = x0 | x1 | x2;
    CHECK(values.has<0>());
    CHECK(values.has<1>());
    CHECK(values.has<2>());
    values.remove<0>();
    CHECK(!values.has<0>());
    CHECK(values.has<1>());
    CHECK(values.has<2>());
    values.remove<0>();
    CHECK(!values.has<0>());
    CHECK(values.has<1>());
    CHECK(values.has<2>());
    values.remove<2>();
    CHECK(!values.has<0>());
    CHECK(values.has<1>());
    CHECK(!values.has<2>());
    values.remove<1>();
    CHECK(values.empty());
}

TEST_CASE_FIXTURE(test_fixture, "value set complement operator works")
{
    constexpr test_set x = {0, 2};
    STATIC_CHECK(~x == test_set{1}, "Complement of {0, 2} w.r.t to {0, 1, 2} is {1}");
}

TEST_CASE_FIXTURE(test_fixture, "value set union operator works")
{
    STATIC_CHECK( (x0 | x2).has<0>(), "{0, 2} has 0");
    STATIC_CHECK(!(x0 | x2).has<1>(), "{0, 2} do not have 1");
    STATIC_CHECK( (x0 | x2).has<2>(), "{0, 2} has 2");
}

TEST_CASE_FIXTURE(test_fixture, "value set intersection operator works")
{
    STATIC_CHECK((all & x0) == x0, "all intersect with x0 is x0");
    STATIC_CHECK((all & x1) == x1, "all intersect with x1 is x1");
    STATIC_CHECK((all & x2) == x2, "all intersect with x2 is x2");
    constexpr auto x01 = x0 | x1;
    constexpr auto x12 = x1 | x2;
    STATIC_CHECK((x01 & x12) == x1, "{0, 1} intersect with {1, 2} is {1}");
}

TEST_CASE_FIXTURE(test_fixture, "value set difference operator works")
{
    STATIC_CHECK((all / x0) == (x1 | x2), "all without x0 is x1 | x2");
    STATIC_CHECK((all / x1) == (x0 | x2), "all without x1 is x0 | x2");
    STATIC_CHECK((all / x2) == (x0 | x1), "all without x2 is x0 | x1");
}

TEST_CASE_FIXTURE(test_fixture, "value set symmetric difference operator works")
{
    constexpr auto x01 = x0 | x1;
    constexpr auto x12 = x1 | x2;
    STATIC_CHECK((x01  ^ x12) == (x0 | x2), "Symmetric diff between {0, 1} and {1, 2} is {0, 2}");
}

TEST_CASE_FIXTURE(test_fixture, "value set in place union works")
{
    auto x = x0;
    x |= x1;
    CHECK(x == (x0 | x1));
}

TEST_CASE_FIXTURE(test_fixture, "value set in place intersection works")
{
    auto x = x0 | x1;
    const auto y = x1 | x2;
    x &= y;
    CHECK(x == x1);
}

TEST_CASE_FIXTURE(test_fixture, "value set in place difference works")
{
    auto x = all;
    x /= x1;
    CHECK(x == ~x1);
}

TEST_CASE_FIXTURE(test_fixture, "value set in place symmetric difference works")
{
    auto x = x0 | x1;
    const auto y = x1 | x2;
    x ^= y;
    CHECK(x == ~x1);
}

TEST_CASE_FIXTURE(test_fixture, "value set iterator increments as expected")
{
    test_set x = test_set::make<0>() | test_set::make<2>();
    auto it = x.begin();
    CHECK(*it == 0);
    CHECK(*(++it) == 2);
    CHECK(++it == x.end());
    CHECK_THROWS(*it);
}

TEST_CASE_FIXTURE(test_fixture, "value set visit visits the expected types")
{
    std::vector<std::pair<int, size_t>> result;

    SUBCASE("empty set")
    {
        visit(test_visitor(result), empty);
        REQUIRE(result.empty());
    }

    SUBCASE("for x1")
    {
        visit(test_visitor(result), x1);
        REQUIRE(result.size() == 1);
        CHECK(result.at(0) == std::pair<int, size_t>{1, 2});
    }

    SUBCASE("for x0 | x1")
    {
        visit(test_visitor(result), x0 | x1);
        REQUIRE(result.size() == 2);
        CHECK(result.at(0) == std::pair<int, size_t>{0, 0});
        CHECK(result.at(1) == std::pair<int, size_t>{1, 2});
    }

    SUBCASE("for x1 | x2")
    {
        visit(test_visitor(result), x1 | x2);
        REQUIRE(result.size() == 2);
        CHECK(result.at(0) == std::pair<int, size_t>{2, 1});
        CHECK(result.at(1) == std::pair<int, size_t>{1, 2});
    }

    SUBCASE("for x0 | x1 | x2 ")
    {
        visit(test_visitor(result), x0 | x1 | x2);
        REQUIRE(result.size() == 3);
        CHECK(result.at(0) == std::pair<int, size_t>{0, 0});
        CHECK(result.at(1) == std::pair<int, size_t>{2, 1});
        CHECK(result.at(2) == std::pair<int, size_t>{1, 2});
    }
}

} // namespace enum_set
