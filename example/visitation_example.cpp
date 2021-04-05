#include <enum_set/type_set.hpp>

#include <iostream>
#include <string>
#include <vector>

// Define an option as a "strong enum".
struct option
{
    class A;
    class B;
    class C;
};

// Define a set of options as a `type_set`.
using option_set =
    enum_set::type_set<
        option::A,
        option::B,
        option::C
    >;

// Define a an `option_tag` class that can be used for tag dispatching.
template <typename T>
struct option_tag
{
};

// Define a functor `option_name` that maps an `option_tag` to a `std::string`.
struct option_name
{
    std::string operator()(option_tag<option::A>) { return "A"; }
    std::string operator()(option_tag<option::B>) { return "B"; }
    std::string operator()(option_tag<option::C>) { return "C"; }
};

// Define a visitor that can be used on a `enum_set::type_set`.
// The visitor should define an templated call operators without parameters.
struct option_set_visitor
{
    std::vector<std::string> result;

    template <typename Option>
    void operator()()
    {
        result.push_back(option_name()(option_tag<Option>()));
    }
};

int main()
{
    // Select the options we want to use.
    const option_set options
      = option_set::make<option::A>()
      | option_set::make<option::C>()
      ;

    // Make a visitor and visito the options.
    option_set_visitor visitor;
    visit(visitor, options);

    // Print the names of the selected options.
    std::cout << "Compute options given:\n";
    for (auto const& name : visitor.result)
    {
        std::cout << "  - " << name << '\n';
    }
}
