#include "testing.hpp"

#include <enum_set/type_set.hpp>

#include <vector>

using namespace ::enum_set;

struct code
{
    class A;
    class B;
    class C;
    class D;
};

// explicitly instantiate to make sure the class is well defined
template class
    ::enum_set::type_set<
        code::A,
        code::B,
        code::C,
        code::D
      >;

using test_set =
    ::enum_set::type_set<
        code::A,
        code::B,
        code::C,
        code::D
    >;

namespace
{

struct test_fixture
{
    static constexpr auto empty = test_set();
    static constexpr auto A = test_set::make<code::A>();
    static constexpr auto B = test_set::make<code::B>();
    static constexpr auto C = test_set::make<code::C>();
    static constexpr auto D = test_set::make<code::D>();
    static constexpr auto AB = A | B;
    static constexpr auto BC = B | C;
    static constexpr auto CD = C | D;
    static constexpr auto ABC = A | B | C;
    static constexpr auto BCD = B | C | D;
    static constexpr auto DCA = D | C | A;
    static constexpr auto universe = A | B | C | D;
};

constexpr test_set test_fixture::empty;
constexpr test_set test_fixture::A;
constexpr test_set test_fixture::B;
constexpr test_set test_fixture::C;
constexpr test_set test_fixture::D;
constexpr test_set test_fixture::AB;
constexpr test_set test_fixture::BC;
constexpr test_set test_fixture::CD;
constexpr test_set test_fixture::ABC;
constexpr test_set test_fixture::BCD;
constexpr test_set test_fixture::DCA;
constexpr test_set test_fixture::universe;

struct test_visitor
{
    std::vector<size_t>& result;

    test_visitor(std::vector<size_t>& result)
        : result{result} {}

    template <typename T>
    void operator()()
    {
        result.push_back(test_set::index<T>());
    }
};

}  // namespace

TEST_CASE("make_bit_mask returns the expected mask")
{
    static constexpr auto mask_for_int = detail::make_bit_mask<int, int, bool, int>();
    STATIC_CHECK(mask_for_int.size() == 3, "Size of returns mask is the size of the typelist");
    STATIC_CHECK(mask_for_int.get(0) == true, "Mask is true at all indices of the requested type");
    STATIC_CHECK(mask_for_int.get(1) == false, "Mask is false for all other indices");
    STATIC_CHECK(mask_for_int.get(2) == true, "Mask is true at all indices of the requested type");
    static constexpr auto mask_for_bool = detail::make_bit_mask<bool, int, bool, int>();
    STATIC_CHECK(mask_for_int.size() == 3, "Size of returns mask is the size of the typelist");
    STATIC_CHECK(mask_for_bool.get(0) == false, "Mask is false for all other indices");
    STATIC_CHECK(mask_for_bool.get(1) == true, "Mask is true at the index of the requested type");
    STATIC_CHECK(mask_for_bool.get(2) == false, "Mask is false for all other indices");
    static constexpr auto mask_for_invalid = detail::make_bit_mask<double, int, bool, int>();
    STATIC_CHECK(mask_for_int.size() == 3, "Size of returns mask is the size of the typelist");
    STATIC_CHECK(mask_for_invalid.get(0) == false, "Mask is zero when requested type is invalid");
    STATIC_CHECK(mask_for_invalid.get(1) == false, "Mask is zero when requested type is invalid");
    STATIC_CHECK(mask_for_invalid.get(2) == false, "Mask is zero when requested type is invalid");
}

TEST_CASE("default constructed type set is empty")
{
    STATIC_CHECK(test_set().empty(), "Default constructed type set is empty");
}

TEST_CASE("variadic index constructor yields the expected type set")
{
    STATIC_CHECK(!test_set(1,2).has<code::A>(), "Variadic constructed set does not contain A");
    STATIC_CHECK( test_set(1,2).has<code::B>(), "Variadic constructed set does contain B");
    STATIC_CHECK( test_set(1,2).has<code::C>(), "Variadic constructed set does contain C");
    STATIC_CHECK(!test_set(1,2).has<code::D>(), "Variadic constructed set does not contain D");
}

TEST_CASE("variadic index constructor throws exception on invalid index")
{
    CHECK_THROWS(test_set(test_set::capacity()));
}

