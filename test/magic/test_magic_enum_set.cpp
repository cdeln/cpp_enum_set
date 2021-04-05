#include "testing.hpp"

#define MAGIC_ENUM_RANGE_MAX (1 << 8)
#include <enum_set/magic/magic_enum_set.hpp>

namespace
{

enum test_unscoped_enum
{
    unscoped_0 = (1 << 0),
    unscoped_1 = (1 << 1),
    unscoped_2 = (1 << 2),
    unscoped_3 = (1 << 3),
    unscoped_4 = (1 << 4),
    unscoped_5 = (1 << 5),
    unscoped_6 = (1 << 6),
    unscoped_7 = (1 << 7),
    unscoped_8 = (1 << 8)
};

enum test_scoped_enum
{
    scoped_0 = (1 << 0),
    scoped_1 = (1 << 1),
    scoped_2 = (1 << 2),
    scoped_3 = (1 << 3),
    scoped_4 = (1 << 4),
    scoped_5 = (1 << 5),
    scoped_6 = (1 << 6),
    scoped_7 = (1 << 7),
    scoped_8 = (1 << 8)
};

}  // namespace

namespace enum_set
{

TEST_CASE("make_magic_enum_set builds the expected enum set")
{
    STATIC_CHECK(
        (std::is_same<
            make_magic_enum_set<test_unscoped_enum>,
            value_set<
                test_unscoped_enum,
                unscoped_0,
                unscoped_1,
                unscoped_2,
                unscoped_3,
                unscoped_4,
                unscoped_5,
                unscoped_6,
                unscoped_7,
                unscoped_8
            >
        >::value),
        "make_magic_enum_set builds the expected enum set");

    STATIC_CHECK(
        (std::is_same<
            make_magic_enum_set<test_scoped_enum>,
            value_set<
                test_scoped_enum,
                test_scoped_enum::scoped_0,
                test_scoped_enum::scoped_1,
                test_scoped_enum::scoped_2,
                test_scoped_enum::scoped_3,
                test_scoped_enum::scoped_4,
                test_scoped_enum::scoped_5,
                test_scoped_enum::scoped_6,
                test_scoped_enum::scoped_7,
                test_scoped_enum::scoped_8
            >
         >::value),
        "make_magic_enum_set builds the expected enum set");
}

}  // namespace enum_set
