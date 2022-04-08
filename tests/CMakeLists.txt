add_executable(metaprogramming_test metaprogramming_test.cpp)
target_link_libraries(metaprogramming_test Catch2 Metaprogramming)

add_executable(PokerHandAllPossible PokerHandAllPossible_Test.cpp)
target_link_libraries(PokerHandAllPossible Poker)

add_executable(PokerHandTest PokerHandTest.cpp)
target_link_libraries(PokerHandTest Catch2 Poker)

add_executable(ScopedEnumTest CardTest.cpp)
target_link_libraries(ScopedEnumTest Catch2 Poker)

enable_testing()

add_test(NAME "CardTest" COMMAND ScopedEnumTest)
add_test(NAME "PokerHandTest" COMMAND PokerHandTest)

add_test(NAME "Metaprogramming_Test" COMMAND metaprogramming_test)