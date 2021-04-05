#include "testing.hpp"

#include <enum_set/enum_set.hpp>

#include <type_traits>

namespace
{

enum test_unscoped_enum
{
    unscoped_first,
    unscoped_second,
    unscoped_third,
    unscoped_last
};

enum test_scoped_enum
{
    first,
    second,
    third,
    last
};

}  // namespace

namespace enum_set
{

TEST_CASE("make_enum_set builds the expected value_set")
{
    STATIC_CHECK(
        (std::is_same<
            make_enum_set<test_unscoped_enum, test_unscoped_enum::unscoped_last>,
            value_set<
                test_unscoped_enum,
                unscoped_first,
                unscoped_second,
                unscoped_third,
                unscoped_last
            >
         >::value),
        "make_enum_set builds the expected value_set");

    STATIC_CHECK(
        (std::is_same<
            make_enum_set<test_scoped_enum, test_scoped_enum::last>,
            value_set<
                test_scoped_enum,
                test_scoped_enum::first,
                test_scoped_enum::second,
                test_scoped_enum::third,
                test_scoped_enum::last
            >
         >::value),
        "make_enum_set builds the expected value_set");
}

}  // namespace enum_set
