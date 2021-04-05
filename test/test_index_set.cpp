#include "testing.hpp"

#include <enum_set/index_set.hpp>
#include <enum_set/standard_types.hpp>

#include <type_traits>

namespace enum_set
{

TEST_CASE("base type of index set is as expected")
{
    STATIC_CHECK(
        (std::is_base_of<
            type_set<
                index<0>, index<1>, index<2>,
                index<3>, index<4>, index<5>,
                index<6>, index<7>, index<8>
            >,
            make_index_set<9>
         >::value),
        "index set of size 9 is type set of indices between 0 and 9");
}

TEST_CASE("size of index set of specified size is as expected")
{
    STATIC_CHECK(sizeof(make_index_set<1>) == 1,  "Index set of size 1 requires 1 byte");
    STATIC_CHECK(sizeof(make_index_set<8>) == 1,  "Index set of size 8 requires 1 byte");
    STATIC_CHECK(sizeof(make_index_set<9>) == 2,  "Index set of size 9 requires 2 bytes");
    STATIC_CHECK(sizeof(make_index_set<16>) == 2, "Index set of size 16 requires 2 bytes");
    STATIC_CHECK(sizeof(make_index_set<17>) == 3, "Index set of size 17 requires 3 bytes");
}

} // namespace enum_set
