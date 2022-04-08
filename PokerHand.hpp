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
  Optional<Collection> best_hand;
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
  hand_value(),
  best_hand()
  {
      if(size() == 5) {
          std::sort(begin(), end());  // sort as if ace is 14.
          hand_value = make_optional(HandValue::from_hand(*this));
          best_hand = make_optional<Collection>(static_cast<const Collection&>(*this));
      }
  }

  // add cards -> std::vector::emplace_back.
  // This supports copy, move, or custom constructors
  template<typename... CardConstructs>
  PokerHand& addCard(CardConstructs&&... args)
  {
    emplace_back(std::forward<CardConstructs>(args)...);
    if(size() < 5) {return *this;}
    auto all_possibles = getAllPossibleHands();
    HandValue max_value = HandValue::from_hand(all_possibles[0]);
    int max_idx = 0;
    for (int i = 1; i < all_possibles.size(); ++i)
    {
      HandValue next_value = HandValue::from_hand(all_possibles[i]);
      if(max_value < next_value) {
        max_value = next_value;
        max_idx = i;
      }
    }
    hand_value = make_optional(std::move(max_value));
    this->best_hand = make_optional<Collection>(std::move(static_cast<Collection>(all_possibles[max_idx])));
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
    backtrack.push_back(-1);
    const int backtrack_pop_threshold = size();
    while(backtrack.size() > 0) {
        int threshold = backtrack_pop_threshold - cards_per_hand + backtrack.size();
        ++backtrack.back();
        if(backtrack.back() == threshold) {
            if(partial_hand.size() > 0) {partial_hand.pop_back();}
            backtrack.pop_back();
            continue;
        }
        if(partial_hand.size() > 0) {partial_hand.pop_back();}
        partial_hand.push_back((*this)[backtrack.back()]);
        // add until it's sufficient to be 5
        for(int i = backtrack.size(); i < cards_per_hand; ++i) {
            int v = backtrack.back() + 1;
            backtrack.push_back(v);
            partial_hand.push_back((*this)[v]);
        }        
        // partial_hand ready to be consumed!
        retval.emplace_back(partial_hand);
    }
    return retval;
}

 public:
  const HandValue& get_hand_value() const
  {
      return hand_value.value();
  }

  const auto& get_best_hand() const {
    return best_hand.value();
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
    os << "\n    Best hand: [";
    if(cards.best_hand.has_value()) {
      auto &best_hand = cards.best_hand.value();
      for(auto card = best_hand.cbegin(); card != best_hand.cend(); ++card) {
        if(card != best_hand.cbegin()) {
          os << ' ';
        } 
        os << *card;
      }
    } else {
      os << "<Unknown>";
    }
    os << "]";
    os << "     " << cards.hand_value;
    os << ")";
    return os;
  }
};

#endif  // SCOPEDENUM_POKERHAND_HPP
