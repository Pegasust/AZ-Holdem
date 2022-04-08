#pragma once

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

int common_rank_from_type(HandType type) {
  switch(type) {
    case HandType::PAIR:
      return 2;
    case HandType::HIGH:
      return 1;
    case HandType::SAME_3:
      return 3;
    case HandType::SAME_4:
      return 4;
    default:
      assert(false && "type is not PAIR, HIGH, SAME_3, or SAME_4");
  }
  assert(false && "Unreachable");
}

HandType type_from_common_rank(int most_common_rank, int second_common_rank=0) {
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
    "Full House",
    "Four-of-a-kind",
    "Straight Flush",
  };
  auto idx = static_cast<int>(type);
  return os << hand_type_str[idx];
}

