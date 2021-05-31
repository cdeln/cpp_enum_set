FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive

# Install standard dependencies.
RUN apt-get update && apt-get install -y \
    build-essential \
    clang \
    clang-tidy \
    cppcheck \
    doctest-dev \
    doxygen \
    curl \
    git \
    lcov \
    python3-pip \
    tar \
    unzip \
    wget \
    zip

# Install minimal CMake version
ENV CMAKE_VERSION=3.19.8
RUN wget https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/cmake-${CMAKE_VERSION}-Linux-x86_64.tar.gz \
    && tar -xf cmake-${CMAKE_VERSION}-Linux-x86_64.tar.gz \
    && cp -r cmake-${CMAKE_VERSION}-Linux-x86_64/* /usr/ \
    && rm -rf cmake-${CMAKE_VERSION}-Linux-x64-64

# Install vcpkg
ENV VCPKG_DISABLE_METRICS=
ENV VCPKG_ROOT=/vcpkg
RUN git clone https://github.com/microsoft/vcpkg \
    && ./vcpkg/bootstrap-vcpkg.sh

# Install python dependencies
RUN pip3 install jinja2 Pygments
