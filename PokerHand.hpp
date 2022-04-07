//
// Created by _nhitrl on 3/29/2022.
//

#ifndef SCOPEDENUM_POKERHAND_HPP
#define SCOPEDENUM_POKERHAND_HPP

#include <algorithm>
#include <array>
#include <cassert>
#include <optional>
#include <vector>

#include "Card.hpp"
#include "metaprogramming.hpp"
#include "PokerHandAlgo.hpp"


class PokerHand : 
public CompareFromLessCRTP<PokerHand>, public ToStringCRTP<PokerHand>, private std::vector<Card>
{
 private:
  using Collection = std::vector<Card>;
  Optional<HandValue> hand_value;  
  static constexpr int ace_value = static_cast<int>(Rank::ACE);

 public:
  // using Collection::vector; // ctor
  using Collection::begin;
  using Collection::cbegin;
  using Collection::cend;
  using Collection::end;

  // constructor with va-args -> initializer list of vector
  template<typename... CardConstructs>
  PokerHand(CardConstructs&&... args) : 
  vector({ std::forward<CardConstructs>(args)... }),
  hand_value()
  {
      if(size() == 5) {
          std::sort(begin(), end());  // sort as if ace is 14.
          hand_value = make_optional(HandValue::from_hand(*this));
      }
  }

  // add cards -> std::vector::emplace_back.
  // This supports copy, move, or custom constructors
  template<typename... CardConstructs>
  PokerHand& addCard(CardConstructs&&... args)
  {
    emplace_back(std::forward<CardConstructs>(args)...);
    auto all_possibles = getAllPossibleHands();
    assert(all_possibles.size() >= 1 && "No possibilities for given hand");
    HandValue max_value = HandValue::from_hand(all_possibles[0]);
    for (int i = 1; i < all_possibles.size(); ++i)
    {
      HandValue next_value = HandValue::from_hand(all_possibles[i]);
      max_value = max_value < next_value ? next_value : max_value;
    }
    hand_value = make_optional(max_value);
    return *this;
  }

  auto getAllPossibleHands() const
  {
    std::vector<PokerHand> retval;
    constexpr int cards_per_hand = 5;
    assert(size() >= cards_per_hand && "Size less than cards per hand required");

    // recursive backtracking
    std::vector<int> backtrack;
    std::vector<Card> partial_hand;
    // initialize
    for (int i = 0; i < cards_per_hand; ++i)
    {
      backtrack.push_back(i);
      partial_hand.push_back((*this)[i]);
    }
    const int backtrack_pop_threshold = size();
    // TODO: simplify this loop; this is essentially unrolling recursive-backtracking.
    while (true)
    {
      // first, consume the hand
      assert(partial_hand.size() == 5);
      retval.emplace_back(partial_hand);                      // copy
      // then structure to the next iteration TODO: reformatable

      // pop if needed, if no more hand reachable, end
      bool end = false;
      int threshold = backtrack_pop_threshold - cards_per_hand + backtrack.size();
      ++backtrack.back();
      while (backtrack.back() == threshold)
      {
        partial_hand.pop_back();
        backtrack.pop_back();
        --threshold;  // maintain correct threshold because --backtrack.size()
        if (backtrack.size() == 0)
        {
          end = true;
          break;
        }
        ++backtrack.back();
      }
      if (end == true)
      {
        break;
      }
      // follow partial_hand up with ++backtrack.back()
      partial_hand.back() = (*this)[backtrack.back()];

      // repopulate if we have popped many times
      for (int i = backtrack.size(); i < cards_per_hand; ++i)
      {
        int v = backtrack.back() + 1;
        backtrack.push_back(v);
        partial_hand.push_back((*this)[v]);
      }
    }

    return retval;
  }

 private:
  const HandValue& get_hand_value() const
  {
      return hand_value.value();
  }

 public:
  bool operator<(const PokerHand& other) const
  {
    return get_hand_value() < other.get_hand_value();
  }

  friend std::ostream& operator<<(std::ostream& os, const PokerHand& cards)
  {
    os << "PokerHand(";
    for (auto card = cards.cbegin(); card != cards.cend(); ++card)
    {
      if (card != cards.cbegin())
      {
        os << ' ';
      }
      os << *card;
    }
    os << ", value: " << cards.hand_value;
    os << ")";
    return os;
  }
};

#endif  // SCOPEDENUM_POKERHAND_HPP
