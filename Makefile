CXXFLAGS       := -Wall -Wextra -Wpedantic -Werror

sources        := $(wildcard include/enum_set/*.hpp)
example_srcs   := $(wildcard example/*.cpp)
example_exes   := $(patsubst example/%.cpp,build/%.exe,$(example_srcs)) build/magic_enum_set_example.exe
test_sources   := $(wildcard test/*.cpp)
test_cov_objs  := $(patsubst test/%.cpp,build/%.coverage.o,$(test_sources)) build/test_magic.coverage.o
test_san_objs  := $(patsubst test/%.cpp,build/%.sanitize.o,$(test_sources)) build/test_magic.sanitize.o
test_exes      := build/test_coverage.exe build/test_sanitize.exe
base_flags     := $(CXXFLAGS) -O0 -g3 -Iinclude
test_flags     := $(base_flags) -Itest
coverage_flags := $(test_flags) --coverage
sanitize_flags := $(test_flags) -fsanitize-address-use-after-scope -fsanitize=address,undefined
report_flags   := -r include/ --object-directory . --exclude-throw-branches --exclude-unreachable-branches --use-gcov-files

all: $(test_exes) $(example_exes)

build/test_coverage.exe: $(test_cov_objs) | build
	$(CXX) -std=c++14 $(coverage_flags) $^ -o $@

build/test_sanitize.exe: $(test_san_objs) | build
	$(CXX) -std=c++14 $(sanitize_flags) $^ -o $@

build/%.coverage.o: test/%.cpp $(sources) | build
	$(CXX) -std=c++14 $(coverage_flags) -c $< -o $@

build/%.sanitize.o: test/%.cpp $(sources) | build
	$(CXX) -std=c++14 $(sanitize_flags) -c $< -o $@

build/test_magic.coverage.o: test/magic/test_magic_enum_set.cpp include/enum_set/magic/magic_enum_set.hpp | build
	$(CXX) -std=c++17 $(coverage_flags) -c $< -o $@

build/test_magic.sanitize.o: test/magic/test_magic_enum_set.cpp include/enum_set/magic/magic_enum_set.hpp | build
	$(CXX) -std=c++17 $(sanitize_flags) -c $< -o $@

build/%.exe: example/%.cpp $(sources) | build
	$(CXX) -std=c++14 $(base_flags) $< -o $@

build/magic_enum_set_example.exe: example/magic/magic_enum_set_example.cpp $(sources) | build
	$(CXX) -std=c++17 $(base_flags) $< -o $@

build:
	mkdir build

.PHONY: analyze test_coverage test_sanitize test cover report install clean docker_build docker_run

analyze:
	cppcheck --bug-hunting --clang=$(shell which clang) -I ./include/ ./include/enum_set/*

test_coverage: ./build/test_coverage.exe
	./build/test_coverage.exe

test_sanitize: ./build/test_sanitize.exe
	./build/test_sanitize.exe

test: test_coverage test_sanitize

cover: test_coverage
	gcov -b -c -m -r ./build/*.coverage.o

report: cover
	gcovr $(report_flags) --keep
	gcovr $(report_flags) --keep --branches
	gcovr $(report_flags) --html-details -o report.html

install:
	cp -r include/* /usr/local/include/

docker_build:
	docker build -t cpp_enum_set:latest .

docker_run:
	docker run --rm -it -v $(shell pwd):/cpp_enum_set -w /cpp_enum_set cpp_enum_set:latest

clean:
	rm -rf build
	rm -f *.gcov *.gcda *.gcno
	rm -f report.html report.*.html
