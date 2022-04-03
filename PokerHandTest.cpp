// Unit test for PokerHand and its overloaded < operator
#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include "52Cards.hpp"
#include "PokerHand.hpp"


TEST_CASE("testHighCard1") {
  PokerHand a(C3, C4, D6, D7, DA);
  PokerHand b(C2, C5, C7, DQ, DK);
  REQUIRE(b < a);
  REQUIRE(not(a < b));
}

TEST_CASE("testHighCard2") {
  PokerHand a(C3, C4, D6, D7, DQ);
  PokerHand b(C2, C5, C7, DJ, DK);
  REQUIRE(a < b);
  REQUIRE(not(b < a));
}

TEST_CASE("testHighCard3") {
  PokerHand a(C3, C4, C6, CJ, DK);
  PokerHand b(D2, D5, D7, DJ, SK);
  REQUIRE(a < b);
  REQUIRE(not(b < a));
}

TEST_CASE("testHighCard4") {
  PokerHand a(C3, C4, C7, CJ, DK);
  PokerHand b(D2, D5, D7, DJ, SK);
  REQUIRE(a < b);
  REQUIRE(not(b < a));
}

TEST_CASE("testHighCard5") {
  PokerHand a(C3, C5, C7, CJ, DK);
  PokerHand b(D2, D5, D7, DJ, SK);
  REQUIRE(b < a);
  REQUIRE(not(a < b));
}


// TEST HGH HANDS
PokerHand nothing72(C2, C3, C4, C5, D7); /* NOLINT */
PokerHand nothing73(D2, D4, D5, D6, C7);/* NOLINT */
PokerHand nothingJ(C8, C9, C10, SJ, D3);/* NOLINT */
PokerHand nothingK9(CK, CQ, CJ, D8, H9);/* NOLINT */
PokerHand nothingK8(HK, HQ, HJ, H10, S8);/* NOLINT */
PokerHand nothingA(S9, SJ, SQ, SK, CA);/* NOLINT */

TEST_CASE("testNothing0") {
  REQUIRE(nothing72 < nothing73);
  REQUIRE(not(nothing73 < nothing72));
}

TEST_CASE("testNothing1") {
  REQUIRE(nothing73 < nothingJ);
  REQUIRE(not(nothingJ < nothing73));
}

TEST_CASE("testNothing2") {
  REQUIRE(nothingJ < nothingK8);
  REQUIRE(not(nothingK8 < nothingJ));
}

TEST_CASE("testNothing3") {
  REQUIRE(nothingK9 < nothingK8);
  REQUIRE(not(nothingK8 < nothingK9));
}

TEST_CASE("testNothing4") {
  REQUIRE(nothingK8 < nothingA);
  REQUIRE(not(nothingA < nothingK8));
}

// TEST PAIRS PokerHand pair(D2, H2, D3, S7, H6);
TEST_CASE("testPair1") {
  PokerHand a(C3, C4, C7, CJ, DJ);
  PokerHand b(D2, D5, D7, D10, S10);
  REQUIRE(b < a);
  REQUIRE(not(a < b));
}

TEST_CASE("testPairWhenTied") {
  PokerHand a(H3, CA, D4, H6, SA);
  PokerHand b(D3, C4, DA, HA, C6);
  REQUIRE(not(a < b));
  REQUIRE(not(b < a));
}

PokerHand pair2(C2, S2, CQ, CK, CA); /* NOLINT */
PokerHand pair3(C3, H3, H4, H5, S6);/* NOLINT */
PokerHand pairLow(C2, D2, C3, H4, H5);/* NOLINT */
PokerHand pairHigh(HA, CA, CK, CQ, HJ);/* NOLINT */

TEST_CASE("testPair4") {
  PokerHand pair2(C2, S2, CQ, CK, CA); /* NOLINT */
  PokerHand pair3(C3, H3, H4, H5, S6);/* NOLINT */
  REQUIRE(pair2 < pair3);
  REQUIRE(not(pair3 < pair2));
}

TEST_CASE("testPair5") {
  REQUIRE(pairLow < pairHigh);
  REQUIRE(not(pairHigh < pairLow));
}

TEST_CASE("testPair7") {
  REQUIRE(nothingJ < pairHigh);
  REQUIRE(not(pairHigh < nothingJ));
}