TEST_CASE("variadic bool constructor yields the expected type set")
{
    STATIC_CHECK(
        ! test_set(false, true, false, true).has<code::A>(),
        "Variadic constructed set does not contain A");
    STATIC_CHECK(
        test_set(false, true, false, true).has<code::B>(),
        "Variadic constructed set does contain B");
    STATIC_CHECK(
        ! test_set(false, true, false, true).has<code::C>(),
        "Variadic constructed set does not contain C");
    STATIC_CHECK(
        test_set(false, true, false, true).has<code::D>(),
        "Variadic constructed set does contain D");
}

TEST_CASE_FIXTURE(test_fixture, "empty method returns true for non empty type sets")
{
    STATIC_CHECK(empty.empty(), "Default constructed type set is initially empty");
    STATIC_CHECK(!A.empty(), "Type set created with factory method is be non-empty");
    STATIC_CHECK(!B.empty(), "Type set created with factory method is be non-empty");
    STATIC_CHECK(!C.empty(), "Type set created with factory method is be non-empty");
    STATIC_CHECK(!D.empty(), "Type set created with factory method is be non-empty");
}

TEST_CASE_FIXTURE(test_fixture, "factory method creates the correct type set")
{
    STATIC_CHECK(
        A.has<code::A>(),
        "Type set created with factory method should contain the specified tag");
    STATIC_CHECK(
        !A.has<code::B>(),
        "Type set created from factory method should not contain an unspecified tag");
    STATIC_CHECK(
        !A.has<code::C>(),
        "Type set created from factory method should not contain an unspecified tag");
    STATIC_CHECK(
        !A.has<code::D>(),
        "Type set created from factory method should not contain an unspecified tag");
}

TEST_CASE_FIXTURE(test_fixture, "equality operator")
{
    STATIC_CHECK(empty == empty, "Default constructed type set should equal itself");
    STATIC_CHECK(A == A, "Type set created with factory method should equal itself");
    STATIC_CHECK(A != B, "Type set does not equal a different type set");
    STATIC_CHECK(B != A, "Equality is symmetric");
}


TEST_CASE_FIXTURE(test_fixture, "union operator")
{
    STATIC_CHECK((A | A) == A, "Union is idempotent");
    STATIC_CHECK((A | B).has<code::A>(), "Union contains the left operand");
    STATIC_CHECK((A | B).has<code::B>(), "Union contains the right operand");
    STATIC_CHECK((B | A) == (A | B), "Union is commutative");
    STATIC_CHECK(((A | B) | C) == (A | (B | C)), "Union is associative");
}

TEST_CASE_FIXTURE(test_fixture, "capacity method returns total number of types in a type set")
{
    STATIC_CHECK(empty.capacity() == 4, "Capacity equals number of template arguments");
    STATIC_CHECK(A.capacity() == 4, "Capacity equals number of template arguments");
    STATIC_CHECK(AB.capacity() == 4, "Capacity equals number of template arguments");
    STATIC_CHECK(ABC.capacity() == 4, "Capacity equals number of template arguments");
    STATIC_CHECK(universe.capacity() == 4, "Capacity equals number of template arguments");
}

TEST_CASE_FIXTURE(test_fixture, "size method returns the number of types in a type set")
{
    STATIC_CHECK(empty.size() == 0, "Type set size equals number of types in the set");
    STATIC_CHECK(A.size() == 1, "Type set size equals number of types in the set");
    STATIC_CHECK(AB.size() == 2, "Type set size equals number of types in the set");
    STATIC_CHECK(ABC.size() == 3, "Type set size equals number of types in the set");
    STATIC_CHECK(universe.size() == 4, "Type set size equals number of types in the set");
}

TEST_CASE_FIXTURE(test_fixture, "count method returns the number of types in a type set")
{
    STATIC_CHECK(empty.count() == 0, "Type set count equals number of types in the set");
    STATIC_CHECK(A.count() == 1, "Type set count equals number of types in the set");
    STATIC_CHECK(AB.count() == 2, "Type set count equals number of types in the set");
    STATIC_CHECK(ABC.count() == 3, "Type set count equals number of types in the set");
    STATIC_CHECK(universe.count() == 4, "Type set count equals number of types in the set");
}

