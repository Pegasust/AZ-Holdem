#pragma once
#ifndef POKERHAND_ALGO_HPP
#define POKERHAND_ALGO_HPP

#include <algorithm>
#include <cassert>

#include "Card.hpp"
#include "metaprogramming.hpp"
#include "HandType.hpp"

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

  HandValue() = default;
  HandValue(HandType type, int tiebreaker_value) : 
  type(type), tiebreaker_value(tiebreaker_value){}

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
        << hand.tiebreaker_value << ")";
  }
  static HandValue from_card(const Card& card) {
    return {HandType::HIGH, card.getValue()};
  }
// from_hand re-model
static HandValue from_hand(const std::vector<Card>& cards) {
    auto flush = flush_hand_value(cards);
    auto straight = straight_hand_value(cards);

    if(flush.has_value()) {
        if(!straight.has_value()) {
            return flush.value();
        }
        return {HandType::STRAIGHT_FLUSH, straight.value().tiebreaker_value};
    }

    if(straight.has_value()) {
        return straight.value();
    }

    std::vector<HandValue> hand_values;
    int count = 1;
    int value = cards.front().getValue();
    for(int i = 1; i < cards.size(); ++i) {
        if(cards[i].getValue() != value) {
            auto rank = type_from_common_rank(count);
            hand_values.emplace_back(rank, value);
            count = 0;
        }
        ++count;
        value = cards[i].getValue();
    }
    auto rank = type_from_common_rank(count);
    hand_values.emplace_back(rank, value);
    std::sort(hand_values.begin(), hand_values.end());
    int tiebreaker_v = tiebreaker(hand_values.begin(), hand_values.end(), [](const HandValue& hv) {
        return hv.tiebreaker_value;
    });
    int second = hand_values.size() <= 1? 0: common_rank_from_type(hand_values[hand_values.size()-2].type);
    HandType responding_type = type_from_common_rank(
      common_rank_from_type(hand_values.back().type), second
    );
    return {responding_type, tiebreaker_v};
}
private:
static Optional<HandValue> flush_hand_value(const std::vector<Card>& cards) {
    auto itr = std::adjacent_find(cards.cbegin(), cards.cend(), [](const auto& lhs, const auto& rhs) {
        return lhs.getSuit() != rhs.getSuit();
    });
    if(itr != cards.cend()) {
        // not flush
        return {};
    }
    return make_optional<HandValue>(HandType::FLUSH, 
    tiebreaker(cards.cbegin(), cards.cend(), [](const Card& card) {
        return card.getValue();
    }));
}
static Optional<HandValue> straight_hand_value(const std::vector<Card>& cards) {
    for (int i = 1; i < 4; ++i)
    {
      if (cards[i - 1].getValue() != cards[i].getValue() - 1)
      {
        // not straight
        return {};
      }
    }
    bool low_ace = (cards.front().getRank() == Rank::DEUCE && cards.back().getRank() == Rank::ACE);
    if(!low_ace && cards[cards.size()-2].getValue() != cards.back().getValue() -1) {
        // last card doesn't go on the sequence
        return {};
    }
    auto highest_card = low_ace? cards[cards.size()-2]: cards.back();
    return make_optional<HandValue>(HandType::STRAIGHT, highest_card.getValue());
}
  template<typename ConstInputIterator, typename UnaryOperator>
  static int tiebreaker(ConstInputIterator begin, ConstInputIterator end, UnaryOperator unary) {
    int val = 0;
    int shift = 0;
    constexpr int shifter = 4;
    // we will shift by 4 to make multiplier to be 16 (closest to 15 and fastest)
    for(auto itr = begin; itr != end; ++itr) {
      val += unary(*itr) << shift;
      shift += shifter;
    }
    return val;
  }
};

#endif