TEST_CASE("testPair3") {
  PokerHand a(CA, CK, CQ, C5, D5);
  PokerHand b(DQ, DK, DA, D6, S6);
  REQUIRE(a < b);
  REQUIRE(not(b < a));
}

//TEST_CASE("testPair13") {
//  PokerHand a(CA, CK, CQ, C5, D5);
//  PokerHand b(DQ, DK, DA, D4, S4);
//  REQUIRE(b < a);
//  REQUIRE(not(a < b));
//}


// Two Pair
TEST_CASE("testTwoPair3") {
  PokerHand a(C4, CA, D4, H3, DA);
  PokerHand b(H4, C10, HA, SA, S4);
  REQUIRE(a < b);
  REQUIRE(not(b < a));
}

TEST_CASE("testTwoPairWhenTwoPairsAreEqual4") {
  PokerHand a(C2, D2, C5, D5, CJ);
  PokerHand b(H2, S2, H5, S5, C10);
  REQUIRE(b < a);
  REQUIRE(not(a < b));
}

TEST_CASE("testTwoPairWhenOnePairIsEqual") {
  PokerHand a(C4, HK, D4, H3, DK);
  PokerHand b(H4, C10, CA, DA, S4);
  REQUIRE(a < b);
  REQUIRE(not(b < a));
}

TEST_CASE("testTwoPairWhenOnePairIsEqual2") {
  PokerHand a(C8, D8, CA, SA, CK);
  PokerHand b(H9, S9, HA, DA, HK); // 2 ACES, 2 NINES
  REQUIRE(a < b);
  REQUIRE(not(b < a));
}

TEST_CASE("testTwoPair2") {
  PokerHand a(C4, CA, D4, H3, DA);
  PokerHand b(H4, C10, HA, SA, S4);
  REQUIRE(a < b);
  REQUIRE(not(b < a));
}

TEST_CASE("testTwoPair6") {
  PokerHand a(C2, C5, D2, D5, CJ);
  PokerHand b(H5, S5, H2, S2, C10);
  REQUIRE(b < a);
  REQUIRE(not(a < b));
}

// Test two pairs
PokerHand twoPair(D2, H4, C3, C2, H3);/* NOLINT */
PokerHand twoPair2(H9, D9, S2, H2, SJ);/* NOLINT */
PokerHand twoPairA(DA, CA, DK, CK, H7);/* NOLINT */
PokerHand twoPairLow(C2, D2, C3, H3, D4);/* NOLINT */
PokerHand twoPairHigh(CA, HA, CK, HK, CQ);/* NOLINT */

TEST_CASE("testTwoPairs0") {
  REQUIRE(twoPair < twoPair2);
  REQUIRE(not(twoPair2 < twoPair));
}

TEST_CASE("testTwoPairs1") {
  REQUIRE(twoPair2 < twoPairA);
  REQUIRE(not( twoPairA < twoPair2 ));
}

TEST_CASE("testTwoPairs2") {
  REQUIRE(twoPairLow < twoPairA);
  REQUIRE(not(twoPairA < twoPairLow ));
}

TEST_CASE("testTwoPairs") {
  REQUIRE(twoPairLow < twoPairHigh);
  REQUIRE(not(twoPairHigh < twoPairLow ));
}

TEST_CASE("testPairWhenOnePairIsEqual3") {
  PokerHand a(C9, D9, CK, DA, C2);
  PokerHand b(H9, S9, HK, SA, C3);
  REQUIRE(a < b);
  REQUIRE(not(b < a));
}


// Trips
TEST_CASE("testTrips1") {
  PokerHand a(H7, C7, D7, S9, S10);
  PokerHand b(H9, C10, H6, C6, D6);
  REQUIRE(b < a);
  REQUIRE(not(a < b));
}

TEST_CASE("testTrips2") {
  PokerHand a(CK, DK, HK, CQ, CJ);
  PokerHand b(CA, DA, HA, H2, D2);
  REQUIRE(a < b);
  REQUIRE(not(b < a));
}

TEST_CASE("testTripsBeatsTwoPair") {
  PokerHand a(C2, H2, S2, CQ, CJ);
  PokerHand b(CA, DA, HK, SK, D2);
  REQUIRE(b < a);
  REQUIRE(not(a < b));
}