TEST_CASE_FIXTURE(test_fixture, "intersection operator")
{
    STATIC_CHECK((ABC & ABC) == ABC, "Intersection is idempotent");
    STATIC_CHECK((ABC & D).empty(), "Intersection between disjoint type sets is empty");
    STATIC_CHECK((universe & ABC) == ABC, "Intersection projects universe to subset");
    STATIC_CHECK((ABC & BCD) == (BCD & ABC), "Intersection is commutative");
    STATIC_CHECK((ABC & (BCD & DCA)) == ((ABC & BCD) & DCA), "Intersection is associative");
    STATIC_CHECK((ABC & BCD) == BC, "Intersection gives the common subset");
}

TEST_CASE_FIXTURE(test_fixture, "difference operator")
{
    STATIC_CHECK((AB / AB) == empty, "Difference is nilpotent");
    STATIC_CHECK((AB / empty) == AB, "Difference with empty set is first");
    STATIC_CHECK((AB / CD) == AB, "Difference of to disjoint sets is the first");
    STATIC_CHECK((AB / universe) == empty, "Difference of set and universe set is empty");
    STATIC_CHECK((universe / AB) == ~AB, "Difference of universe and set is complement");
}

TEST_CASE_FIXTURE(test_fixture, "complement operator")
{
    STATIC_CHECK((~empty) == universe, "Complement of the empty set is the universe");
    STATIC_CHECK((~~AB) == AB, "Complement is idempotent");
    STATIC_CHECK((~AB) == (universe / AB), "Complement gives difference from universe");
}

TEST_CASE_FIXTURE(test_fixture, "symmetric difference operator")
{
    STATIC_CHECK((AB ^ AB) == empty, "Symmetric difference is nilpotent");
    STATIC_CHECK((AB ^ CD) == universe, "Symmetric difference of to disjoint sets is join");
    STATIC_CHECK((CD ^ AB) == (AB ^ CD), "Symmetric difference is commutative");
    STATIC_CHECK((AB ^ empty) == AB, "Symmetric difference of subset and empty set is subset");
    STATIC_CHECK((AB ^ universe) == ~AB, "Symmetric difference of set and universe is complement");
}

TEST_CASE_FIXTURE(test_fixture, "subset operator")
{
    STATIC_CHECK(empty <= empty, "Empty type set is a subset of any type set");
    STATIC_CHECK(empty <= A, "Empty type set is a subset of any type set");
    STATIC_CHECK(empty <= AB, "Empty type set is a subset of any type set");
    STATIC_CHECK(empty <= ABC, "Empty type set is a subset of any type set");
    STATIC_CHECK(empty <= universe, "Empty type set is a subset of any type set");
    STATIC_CHECK(A <= universe, "Any type set is a subset of the universe");
    STATIC_CHECK(AB <= universe, "Any type set is a subset of the universe");
    STATIC_CHECK(ABC <= universe, "Any type set is a subset of the universe");
    STATIC_CHECK(universe <= universe, "Any type set is a subset of the universe");
    STATIC_CHECK(A <= A, "An type set is a subset of itself");
    STATIC_CHECK(AB <= AB, "An type set is a subset of itself");
    STATIC_CHECK(ABC <= ABC, "An type set is a subset of itself");
    STATIC_CHECK(A <= AB, "Subset is a subset of a superset");
    STATIC_CHECK(B <= AB, "Subset is a subset of a superset");
    STATIC_CHECK(AB <= ABC, "Subset is a subset of a superset");
    STATIC_CHECK(BC <= ABC, "Subset is a subset of a superset");
    STATIC_CHECK(!(AB <= CD), "Two disjoint type sets are not subsets of each other");
    STATIC_CHECK(!(CD <= AB), "Two disjoint type sets are not subsets of each other");
    STATIC_CHECK(!(AB <= BC), "Not partially ordered subsets are not subsets of each other");
    STATIC_CHECK(!(BC <= AB), "Not partially ordered subsets are not subsets of each other");
}

