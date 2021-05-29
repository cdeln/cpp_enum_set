# Hacking

Here is some wisdom to help you build and test this project as a developer and
potential contributor.

## Developer mode

Build system targets that are only useful for developers of this project are
hidden if the `enum_set_DEVELOPER_MODE` option is disabled. Enabling this
option makes tests and other developer targets and options available. Not
enabling this option means that you are a consumer of this project and thus you
have no need for these targets and options.

Developer mode is always set to on in CI workflows.

### Presets

This project makes use of [presets][1] to simplify the process of configuring
the project. As a developer, you are recommended to always have the [latest
CMake version][2] installed to make use of the latest Quality-of-Life
additions.

You have a few options to pass `enum_set_DEVELOPER_MODE` to the configure
command, but this project prefers to use presets.

As a developer, you should create a `CMakeUserPresets.json` file at the root of
the project:

```json
{
  "version": 1,
  "cmakeMinimumRequired": {
    "major": 3,
    "minor": 14,
    "patch": 0
  },
  "configurePresets": [
    {
      "name": "dev-base",
      "hidden": true,
      "inherits": ["dev-mode", "vcpkg", "ci-<os>"]
    },
    {
      "name": "dev",
      "buildDir": "${sourceDir}/build/dev",
      "inherits": "dev-base",
      "cacheVariables": {
        "VCPKG_MANIFEST_FEATURES": "test"
      }
    },
    {
      "name": "dev-magic_enum",
      "buildDir": "${sourceDir}/build/dev-magic_enum",
      "inherits": "dev-base",
      "cacheVariables": {
        "VCPKG_MANIFEST_FEATURES": "test;magic-enum"
      }
    }
  ]
}
```

You should replace `<os>` in your newly created presets file with the name of
the operating system you have, which may be `win64` or `unix`. You can see what
these correspond to in the [`CMakePresets.json`](CMakePresets.json) file.

`CMakeUserPresets.json` is also the perfect place in which you can put all
sorts of things that you would otherwise want to pass to the configure command
in the terminal.

### Configure, build and test

To build the project as a developer, please make sure you have [vcpkg][3]
installed and properly set up at least with the `VCPKG_ROOT` env var pointing
to the right place, because some dependencies are provided with ports in the
project.

If you followed the above instructions, then you can configure, build and test
the project respectively with the following commands from the project root on
Windows:

```sh
cmake --preset=dev
cmake --build build --config Release
ctest --test-dir build -C Release
```

And here is the same on a Unix based system (Linux, macOS):

```sh
cmake --preset=ci-coverage
cmake --build build
ctest --test-dir build
cmake --build --target coverage
```

[1]: https://cmake.org/cmake/help/latest/manual/cmake-presets.7.html
[2]: https://cmake.org/download/
[3]: https://github.com/microsoft/vcpkg
