#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <cstring>

#include "metaprogramming.hpp"


TEST_CASE("make_optional with copy ctor") 
{
    // primitives
    auto int12 = make_optional(12);
    REQUIRE(int12.has_value());
    REQUIRE(int12.value() == 12);

    auto long_0xdeadbeef = make_optional<long>(0xdeadbeef);
    REQUIRE(long_0xdeadbeef.has_value());
    REQUIRE(long_0xdeadbeef.value() == 0xdeadbeef);
    *long_0xdeadbeef = 0xdeadbeefcafeL;
    REQUIRE(long_0xdeadbeef.has_value());
    REQUIRE(long_0xdeadbeef.value() == 0xdeadbeefcafeL);

    // well-written objects
    auto cstr_hello_world = make_optional("hello world");
    REQUIRE(cstr_hello_world.has_value());
    REQUIRE(strcmp(cstr_hello_world.value(), "hello world") == 0);
};