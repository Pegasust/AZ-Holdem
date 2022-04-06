//
// Created by _nhitrl on 3/29/2022.
//

#ifndef SCOPEDENUM_POKERHAND_HPP
#define SCOPEDENUM_POKERHAND_HPP

#include <vector>
#include <array>
#include <cassert>
#include <optional>
#include <algorithm>

#include "Card.hpp"
#include "metaprogramming.hpp"

enum class HandType: int {
    STRAIGHT_FLUSH=7,
    SAME_4=6,
    FULL_HOUSE=5,
    FLUSH=4,
    SAME_3=3,
    DOUBLE_PAIR=2,
    PAIR=1,
    HIGH=0
};

std::ostream& operator<<(std::ostream& os, const HandType& type) {
    static std::string hand_type_str[] = {
        "High card"
        "Pair",
        "Double pairs",
        "Three-of-a-kind",
        "Flush",
        "Fullhouse",
        "Four-of-a-kind",
        "Straight",
    };
    return os << hand_type_str[static_cast<int>(type)];
}

/**
 * @brief Simple data-type for value of a given hand.
 * This structure coheres to the following Concepts:
 *  - Comparable (Strong order)
 *  - Stringizable
 */
class HandValue: CompareFromLessCRTP<HandValue> {
    int rank_value(const Card& card, const int ace_value) const {
        auto v = card.getRank();
        return v == Rank::ACE? ace_value: static_cast<int>(v);
    }
    int rank_value() const {
        return rank_value(highest_card, ace_value);
    }
public:
    HandType type;
    Card highest_card;
    int ace_value;

    bool operator<(const HandValue& other) const {
        if(static_cast<int>(type) == static_cast<int>(other.type)) {
            if(rank_value() == other.rank_value()) {
                return highest_card.getSuit() < other.highest_card.getSuit();
            }
            return rank_value() < other.rank_value();
        }
        return static_cast<int>(type) < static_cast<int>(other.type);
    }
    friend std::ostream& operator<<(std::ostream& os, const HandValue& hand) {
        return os << "(" << hand.type << ", Highest card: " << hand.highest_card << ")";
    }
};

class PokerHand: 
public CompareFromLessCRTP<PokerHand>, private std::vector<Card>  
{
private:
    using Collection = std::vector<Card>;
public:
    // using Collection::vector; // ctor
    using Collection::begin;
    using Collection::end;
    using Collection::cbegin;
    using Collection::cend;

    // constructor with va-args -> initializer list of vector
    template<typename... CardConstructs>
    PokerHand(CardConstructs&&... args): vector({std::forward<CardConstructs>(args)...}) {

    }

    // add cards -> std::vector::emplace_back.
    // This supports copy, move, or custom constructors
    template<typename... CardConstructs>
    PokerHand& addCard(CardConstructs&&... args) {
        emplace_back(std::forward<CardConstructs>(args)...);
        return *this;
    }

    auto getAllPossibleHands() const {
        std::vector<PokerHand> retval;
        constexpr int cards_per_hand = 5;
        assert(size() >= cards_per_hand && "Size less than cards per hand required");

        // recursive backtracking
        std::vector<int> backtrack;
        std::vector<Card> partial_hand;
        // initialize
        for(int i = 0; i < cards_per_hand; ++i) {
            backtrack.push_back(i);
            partial_hand.push_back((*this)[i]);
        }
        const int backtrack_pop_threshold = size();
        // TODO: simplify this loop; this is essentially unrolling recursive-backtracking.
        while(true) {
            // first, consume the hand
            retval.emplace_back(partial_hand); // copy
            std::sort(retval.back().begin(), retval.back().end()); // sort as if ace is 14.

            // then structure to the next iteration TODO: reformatable

            // pop if needed, if no more hand reachable, end
            bool end = false;
            int threshold = backtrack_pop_threshold - cards_per_hand + backtrack.size();
            ++backtrack.back();
            while(backtrack.back() == threshold) {
                partial_hand.pop_back();
                backtrack.pop_back();
                --threshold; // maintain correct threshold because --backtrack.size()
                if(backtrack.size() == 0) {
                    end = true;
                    break;
                }
                ++backtrack.back();
            }
            if(end == true) {break;}
            // follow partial_hand up with ++backtrack.back()
            partial_hand.back() = (*this)[backtrack.back()];

            // repopulate if we have popped many times
            for(int i = backtrack.size(); i < cards_per_hand; ++i) {
                int v = backtrack.back() + 1;
                backtrack.push_back(v);
                partial_hand.push_back((*this)[v]);
            }
        }

        return retval;
    }

private:    
    Optional<Suit> flushing_suit() const {
        static constexpr int n_suits = 4; // there are 4 suits in a deque.
        std::array<int, n_suits> suit_count = {0};
        /// populate suit count
        for(auto itr = cbegin(); itr != cend(); ++itr) {
            suit_count[static_cast<int>(itr->getSuit())] += 1;
        }
        /// determine if there is a flushing suit (suit with count >= 5)
        auto flush_suit_itr = std::find_if(suit_count.begin(), suit_count.end(), [](const auto& suit) {
            return suit >= 5;
        });

        if(flush_suit_itr == suit_count.end()) {
            return {}; // no suit
        }
        return make_optional<Suit>(static_cast<Suit>(flush_suit_itr - suit_count.begin()));
    }
    HandValue _get_hand_value() const {
        assert(size() == 5 && "May only get hand value 5-card at a time");
        // flush
        auto flush_suit = flushing_suit();
        if(flush_suit.has_value()) {
            // determine if also straight (requires sorted)
            // determine ace's value
        }

    }
    HandValue get_hand_value() const {
        auto all_possibles = getAllPossibleHands();
        assert(all_possibles.size() >= 1 && "No possibilities for given hand");
        HandValue max_value = all_possibles[0]._get_hand_value();
        for(int i = 1; i < all_possibles.size(); ++i) {
            HandValue next_value = all_possibles[i]._get_hand_value();
            max_value = max_value < next_value? next_value: max_value;
        }
        return max_value;
    }
public:

    bool operator<(const PokerHand& other) const {
        // rankings:
        // Straight Flush (5): same suit, 5 in sequence
        // 4 of a kind: 4 aces, 4 threes
        // full house: three-of-a-kind + pair
        // flush: 5 cards, same suit, not in sequence
        // three-of-a-kind
        // two pairs
        // 1 pair
        // no pair

        return get_hand_value() < other.get_hand_value();
    }

    std::string toString() const {
        std::string retval {"hand("};
        for(auto card = cbegin(); card != cend(); ++card) {
            if(card != cbegin()) {
                retval += ' ';
            }
            retval += card->toString();
        }
        retval+=")";
        return retval;
    }

    friend std::ostream& operator<<(std::ostream& os, const PokerHand& card) {
        os << card.toString();
        return os;
    }
};




#endif //SCOPEDENUM_POKERHAND_HPP
