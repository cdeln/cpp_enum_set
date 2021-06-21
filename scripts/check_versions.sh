#!/bin/bash

build2_version=$(cat manifest | grep version | cut -d' ' -f2)
cmake_version=$(cat CMakeLists.txt | grep CPP_ENUM_SET_VERSION | head -n 1 | cut -d' ' -f2)
vcpkg_version=$(jq '."version-semver"' vcpkg.json | sed 's/"//g')

if [ $build2_version != $cmake_version ]
then
    echo "build2 version $build2_version does not match cmake version $cmake_version"
    exit 1
fi

if [ $build2_version != $vcpkg_version ]
then
    echo "build2 version $build2_version does not match vcpkg version $vcpkg_version"
    exit 1
fi
