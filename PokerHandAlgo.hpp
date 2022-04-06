#pragma once
#ifndef POKERHAND_ALGO_HPP
#define POKERHAND_ALGO_HPP

#include <algorithm>
#include <cassert>

#include "Card.hpp"
#include "metaprogramming.hpp"


enum class HandType : int
{
  STRAIGHT_FLUSH = 8,
  SAME_4 = 7,
  FULL_HOUSE = 6,
  FLUSH = 5,
  STRAIGHT = 4,
  SAME_3 = 3,
  DOUBLE_PAIR = 2,
  PAIR = 1,
  HIGH = 0
};

HandType type_from_common_rank(int most_common_rank, int second_common_rank) {
    if(most_common_rank == 4) {
        return HandType::SAME_4;
    }
    if(most_common_rank == 3) {
        return second_common_rank == 2? HandType::FULL_HOUSE: HandType::SAME_3;
    }
    if(most_common_rank == 2) {
        return second_common_rank == 2? HandType::DOUBLE_PAIR: HandType::PAIR;
    }
    return HandType::HIGH;
}

std::ostream& operator<<(std::ostream& os, const HandType& type)
{
  static const char* hand_type_str[] = {
    "High Card",
    "Pair",
    "Double pairs",
    "Three-of-a-kind",
    "Straight", 
    "Flush",
    "Fullhouse",
    "Four-of-a-kind",
    "Straight Flush",
  };
  auto idx = static_cast<int>(type);
  return os << "["<<idx<<"]" << hand_type_str[idx];
}

/**
 * @brief Simple data-type for value of a given hand.
 * This structure coheres to the following Concepts:
 *  - Comparable (Strong order)
 *  - Stringizable
 */
class HandValue : 
public CompareFromLessCRTP<HandValue>, public ToStringCRTP<HandValue>
{
 public:
  HandType type;
  int tiebreaker_value;
  bool low_ace;

  HandValue() = default;
  HandValue(HandType type, int tiebreaker_value, bool low_ace) : 
  type(type), tiebreaker_value(tiebreaker_value), low_ace(low_ace){}

  bool operator<(const HandValue& other) const
  {
    if (static_cast<int>(type) == static_cast<int>(other.type))
    {
      return tiebreaker_value < other.tiebreaker_value;
    }
    return static_cast<int>(type) < static_cast<int>(other.type);
  }
  friend std::ostream& operator<<(std::ostream& os, const HandValue& hand)
  {
    return os << "HandValue(" << hand.type << ", tiebreak_value: " 
        << hand.tiebreaker_value << ", low ace: " << hand.low_ace << ")";
  }
  static HandValue from_hand(const std::vector<Card>& cards) {
      assert(cards.size() == 5 && "Algorithm only evaluates 5 cards at a time.");
    // flush
    auto flush_suit = flushing_suit(cards);
    if (flush_suit.has_value())
    {
      return _flush_hand_value(cards);
    }
    // check for pairs (all is sorted)
    
    int most_common_rank = 1;
    int second_common_rank = 1;

    int common_rank_count = 1;
    auto common_rank = cards.front().getRank();
    for(int i = 1; i <= cards.size(); ++i) { // the end is also a cue for update
        if(i == cards.size() || cards[i].getRank() != common_rank) {
            // evaluate current common_rank_count.
            if(common_rank_count > most_common_rank) {
                second_common_rank = most_common_rank;
                most_common_rank = common_rank_count;
            } else if(common_rank_count > second_common_rank) {
                second_common_rank = common_rank_count;
            }
            common_rank_count = 1;
        }
        common_rank = cards[i].getRank();
    }

    HandType type = type_from_common_rank(most_common_rank, second_common_rank);
    auto card = cards.back();
    int tiebreak_value = card.getValue() * 4 + static_cast<int>(card.getRank()); // TODO: determine this tiebreak_value
    return {type, tiebreak_value, false};      
  }
private:

  static Optional<Suit> flushing_suit(const std::vector<Card>& cards)
  {
    static constexpr int n_suits = 4;  // there are 4 suits in a deque.
    std::array<int, n_suits> suit_count = { 0 };
    /// populate suit count
    for (auto itr = cards.cbegin(); itr != cards.cend(); ++itr)
    {
      suit_count[static_cast<int>(itr->getSuit())] += 1;
    }
    /// determine if there is a flushing suit (suit with count >= 5)
    auto flush_suit_itr = std::find_if(suit_count.begin(), suit_count.end(), [](const auto& suit) { return suit >= 5; });

    if (flush_suit_itr == suit_count.end()){ return {}; /*no suit*/}
    return make_optional<Suit>(static_cast<Suit>(flush_suit_itr - suit_count.begin()));
  }
  static HandValue _flush_hand_value(const std::vector<Card>& cards)
  {
    // determine if also straight (requires sorted)
    // determine ace's value

    // The only way ace takes on value of 1 is when there is
    // straight flush containing 2-3-4-5

    // check that at least the first four is consecutive
    for (int i = 1; i < 4; ++i)
    {
      if (cards[i - 1].getValue() != cards[i].getValue() - 1)
      {
        // not straight
        int tiebreak_value; // TODO: determine tiebreak_value
        return { HandType::FLUSH, tiebreak_value, false };
      }
    }
    // The algorithm now ends in 2 ways:

    // 1. there is an ace at the end AND the sequence is strictly 2-3-4-5
    if(cards.front().getRank() == Rank::DEUCE && cards.back().getRank() == Rank::ACE) {
        // ace is 1
        auto card = cards[cards.size()-2];
        int tiebreak_value = card.getValue() * 4 + static_cast<int>(card.getSuit());
        return { HandType::STRAIGHT_FLUSH, tiebreak_value, true};
    }
    // 2. normal case, the sequence ends with the highest card.
    auto card = cards.back();
    int tiebreak_value = card.getValue() * 4 + static_cast<int>(card.getSuit());
    return { HandType::STRAIGHT_FLUSH, tiebreak_value, false };
  }
};

#endif