TEST_CASE_FIXTURE(test_fixture, "superset operator")
{
    STATIC_CHECK(empty >= empty, "Any type set is a superset of the empty type set");
    STATIC_CHECK(D >= empty, "Any type set is a superset of the empty type set");
    STATIC_CHECK(CD >= empty, "Any type set is a superset of the empty type set");
    STATIC_CHECK(BCD >= empty, "Any type set is a superset of the empty type set");
    STATIC_CHECK(universe >= empty, "Any type set is a superset of the empty type set");
    STATIC_CHECK(universe >= D, "Universe is superset of any type set");
    STATIC_CHECK(universe >= CD, "Universe is superset of any type set");
    STATIC_CHECK(universe >= BCD, "Universe is superset of any type set");
    STATIC_CHECK(universe >= universe, "Universe is superset of any type set");
    STATIC_CHECK(D >= D, "A type set is a superset of itself");
    STATIC_CHECK(CD >= CD, "A type set is a superset of itself");
    STATIC_CHECK(BCD >= BCD, "A type set is a superset of itself");
    STATIC_CHECK(BC >= B, "Superset is a superset of a subset");
    STATIC_CHECK(BC >= C, "Superset is a superset of a subset");
    STATIC_CHECK(BCD >= BC, "Superset is a superset of a subset");
    STATIC_CHECK(BCD >= CD, "Superset is a superset of a subset");
    STATIC_CHECK(!(AB >= CD), "Two disjoint type sets are not supersets of each other");
    STATIC_CHECK(!(CD >= AB), "Two disjoint type sets are not supersets of each other");
    STATIC_CHECK(!(BC <= CD), "Not partially ordered supersets are not supersets of each other");
    STATIC_CHECK(!(CD <= BC), "Not partially ordered supersets are not supersets of each other");
}

TEST_CASE_FIXTURE(test_fixture, "strict subset operator")
{
    STATIC_CHECK(!(empty < empty), "Empty type set is not a strict subset of itself");
    STATIC_CHECK(empty < A, "Empty type set is a strict subset of any non-empty type set");
    STATIC_CHECK(empty < AB, "Empty type set is a strict subset of any non-empty type set");
    STATIC_CHECK(empty < ABC, "Empty type set is a strict subset of any non-empty type set");
    STATIC_CHECK(empty < universe, "Empty type set is a subset of any non-empty type set");
    STATIC_CHECK(A < universe, "Any non-universal type set is a strict subset of the universe");
    STATIC_CHECK(AB < universe, "Any non-universal type set is a strict subset of the universe");
    STATIC_CHECK(ABC < universe, "Any non-universal type set is a strict subset of the universe");
    STATIC_CHECK(!(universe < universe), "Universe is not a strict subset of itself");
    STATIC_CHECK(!(A < A), "No type set is not a strict subset of itself");
    STATIC_CHECK(!(AB < AB), "No type set is not a strict subset of itself");
    STATIC_CHECK(!(ABC < ABC), "No type set is not a strict subset of itself");
    STATIC_CHECK(A < AB, "Subset is a strict subset of a strict superset");
    STATIC_CHECK(B < AB, "Subset is a strict subset of a strict superset");
    STATIC_CHECK(AB < ABC, "Subset is a strict subset of a strict superset");
    STATIC_CHECK(BC < ABC, "Subset is a strict subset of a strict superset");
    STATIC_CHECK(!(AB < CD), "Two disjoint subsets are not strict subsets of each other");
    STATIC_CHECK(!(CD < AB), "Two disjoint subsets are not strict subsets of each other");
    STATIC_CHECK(!(AB < BC), "Not partially ordered subsets are not strict subsets of each other");
    STATIC_CHECK(!(BC < AB), "Not partially ordered subsets are not strict subsets of each other");
}

TEST_CASE_FIXTURE(test_fixture, "strict superset operator")
{
    STATIC_CHECK(universe > A, "Universe is a strict superset of any non-universal type set");
    STATIC_CHECK(universe > AB, "Universe is a strict superset of any non-universal type set");
    STATIC_CHECK(universe > ABC, "Universe is a strict superset of any non-universal type set");
    STATIC_CHECK(!(universe > universe), "Universe is not a strict superset of itself");
    STATIC_CHECK(universe > empty, "Universe is a strict superset of any non-universal type set");
    STATIC_CHECK(universe > A, "Universe is a strict superset of any non-universal type set");
    STATIC_CHECK(universe > AB, "Universe is a strict superset of any non-universal type set");
    STATIC_CHECK(universe > ABC, "Universe is a strict superset of any non-universal type set");
    STATIC_CHECK(!(A > A), "No type set is not a strict superset of itself");
    STATIC_CHECK(!(AB > AB), "No type set is not a strict superset  itself");
    STATIC_CHECK(!(ABC > ABC), "No type set is not a strict superset of itself");
    STATIC_CHECK(AB > A, "Superset is a strict superset of a strict subset");
    STATIC_CHECK(AB > B, "Superset is a strict superset of a strict subset");
    STATIC_CHECK(ABC > AB, "Superset is a strict superset of a subset");
    STATIC_CHECK(ABC > BC, "Superset is a strict superset of a subset");
    STATIC_CHECK(!(AB > CD), "Two disjoint subsets are not strict supersets of each other");
    STATIC_CHECK(!(CD > AB), "Two disjoint subsets are not strict supersets of each other");
    STATIC_CHECK(!(AB > BC), "Not partially ordered subsets are not mutually strict supersets");
    STATIC_CHECK(!(BC > AB), "Not partially ordered subsets are not mutually strict supersets");
}

