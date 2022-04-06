/*
 * File Name Card.cpp
 *
 * Description: class Card represents an object with a suit and rank
 * The toString member function given shows a graphical representation
 * of a card. When sorted, a vector<card> has all cards sorted using getValue()
 * that returns the rank as an integer from 2 (DEUCE) through 14 (ACE).
 *
 * Author Rick Mercer and Hung Tran
 *
 */

#include <iostream>
#include <string>
#include <vector>

#include "metaprogramming.hpp"
enum class Rank
{
  DEUCE = 2,
  THREE = 3,
  FOUR = 4,
  FIVE = 5,
  SIX = 6,
  SEVEN = 7,
  EIGHT = 8,
  NINE = 9,
  TEN = 10,
  JACK = 11,
  QUEEN = 12,
  KING = 13,
  ACE = 14
};

enum class Suit
{
  CLUBS = 3,
  DIAMONDS = 2,
  HEARTS = 1,
  SPADES = 0
};

class Card : public CompareFromLessCRTP<Card>, public ToStringCRTP<Card>
{
 private:
  Rank rank;
  Suit suit;

 public:
  Card(Rank rank, Suit suit) : rank(rank), suit(suit)
  {
  }

  Rank getRank() const
  {
    return rank;
  }
  Suit getSuit() const
  {
    return suit;
  }
  int getValue() const
  {
    return static_cast<int>(rank);
  }
  bool operator<(const Card& other) const
  {
    return getValue() < other.getValue() ||
           (getValue() == other.getValue() && static_cast<int>(suit) < static_cast<int>(other.suit));
  }
  
  friend std::ostream& operator<<(std::ostream& os, const Card& card)
  {
    std::vector<std::string> suits{ "\xe2\x99\xa0", "\xe2\x99\xa5", "\xe2\x99\xa6", "\xe2\x99\xa3" };
    std::vector<std::string> face{ "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A" };

    os << face.at(static_cast<int>(card.rank) - 2) << suits.at(static_cast<int>(card.suit));
    return os;
  }
};
