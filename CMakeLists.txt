cmake_minimum_required(VERSION 3.16) 
project(ScopedEnum)

set(CMAKE_CXX_STANDARD 14)

add_library(Catch2 INTERFACE)
target_include_directories(Catch2 INTERFACE "../common/include")

add_library(Poker INTERFACE)
target_include_directories(Poker INTERFACE .)

add_library(Metaprogramming INTERFACE)
target_include_directories(Metaprogramming INTERFACE .)

add_executable(PokerGame PokerGameDriver.cpp)
target_link_libraries(PokerGame Poker)

add_subdirectory(tests)