#include <enum_set/enum_set.hpp>

#include <iostream>

enum class option
{
    A,
    B,
    C
};

using option_set =
    enum_set::make_enum_set<
        option,
        option::C
    >;

option_set operator|(option lhs, option rhs)
{
    return {lhs, rhs};
}

int main()
{
    option_set options
        = option::A
        | option::B
        | option::C
        ;
    std::cout << std::boolalpha
              << "options has A ? " << options.has<option::A>() << '\n'
              << "options has B ? " << options.has<option::B>() << '\n'
              << "options has C ? " << options.has<option::C>() << '\n';
}
