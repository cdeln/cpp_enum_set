#include <enum_set/enum_set.hpp>

#include <iostream>
#include <sstream>
#include <string>

// Define an option as a scoped enumeration.
enum class option
{
    A,
    B,
    C
};

// Define a set of options as a `value_set` built using the `make_enum_set` meta function.
using option_set =
    enum_set::make_enum_set<
        option,
        option::C
    >;

// Define a string conversion function for the `option_set`.
std::string to_string(option_set const& options)
{
    std::stringstream builder;
    builder << "{";
    for (auto option : options)
    {
        switch (option)
        {
            case option::A: builder << " A "; break;
            case option::B: builder << " B "; break;
            case option::C: builder << " C "; break;
        }
    }
    builder << "}";
    return builder.str();
}

// Define the output stream operator for an `option_set`.
std::ostream& operator<<(std::ostream& os, option_set const& options)
{
    return os << to_string(options);
}

int main()
{
    constexpr option_set A = option_set::make<option::A>();
    constexpr option_set B = option_set::make<option::B>();
    constexpr option_set C = option_set::make<option::C>();
    constexpr option_set AB = A | B;
    constexpr option_set BC = B | C;
    constexpr option_set ABC = A | B | C;

    std::cout << std::boolalpha;
    std::cout << "A: " << A << '\n';
    std::cout << "A size: " << A.size() << '\n';
    std::cout << "A count (signed size): " << A.count() << '\n';
    std::cout << "A capacity: " << A.capacity() << '\n';
    std::cout << "A has A? " << A.has<option::A>() << '\n';
    std::cout << "A has B? " << A.has<option::B>() << '\n';
    std::cout << "A has C? " << A.has<option::C>() << '\n';

    std::cout << '\n';

    std::cout << "B: " << B << '\n'
              << "C: " << C << '\n'
              << "AB: " << AB << '\n'
              << "BC: " << BC << '\n'
              << "ABC: " << ABC << '\n';

    std::cout << '\n';

    option_set X;
    std::cout << "Default constructed X: " << X << '\n';

    X.add<option::A>();
    std::cout << "X after add A: " << X << '\n';
    X |= BC;
    std::cout << "X after in place union with " << BC << ": " << X << '\n';
    X &= AB;
    std::cout << "X after in place intersection with " << AB << ": " << X << '\n';
    X /= BC;
    std::cout << "X after in place set difference with " << BC << ": " << X << '\n';
    X ^= ABC;
    std::cout << "X after in place symmetric set difference with " << ABC << ": " << X << '\n';
    X.remove<option::C>();
    std::cout << "X after remove C: " << X << '\n';
    X.clear();
    std::cout << "X after clear: " << X << '\n';
    std::cout << "X empty? " << X.empty() << '\n';

    std::cout << '\n';

    std::cout << AB << " subset of " << BC << " ? " << (AB <= BC) << '\n';
    std::cout << AB << " subset of " << ABC << " ? " << (AB <= ABC) << '\n';
    std::cout << AB << " strict subset of " << ABC << " ? " << (AB < ABC) << '\n';
    std::cout << AB << " subset of " << AB << " ? " << (AB <= AB) << '\n';
    std::cout << AB << " strict subset of " << AB << " ? " << (AB < AB) << '\n';
}
