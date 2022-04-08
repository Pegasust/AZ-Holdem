//
// Created by Richard H. Mercer on 3/24/22.
// This is a unit test that provides the specification of class Card
// using two new scoped enums Sit and Rank.  A vector<Card> must be sortable.

// Filename: Card.cpp
//
#define CATCH_CONFIG_MAIN
#include <catch.hpp>

// #include <string>
#include <iostream>
#include <vector>     // std::vector
#include <algorithm>  // std::sort
#include "Card.hpp"

TEST_CASE ("testGetters") {
  Card c2{Rank::DEUCE, Suit::CLUBS};
  REQUIRE(Rank::DEUCE == c2.getRank());
  REQUIRE(Suit::CLUBS == c2.getSuit());
  REQUIRE(2 == c2.getValue());

  Card d2{Rank::DEUCE, Suit::DIAMONDS};
  REQUIRE(Rank::DEUCE == d2.getRank());
  REQUIRE(Suit::DIAMONDS == d2.getSuit());
  REQUIRE(2 == c2.getValue());

  Card c3{Rank::THREE, Suit::CLUBS};
  REQUIRE(Rank::THREE == c3.getRank());
  REQUIRE(Suit::CLUBS == c3.getSuit());
  REQUIRE(3 == c3.getValue());

  Card d3{Rank::THREE, Suit::DIAMONDS};
  REQUIRE(Rank::THREE == d3.getRank());
  REQUIRE(Suit::DIAMONDS == d3.getSuit());
  REQUIRE(3 == d3.getValue());

  Card hk{Rank::KING, Suit::HEARTS};
  REQUIRE(Rank::KING == hk.getRank());
  REQUIRE(Suit::HEARTS == hk.getSuit());
  REQUIRE(13 == hk.getValue());

  Card sa{Rank::ACE, Suit::SPADES};
  REQUIRE(Rank::ACE == sa.getRank());
  REQUIRE(Suit::SPADES == sa.getSuit());
  REQUIRE(14 == sa.getValue());
}

TEST_CASE ("testCardWithAllValueToString") {
  Card C2{Rank::DEUCE, Suit::CLUBS};
  REQUIRE(C2.toString() == "2\xe2\x99\xa3");

  Card D9{Rank::NINE, Suit::DIAMONDS};
  REQUIRE(D9.toString() == "9\xe2\x99\xa6");

  Card H10{Rank::TEN, Suit::HEARTS};
  REQUIRE(H10.toString() == "10\xe2\x99\xa5");

  Card SJ{Rank::JACK, Suit::SPADES};
  REQUIRE(SJ.toString() == "J\xe2\x99\xa0");
  // std::cout << SJ.toString() << std::endl;
}

TEST_CASE("GetalueWitRelationalOps") {
  Card H10{Rank::TEN, Suit::HEARTS};
  Card HJ{Rank::JACK, Suit::HEARTS};
  Card HQ{Rank::QUEEN, Suit::HEARTS};
  Card HK{Rank::KING, Suit::HEARTS};
  Card HA{Rank::ACE, Suit::HEARTS};

  Card S10{Rank::TEN, Suit::SPADES};
  Card SJ{Rank::JACK, Suit::SPADES};
  Card SQ{Rank::QUEEN, Suit::SPADES};
  Card SK{Rank::KING, Suit::SPADES};
  Card SA{Rank::ACE, Suit::SPADES};

  REQUIRE(H10.getValue() == S10.getValue());
  REQUIRE(HJ.getValue() == SJ.getValue());
  REQUIRE(HQ.getValue() == SQ.getValue());
  REQUIRE(HK.getValue() == SK.getValue());
  REQUIRE(HA.getValue() == SA.getValue());

  REQUIRE(HA.getValue() > SK.getValue());
  REQUIRE(H10.getValue() < SJ.getValue());
  REQUIRE(SQ.getValue() > S10.getValue());
}


// This is used is the next TEST_CASE
bool isSorted(std::vector<Card> cards) {
  for (int index = 1; index < cards.size(); index++) {
    if (cards.at(index - 1).getValue() > cards.at(index).getValue())
      return false;
  }
  return true;
}

TEST_CASE("Sortable?") {
  Card C2{Rank::DEUCE, Suit::CLUBS};
  Card C3{Rank::THREE, Suit::CLUBS};
  Card D3{Rank::THREE, Suit::DIAMONDS};
  Card H8{Rank::EIGHT, Suit::HEARTS};
  Card S8{Rank::EIGHT, Suit::SPADES};
  Card H10{Rank::TEN, Suit::HEARTS};
  Card HQ{Rank::QUEEN, Suit::HEARTS};
  Card HA{Rank::ACE, Suit::HEARTS};
  Card SJ{Rank::JACK, Suit::SPADES};
  Card SK{Rank::KING, Suit::SPADES};

  // Card objects are move, not copied in C++20
  std::vector<Card> cards{HQ, SK, SJ, C2, C3, SJ, H10, H8, D3, S8};
  REQUIRE(not isSorted(cards));
  // class Card must overload the < operator
  std::sort(cards.begin(), cards.end());
  REQUIRE(isSorted(cards));
}

TEST_CASE("Comparison operators on Card") {
    Card C2{Rank::DEUCE, Suit::CLUBS};
    Card C3{Rank::THREE, Suit::CLUBS};
    REQUIRE(C2 < C3);
    REQUIRE(C3 > C2);
    REQUIRE(C2 != C3);
    REQUIRE(!(C2 == C3));

    Card eq0{Rank::DEUCE, Suit::CLUBS};
    Card eq1{Rank::DEUCE, Suit::CLUBS};
    REQUIRE(!(eq0 < eq1));
    REQUIRE(!(eq0 > eq1));
    REQUIRE(!(eq0 != eq1));
    REQUIRE(eq0 == eq1);
}