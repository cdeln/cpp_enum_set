#ifndef ENUM_SET_TEST_TESTING_HPP
#define ENUM_SET_TEST_TESTING_HPP

#include <doctest/doctest.h>

#define STATIC_CHECK(Expression, Message) \
    static_assert(Expression, Message); \
    CHECK(Expression);

#define STATIC_REQUIRE(Expression, Message) \
    static_assert(Expression, Message); \
    REQUIRE(Expression);

#endif // ENUM_SET_TEST_TESTING_HPP
