FROM ubuntu:20.04

ENV DEBIAN_FRONTEND noninteractive

# Install standard dependencies.
RUN apt-get update && apt-get install -y \
    build-essential \
    clang \
    cmake \
    doctest-dev \
    gcovr \
    git \
    wget

# Install cppcheck.
# The version provided by apt is too old.
RUN wget https://github.com/danmar/cppcheck/archive/2.4.1.tar.gz -O cppcheck.tar.gz \
    && mkdir cppcheck-src \
    && tar -xf cppcheck.tar.gz --strip-components 1 -C cppcheck-src \
    && cmake -B cppcheck-bin -S cppcheck-src -DCMAKE_BUILD_TYPE=Release \
    && cmake --build cppcheck-bin --parallel \
    && cmake --install cppcheck-bin

# Install magic_enum.
RUN git clone https://github.com/Neargye/magic_enum.git \
    && cd magic_enum \
    && git checkout v0.7.2 \
    && cmake -B build -S . \
    && cmake --install build