// TEST 3 OF A KIND
PokerHand threeKind(D2, H5, H2, H4, C2); /* NOLINT */
PokerHand threeKind2(C5, D5, S5, HA, HK);/* NOLINT */
PokerHand threeKind3(H6, S6, C6, H3, S2);/* NOLINT */
PokerHand threeKindLow(D2, C2, H2, H3, H4);/* NOLINT */
PokerHand threeKindHigh(CA, HA, SA, CK, CQ);/* NOLINT */

TEST_CASE("testThreeMiddle3AreTheSameForCodeCoverage") {
  PokerHand threeKind(D2, S5, D5, H5, CA);
  PokerHand threeKind2(C2, S4, D4, H4, SA);
  REQUIRE(threeKind2 < threeKind);
  REQUIRE(not(threeKind < threeKind2 ));
}

TEST_CASE("testThreeOfAKind1") {
  REQUIRE(threeKind < threeKind2);
  REQUIRE(not(threeKind2 < threeKind));
}

TEST_CASE("testThreeOfAKind2") {
  REQUIRE(threeKind2 < threeKind3);
  REQUIRE(not( threeKind3 < threeKind2 ));
}

TEST_CASE("testThreeOfAHigh3") {
  REQUIRE(threeKindLow < threeKindHigh);
  REQUIRE(not(threeKindHigh < threeKindLow ));
}

TEST_CASE("testThreeOfAHigh4") {
  REQUIRE(threeKind3 < threeKindHigh);
  REQUIRE(not(threeKindHigh < threeKind3));
}

TEST_CASE("testThreeOfAHigh5") {
  REQUIRE(threeKind < threeKindHigh);
  REQUIRE(not(threeKindHigh < threeKind ));
}

// Full house
TEST_CASE("testFullHouse1") {
  PokerHand a(H8, C8, D8, H4, C4);
  PokerHand b(H7, C7, D7, S4, D4);
  REQUIRE(b < a);
  REQUIRE(not (a < b));
}

TEST_CASE("testFullHouse2") {
  PokerHand a(H3, C3, D3, C4, D4);
  PokerHand b(H5, C5, D5, H4, S4);
  REQUIRE(a < b);
}

TEST_CASE("testFullHouse8") {
  PokerHand a(H8, C8, D8, H4, C4);
  PokerHand b(H7, C7, D7, S4, D4);
  REQUIRE(b < a);
  REQUIRE(not(a < b));
}

TEST_CASE("testFullHouseHands1") {
  PokerHand a(S6, D6, DA, CA, HA);
  PokerHand b(S8, D8, DK, CK, HK);
  REQUIRE(b < a);
  REQUIRE(not(a < b));
}

TEST_CASE("testFullHouseWhenPairsAreEqual") {
  PokerHand a(C2, D2, C3, D3, H3);
  PokerHand b(H2, S2, C4, D4, H4);
  REQUIRE(a < b);
  REQUIRE(not(b < a));
}

TEST_CASE("testFullHouseWhenPairIsGreaterThanTripsInOtherHandl") {
  PokerHand a(C8, D8, C3, D3, H3);
  PokerHand b(H7, S7, C4, D4, H4);
  REQUIRE(a < b);
  REQUIRE(not(b < a));
}

TEST_CASE("testAceLowStraight") {
  PokerHand a(CA, C2, S3, D4, H5); // Ace low straight
  PokerHand b(DK, HK, SK, D5, H6); // 3 of a kind
  REQUIRE(b < a);
  REQUIRE(not(a < b));
}

TEST_CASE("testFourOfKind2_3") {
  PokerHand a(CA, C2, D2, H2, S2);
  PokerHand b(DA, C3, D3, H3, S3);
  REQUIRE(a < b);
  REQUIRE(not(b < a));
}

// flush

TEST_CASE("compareFlushes") {
  PokerHand a(CA, C4, C6, C8, C9);
  PokerHand b(HA, H4, H6, H8, H9);
  REQUIRE(not (b < a));
  REQUIRE(not (a < b));
}

