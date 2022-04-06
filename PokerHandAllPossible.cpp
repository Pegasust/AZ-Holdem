#include <iostream>

#include "PokerHand.hpp"
#include "52Cards.hpp"


template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
    os << "[";
    for(auto itr = v.cbegin(); itr != v.cend(); ++itr) {
        if(itr != v.cbegin()) {
            os << ", ";
        }
        os << *itr;
    }
    return os << "]";
}
int main() {
    PokerHand hand(HA, DA, SA, CA, C10);
    auto all_hands = hand.getAllPossibleHands();
    std::cout << all_hands << std::endl;
    hand.addCard(CJ)
        .addCard(C8);
    std::cout << hand << std::endl;
    std::cout << hand.getAllPossibleHands() << std::endl;
    return 0;
}