# cpp_enum_set

[![Build status](https://github.com/cdeln/cpp_enum_set/actions/workflows/ci.yml/badge.svg)](https://github.com/cdeln/cpp_enum_set/actions)
[![Code coverage](https://codecov.io/gh/cdeln/cpp_enum_set/branch/master/graph/badge.svg)](https://codecov.io/gh/cdeln/cpp_enum_set)
[![GitHub release](https://img.shields.io/github/v/release/cdeln/cpp_enum_set)](https://github.com/cdeln/cpp_enum_set/releases)
[![License](https://img.shields.io/github/license/cdeln/cpp_enum_set)](https://github.com/cdeln/cpp_enum_set/blob/master/LICENSE)

A library of type safe sets over fixed size collections of types or values,
including methods for accessing, modifying, visiting and iterating over those.

## Rationale

Enums are great, as long as you use them to represent individual flags or options.
If you want to represent a set of options using an enumeration,
one would typically let the values of the enumeration take on powers of two and then "or" them together

```cpp
enum class option
{
    A = (1 << 0),
    B = (1 << 1),
    C = (1 << 2)
};

int operator|(option lhs, option rhs)
{
    return static_cast<int>(lhs) | static_cast<int>(rhs);
}
```

The problem with this is obvious: when using an integer representation for a set of options, we lose type information.
Instead, one would like to do something like this

```cpp
#include <enum_set/enum_set.hpp>

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
```

where a type alias `option_set` is introduced to represent a set of values from the `option` enumeration.

This library provides means of doing this for various types of enumerated collections.

## Requirements

`enum_set` is a zero dependency (except from standard library) header only library.

Requires at least C++14, and C++17 if you want to use it together with [magic enum](https://github.com/Neargye/magic_enum).

## Installation

There are two ways of installing this library

  1. Poor mans installation: Just copy the contents of the `include` folder to your installation path.
  2. Using CMake: see [BUILDING][1] for detailed installation instructions.

## Usage

The library provides a `type_set` class for holding a set of types and `value_set` class for holding a set of values.

If you roll with strong enums (that is, struct with class tags),
use the `type_set` class (defined in [`<enum_set/type_set.hpp>`](include/enum_set/type_set.hpp)).

If you want to model a set over a fixed collection of values, use the `value_set` class
(defined in [`<enum_set/value_set.hpp>`](include/enum_set/value_set.hpp)).
The `value_set` is a refinement of the `type_set` class that replaces type like semantics with value like.
Thus, if `type_set` provides a template method that accepts a type,
`value_set` provides the a method with the same name accepting a value (of the given type) instead.
In addition to the capabilities provided by `type_set`, `value_set` also provides support for iteration
(there is support for visitation which is more safe and also works for `type_set`).

The library provides type aliases
`integer_set` (defined in [`<enum_set/integer_set.hpp>`](include/enum_set/integer_set.hpp))
and `index_set` (defined in [`<enum_set<index_set.hpp>`](include/enum_set/index_set.hpp))
for a `value_set` of integers or indices (a.k.a. `std::size_t`) respectively.
Meta functions `make_integer_set` and `make_index_set` for creating sets over contiguous ranges of integers or indices are also provided
(mimicks the style used by STL's `std::make_integer_sequence` and `std::make_index_sequence`).

Finally, if you use enums (*drumroll*...), you can use the `make_enum_set` meta function
(defined in [`<enum_set/enum_set.hpp>`](include/enum_set/enum_set.hpp)).
This assumes that you do not set the enumeration values manually (e.g. as powers of two),
Also, as seen in the rationale example in the beginning, note that you have to explicitly provide the last enumeration value
(until C++ have better built in reflection capabilities for enums).
If you can use C++17, are willing to sacrifice standardness and to introduce another dependency,
integration with [magic enum](https://github.com/Neargye/magic_enum) is provided and you can create
a `value_set` conveniently using the `make_magic_enum_set` meta function
(defined in [`<enum_set/magic/magic_enum_set.hpp>`](include/enum_set/magic/magic_enum_set.hpp))
as follows

```cpp
#include <enum_set/magic/magic_enum_set.hpp>

enum class option
{
    A,
    B,
    C
};

using option_set =
    enum_set::make_magic_enum_set<
        option
    >;
```

See [this example](example/basic_tutorial.cpp) for a tutorial on available methods and operators.

See [this example](example/visitation_example.cpp) for an illustration of the visitor pattern with `type_set`.

Reference documentation can be found [here](https://cdeln.github.io/cpp_enum_set).

## Contributing

Feel free to grab an issue from the [issue list][3]. Make a fork and submit a PR.
We currently don't have any dedicated forum for discussion,
if you have any ideas on how to improve the library and there is no issue for it,
feel free to submit an issue yourself and we can discuss the idea there.

See [HACKING][2] for detailed instructions on setting up the developer environment.

See the [Dockerfile][4] for a minimal reproducible environment.
Running the instructions given by the other documents inside the built container should work.
The Dockerfile should, but is not guaranteed to, reflect the steps used by the CI.
To see the exact steps used by the CI, see the [GitHub workflow config file][5].

[1]: BUILDING.md
[2]: HACKING.md
[3]: https://github.com/cdeln/cpp_enum_set/issues
[4]: Dockerfile
[5]: .github/workflows/ci.yml