PokerHand straight(C4, C6, C2, C5, S3); /* NOLINT */
PokerHand straightInDifferentSuit(D2, D3, D4, D5, S6);/* NOLINT */
PokerHand straightHigh(C10, HJ, CQ, CK, CA);/* NOLINT */
PokerHand aceLow(S2, C3, D4, S5, DA);/* NOLINT */

TEST_CASE("testStaights1") {
  // These are ==, can't test with <
  REQUIRE(not(straight < straightInDifferentSuit));
}

TEST_CASE("testStaights2") {
  REQUIRE(straight < straightHigh);
  REQUIRE(not(straightHigh < straight));
}

TEST_CASE("testStaights4") {
  REQUIRE(aceLow < straight);
  REQUIRE(not(straight < aceLow));
}

TEST_CASE("testStaights6") {
  REQUIRE(aceLow < straightHigh);
  REQUIRE(not(straightHigh < aceLow ));
}

TEST_CASE("testStaights9") {
  REQUIRE(threeKindHigh < aceLow);
  REQUIRE(not(aceLow < threeKindHigh ));
}

// TEST FLUSHES
PokerHand flush(H7, H5, H3, H2, HJ); /* NOLINT */
PokerHand flush2(S7, S5, S4, S2, SJ);/* NOLINT */
PokerHand flushLow(D2, D3, D4, D5, D7);/* NOLINT */
PokerHand flushHigh(CA, CK, CQ, CJ, C9);/* NOLINT */

TEST_CASE("testFlush") {
  REQUIRE(flushLow < flush);
  REQUIRE(not(flush < flushLow));
}

TEST_CASE("testFlush2") {
  REQUIRE(flushLow < flushHigh);
  REQUIRE(not(flushHigh < flushLow));
}

TEST_CASE("testFlush3") {
  REQUIRE(flush < flush2);
  REQUIRE(not(flush2 < flush));
}

TEST_CASE("testFlush4") {
  REQUIRE(flush2 < flushHigh);
  REQUIRE(not(flushHigh < flush2));
}

// TEST FULL HOUSE
// The higher triple wins
PokerHand fullHouse(CA, DA, C3, D3, H3); /* NOLINT */
PokerHand fullHouse2(C2, D2, C4, D4, H4);/* NOLINT */
PokerHand fullHouse3(HK, DK, SK, CA, HA);/* NOLINT */
PokerHand fullHouseLow(C2, D2, H2, C3, H3);/* NOLINT */
PokerHand fullHouseHigh(CA, DA, HA, CK, HK);/* NOLINT */

TEST_CASE("testFullHouse3") {
  REQUIRE(fullHouse < fullHouse2);
  REQUIRE(not(fullHouse2 < fullHouse ));
}

TEST_CASE("testFullHouse4") {
  REQUIRE(fullHouse2 < fullHouse3);
  REQUIRE(not(fullHouse3 < fullHouse2));
}

TEST_CASE("testFullHouse5") {
  REQUIRE(fullHouse2 < fullHouse3);
  REQUIRE(not(fullHouse3 < fullHouse2 ));
}

TEST_CASE("testFullHouse6") {
  REQUIRE(fullHouseLow < fullHouseHigh);
  REQUIRE(not(fullHouseHigh < fullHouseLow ));
}

// TEST FOUR OF A KIND
PokerHand fourKindLow(C2, D2, H2, S2, C3);/* NOLINT */
PokerHand fourKindMid(H3, C8, D8, H8, S8);/* NOLINT */
PokerHand fourKindHigh(CA, DA, HA, SA, CK);/* NOLINT */

TEST_CASE("testFourOfAKind1") {
  REQUIRE(fourKindLow < fourKindMid);
  REQUIRE(not(fourKindMid < fourKindLow));
}

TEST_CASE("testFourOfAKind2") {
  REQUIRE(fourKindLow < fourKindHigh);
  REQUIRE(not(fourKindHigh < fourKindLow ));
}

TEST_CASE("testFourOfAKind3") {
  REQUIRE(fourKindMid < fourKindHigh);
  REQUIRE(not(fourKindHigh < fourKindMid));
}

// TEST FLUSH
PokerHand straightFlush(H3, H4, H5, H6, H7); /* NOLINT */
PokerHand straightFlushMid(H8, H9, H10, HJ, HQ);/* NOLINT */
PokerHand straightFlushHi(C10, CJ, CQ, CK, CA);/* NOLINT */