TEST_CASE("union assignment operator")
{
    auto message = test_set::make<code::A>();
    CHECK( message.has<code::A>());
    CHECK(!message.has<code::B>());
    message |= test_set::make<code::B>();
    CHECK( message.has<code::A>());
    CHECK( message.has<code::B>());
}

TEST_CASE("intersection assignment operator")
{
    auto message = test_set::make<code::A>() | test_set::make<code::B>();
    CHECK( message.has<code::A>());
    CHECK( message.has<code::B>());
    message &= test_set::make<code::B>();
    CHECK(!message.has<code::A>());
    CHECK( message.has<code::B>());
    message &= test_set::make<code::A>();
    CHECK( message.empty());
}

TEST_CASE("differential assignment operator")
{
    auto message = test_set::make<code::A>() | test_set::make<code::B>();
    CHECK( message.has<code::A>());
    CHECK( message.has<code::B>());
    message /= test_set::make<code::B>();
    CHECK( message.has<code::A>());
    CHECK(!message.has<code::B>());
    message /= test_set::make<code::A>();
    CHECK( message.empty());
}

TEST_CASE("symmetric differential assignment operator")
{
    test_set message;
    CHECK(!message.has<code::A>());
    CHECK(!message.has<code::B>());
    message ^= test_set::make<code::A>();
    CHECK( message.has<code::A>());
    CHECK(!message.has<code::B>());
    message ^= test_set::make<code::B>();
    CHECK( message.has<code::A>());
    CHECK( message.has<code::B>());
}

TEST_CASE("type_set add method works as expected")
{
    test_set message;
    CHECK( message.empty());
    message.add<code::A>();
    CHECK( message.has<code::A>());
    CHECK(!message.has<code::B>());
    message.add<code::A>();
    CHECK( message.has<code::A>());
    CHECK(!message.has<code::B>());
    message.add<code::B>();
    CHECK( message.has<code::A>());
    CHECK( message.has<code::B>());
}

TEST_CASE("type_set remove method works as expected")
{
    test_set message = test_set::make<code::A>() | test_set::make<code::B>();
    CHECK( message.has<code::A>());
    CHECK( message.has<code::B>());
    message.remove<code::A>();
    CHECK(!message.has<code::A>());
    CHECK( message.has<code::B>());
    message.remove<code::A>();
    CHECK(!message.has<code::A>());
    CHECK( message.has<code::B>());
    message.remove<code::B>();
    CHECK( message.empty());
}

TEST_CASE("type_set clear method works as expected")
{
    test_set X = test_set::make<code::A>() | test_set::make<code::B>();
    X.clear();
    CHECK(X.empty());
}

TEST_CASE_FIXTURE(test_fixture, "type set visit visits the expected types")
{
    std::vector<size_t> indices;

    SUBCASE("for B")
    {
        visit(test_visitor(indices), B);
        REQUIRE(indices.size() == 1);
        CHECK(indices.at(0) == 1);
    }

    SUBCASE("for BC")
    {
        visit(test_visitor(indices), BC);
        REQUIRE(indices.size() == 2);
        CHECK(indices.at(0) == 1);
        CHECK(indices.at(1) == 2);
    }

    SUBCASE("for ABC")
    {
        visit(test_visitor(indices), ABC);
        REQUIRE(indices.size() == 3);
        CHECK(indices.at(0) == 0);
        CHECK(indices.at(1) == 1);
        CHECK(indices.at(2) == 2);
    }

    SUBCASE("for ABCD")
    {
        visit(test_visitor(indices), universe);
        REQUIRE(indices.size() == 4);
        CHECK(indices.at(0) == 0);
        CHECK(indices.at(1) == 1);
        CHECK(indices.at(2) == 2);
        CHECK(indices.at(3) == 3);
    }
}
