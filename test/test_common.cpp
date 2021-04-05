#include "testing.hpp"

#include <enum_set/common.hpp>

using namespace ::enum_set;

TEST_CASE("all returns true only if all arguments are true")
{
    STATIC_CHECK(detail::all(), "All with no arguments is always true");
    STATIC_CHECK(detail::all(true), "All of one true is true");
    STATIC_CHECK(!detail::all(false), "All of false is false");
    STATIC_CHECK(detail::all(true, true), "All of multiple true is true");
    STATIC_CHECK(!detail::all(false, true), "All of multiple booleans with one false is false");
    STATIC_CHECK(!detail::all(true, false), "All of multiple booleans with one false is false");
    STATIC_CHECK(!detail::all(false, false), "All of multiple false is false");
}

TEST_CASE("any returns true if atleast one argument is true")
{
    STATIC_CHECK(!detail::any(), "Any with no arguments is always false");
    STATIC_CHECK(detail::any(true), "Any of one true is true");
    STATIC_CHECK(!detail::any(false), "Any of false is false");
    STATIC_CHECK(detail::any(true, true), "Any of multiple true is true");
    STATIC_CHECK(detail::any(false, true), "Any of multiple booleans with one true is true");
    STATIC_CHECK(detail::any(true, false), "Any of multiple booleans with one true is true");
    STATIC_CHECK(!detail::any(false, false), "Any of all false is false");
}

TEST_CASE("count returns the number of trues in a set of bools")
{
    STATIC_CHECK(detail::count() == 0, "Count of nothing is 0");
    STATIC_CHECK(detail::count(false) == 0, "Count of one false is 0");
    STATIC_CHECK(detail::count(true) == 1, "Count of one true is 1");
    STATIC_CHECK(detail::count(false, false) == 0, "Count of all false is 0");
    STATIC_CHECK(detail::count(false, true) == 1, "Count of bools with one true is 1");
    STATIC_CHECK(detail::count(true, false) == 1, "Count of bools with one true is 1");
    STATIC_CHECK(detail::count(true, true) == 2, "Count of multiple true is the number of true");
}

TEST_CASE("index_of gives the expected index for a singleton type list")
{
    STATIC_CHECK((detail::index_of<int, int>::value == 0),
                 "Index of present type is the index in the type list");
    STATIC_CHECK((detail::index_of<bool, int>::value == 1),
                 "Index of non-present type is the size of the type list");
}

TEST_CASE("index_of gives the expected index for a general type list")
{
    STATIC_CHECK((detail::index_of<int,    int, bool, int>::value == 0),
                 "Index of present type is the index of its first occurence in the type list");
    STATIC_CHECK((detail::index_of<bool,   int, bool, int>::value == 1),
                 "Index of present type is the index of its first occurence in the type list");
    STATIC_CHECK((detail::index_of<float, int, bool, int>::value == 3),
                 "Index of non-present type is the size of the type list");
}

TEST_CASE("index_of_value gives the expected index for a singleton value list")
{
    STATIC_CHECK((detail::index_of_value<int, 1>(1) == 0),
                 "Index of present value is the index in the value list");
    STATIC_CHECK((detail::index_of_value<int, 1>(0) == 1),
                 "Index of non-present value is the size of the value list");
}

TEST_CASE("index_of_value gives the expected index for a general value list")
{
    STATIC_CHECK((detail::index_of_value<int, 1, 2, 1>(1) == 0),
                 "Index of present value is the index of its first occurence in the value list");
    STATIC_CHECK((detail::index_of_value<int, 1, 2, 1>(2) == 1),
                 "Index of present value is the index of its first occurence in the value list");
    STATIC_CHECK((detail::index_of_value<int, 1, 2, 1>(3) == 3),
                 "Index of non-present value is the size of the value list");
}

TEST_CASE("get_value returns the correct value for valid index")
{
    STATIC_CHECK((detail::get_value<int, 1, 2, 1>(0) == 1), "Value at valid index is correct");
    STATIC_CHECK((detail::get_value<int, 1, 2, 1>(1) == 2), "Value at valid index is correct");
    STATIC_CHECK((detail::get_value<int, 1, 2, 1>(2) == 1), "Value at valid index is correct");
}

TEST_CASE("get_value throws an exception on out of range access")
{
    CHECK_THROWS((detail::get_value<int, 1, 2, 1>(3)));
    CHECK_THROWS((detail::get_value<size_t, 1, 2, 1>(3)));
}