TEST_CASE("testStraightFlush1") {
  REQUIRE(straightFlush < straightFlushMid);
  REQUIRE(not(straightFlushMid < straightFlush ));
}

TEST_CASE("testStraightFlush2") {
  REQUIRE(straightFlushMid < straightFlushHi);
  REQUIRE(not(straightFlushHi < straightFlushMid));
}

TEST_CASE("testStraightFlush3") {
  REQUIRE(straightFlush < straightFlushHi);
  REQUIRE(not(straightFlushHi < straightFlush ));

}


/////////////////////////////////
// Test different ranks
////////////////////////////////

TEST_CASE("testManyRanks1") {
  REQUIRE(fourKindHigh < straightFlush);
  REQUIRE(not(straightFlush < fourKindHigh));

}

TEST_CASE("testManyRanks01") {
  REQUIRE(pair2 < straightFlush);
  REQUIRE(not(straightFlush < pair2 ));
}

TEST_CASE("testManyRanks02") {
  REQUIRE(twoPair2 < straightFlush);
  REQUIRE(not(straightFlush < twoPair2));
}

TEST_CASE("testManyRanks03") {
  REQUIRE(fullHouse3 < straightFlush);
  REQUIRE(not( straightFlush < fullHouse3 ));
}

TEST_CASE("testManyRanks04") {
  REQUIRE(straight < straightFlush);
  REQUIRE(not(straightFlush < straight ));
}

TEST_CASE("testManyRanks05") {
  REQUIRE(straightHigh < straightFlush);
  REQUIRE(not(straightFlush < straightHigh ));
}

TEST_CASE("testManyRanks2") {
  REQUIRE(fullHouse2 < fourKindHigh);
  REQUIRE(not(fourKindHigh < fullHouse2 ));
}

TEST_CASE("testManyRanks3") {
  REQUIRE(fullHouse2 < fourKindMid);
  REQUIRE(not(fourKindMid <fullHouse2));
}

TEST_CASE("testManyRanks13") {
  REQUIRE(threeKind2 < fourKindMid);
  REQUIRE(not( fourKindMid < threeKind2 ));
}

TEST_CASE("testManyRanks14") {
  REQUIRE(threeKindHigh < fourKindMid);
  REQUIRE(not( fourKindMid < threeKindHigh ));
}

TEST_CASE("testManyRanks4") {
  REQUIRE(threeKind2 < fullHouse2);
  REQUIRE(not(fullHouse2 < threeKind2 ));
}

TEST_CASE("testManyRanks5") {
  REQUIRE(threeKind3 < fullHouse2);
  REQUIRE(not(fullHouse2 < threeKind3));
}

TEST_CASE("testManyRanks6") {
  REQUIRE(threeKind3 < fullHouse2);
  REQUIRE(not(fullHouse2 < threeKind3 ));
}

TEST_CASE("testManyRanks7") {
  REQUIRE(twoPairHigh < threeKind3);
  REQUIRE(not(threeKind3 < twoPairHigh));
}

TEST_CASE("testManyRanks07") {
  REQUIRE(pairHigh < threeKind3);
  REQUIRE(not(threeKind3 < pairHigh ));
}

TEST_CASE("testManyRanks08") {
  REQUIRE(nothingA < threeKind3);
  REQUIRE(not(threeKind3 < nothingA));
}

TEST_CASE("testManyRanks9") {
  REQUIRE(pair3 < twoPairHigh);
  REQUIRE(not(twoPairHigh < pair3));
}

TEST_CASE("testManyRanks10") {
  REQUIRE(pairLow < twoPairHigh);
  REQUIRE(not(twoPairHigh < pairLow));
}

TEST_CASE("testManyRanks11") {
  REQUIRE(nothing73 < twoPairHigh);
  REQUIRE(not(twoPairHigh < nothing73));
}

TEST_CASE("testManyRanks12") {
  REQUIRE(nothing73 < pairHigh);
  REQUIRE(not(pairHigh < nothing73 ));
}

TEST_CASE("testTripsLosesToFourOfAKind") {
  PokerHand a(CA, HA, SA, CQ, CJ);
  PokerHand b(C2, D2, H2, S2, D8);
  REQUIRE(a < b);
  REQUIRE(not(b < a));
}

