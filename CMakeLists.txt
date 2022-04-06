cmake_minimum_required(VERSION 3.16) 
project(ScopedEnum)

set(CMAKE_CXX_STANDARD 14)

add_library(Catch2 INTERFACE)
target_include_directories(Catch2 INTERFACE "../common/include")

add_library(Poker INTERFACE)
target_include_directories(Poker INTERFACE .)

add_library(Metaprogramming INTERFACE)
target_include_directories(Metaprogramming INTERFACE .)

add_executable(ScopedEnumTest CardTest.cpp)
target_link_libraries(ScopedEnumTest Catch2 Poker)

add_executable(PokerHandTest PokerHandTest.cpp)
target_link_libraries(PokerHandTest Catch2 Poker)

add_executable(PokerHandAllPossible PokerHandAllPossible.cpp)
target_link_libraries(PokerHandAllPossible Poker)


enable_testing()

add_test(NAME "CardTest" COMMAND ScopedEnumTest)
add_test(NAME "PokerHandTest" COMMAND PokerHandTest)
add_subdirectory(tests)