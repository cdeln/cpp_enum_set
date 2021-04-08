#define MAGIC_ENUM_RANGE_MAX (1 << 9)
#include <enum_set/magic/magic_enum_set.hpp>

#include <iostream>
#include <string>
#include <vector>

// Define an option as an unscoped enumeration.
// This reflects a "legacy enum" that produce an integer bit mask when "or'ed".
// Note that `magic_enum` requires us to set the `MAGIC_ENUM_RANGE_MAX` value
// big enough in order to recognize `x9` below.
enum option
{
    x0 = (1 << 0),
    x1 = (1 << 1),
    x2 = (1 << 2),
    x3 = (1 << 3),
    x4 = (1 << 4),
    x5 = (1 << 5),
    x6 = (1 << 6),
    x7 = (1 << 7),
    x8 = (1 << 8),
    x9 = (1 << 9)
};

// Define a set of options as a `value_set` using the magical `make_magic_enum_set` meta function.
using option_set =
    enum_set::make_magic_enum_set<
        option
    >;

// Overload the join operator for the option enum so that we get sets instead.
option_set operator|(option lhs, option rhs)
{
    return option_set(lhs, rhs);
}

int main()
{
    // Select some options.
    const option_set options = x1 | x3 | x7 | x8;

    // Print the options in the option set.
    for (auto option : options)
    {
        switch (option)
        {
            case x0: std::cout << "x0\n"; break;
            case x1: std::cout << "x1\n"; break;
            case x2: std::cout << "x2\n"; break;
            case x3: std::cout << "x3\n"; break;
            case x4: std::cout << "x4\n"; break;
            case x5: std::cout << "x5\n"; break;
            case x6: std::cout << "x6\n"; break;
            case x7: std::cout << "x7\n"; break;
            case x8: std::cout << "x8\n"; break;
            case x9: std::cout << "x9\n"; break;
        }
    }
}