// Test straight flushes when the Ace is involved
PokerHand aceLowStaightFush(HA, H2, H3, H4, H5); /* NOLINT */
PokerHand aceHighStraightFlush(DA, DK, DQ, DJ, D10);/* NOLINT */
PokerHand aceLowStaightFushClubs(CA, C2, C3, C4, C5);/* NOLINT */
PokerHand aceHighStraightFlushSpades(SA, SK, SQ, SJ, S10);/* NOLINT */

TEST_CASE("testStraightFlushesLowAndHigh") {
  REQUIRE(aceLowStaightFush < aceHighStraightFlush);
  REQUIRE(not(aceHighStraightFlush < aceLowStaightFush ));
}

TEST_CASE("testStraightFlushesLowAndHigh2") {
  REQUIRE(aceLowStaightFushClubs < aceHighStraightFlushSpades);
  REQUIRE(not(aceHighStraightFlushSpades < aceLowStaightFushClubs ));
}

TEST_CASE("testStraightFlushesLowAndHigh3") {
  REQUIRE(aceLowStaightFush < aceHighStraightFlush);
  REQUIRE(not(aceHighStraightFlush < aceLowStaightFush));
}

TEST_CASE("testStraightFlushesLowAndHigh4") {
  REQUIRE(aceLowStaightFushClubs < aceHighStraightFlushSpades);
  REQUIRE(not(aceHighStraightFlushSpades < aceLowStaightFushClubs));
}

TEST_CASE("testStraightFlush00") {
  REQUIRE(fullHouse2 < straightFlushMid);
  REQUIRE(not(straightFlushMid < fullHouse2));
}

TEST_CASE("testManyRanks44") {
  REQUIRE(threeKindHigh < fullHouse2);
  REQUIRE(not(fullHouse2 < threeKindHigh));
}

TEST_CASE("testFinalShouldWorkSinceSoFarApart") {
  PokerHand a(CA, DK, CQ, DJ, H10);
  PokerHand b(H2, S3, C4, D5, H6);
  REQUIRE(b < a);
  REQUIRE(not(a < b));
}

TEST_CASE("testAceLowStraight4") {
  PokerHand a(D2, C3, S4, C5, SA);
  PokerHand b(C2, S3, H4, H5, HA);
  REQUIRE(not(a < b));
}

TEST_CASE("testFullHouseHands") {
  PokerHand a(S6, D6, DA, CA, HA);  // Both hands can have the same 3 Aces
  PokerHand b(S3, D3, DA, CA, HA);
  REQUIRE(b < a);
  REQUIRE(not(a < b));
}

TEST_CASE("testFullHouseHands2") {
  PokerHand a(S6, D6, DA, CA, HA);
  PokerHand b(C6, H6, DA, CA, HA);
  REQUIRE(not(a < b));
  REQUIRE(not(b < a));
}

TEST_CASE("testFullHouseHands3") {
  PokerHand a(S6, D6, DA, CA, HA);
  PokerHand b(C7, H7, DA, CA, HA);
  REQUIRE(a < b);
  REQUIRE(not(b < a));
}

TEST_CASE("testThreeOfAKind") {
  PokerHand a(S6, D7, DA, CA, HA);
  PokerHand b(C5, H6, DA, CA, HA);
  REQUIRE(b < a);
  REQUIRE(not(a < b));
}

TEST_CASE("testThreeOfAKind3") {
  PokerHand a(S7, D6, DA, CA, HA);
  PokerHand b(C7, H6, DA, CA, HA);
  // These are equal, but can't use ==
  REQUIRE(not(a < b));
  REQUIRE(not(b < a));
}

TEST_CASE("testFourOfAKindWhenEqual") {
  PokerHand a(S7, CA, DA, HA, SA);
  PokerHand b(D7, CA, DA, HA, SA);
  REQUIRE(not(a < b));
  REQUIRE(not(b < a));
}

TEST_CASE("testFourOfAKind4") {
  PokerHand a(S8, CA, DA, HA, SA);
  PokerHand b(D7, CA, DA, HA, SA);
  REQUIRE(b < a);
  REQUIRE(not(a < b));
}
