#pragma once
#include <iomanip>

#include "PokerHand.hpp"
#include "Card.hpp"
#include "metaprogramming.hpp"

class PokerPlayer: ToStringCRTP<PokerPlayer> {
private:
    std::vector<Card> hand;
    double balance_usd;
    Optional<PokerHand> evaluated_hand;

    void add_to_balance(double add) {
        balance_usd += add;
    }

public:
    template<typename... HandConstruct>
    explicit PokerPlayer(double balance_usd, HandConstruct&&... forward):
        balance_usd(balance_usd),
        hand(std::forward<HandConstruct>(forward)...)
    {}

    template<typename... CardConstruct>
    void add_to_hand(CardConstruct&& ...args) {
        hand.emplace_back(std::forward<CardConstruct>(args)...);
    }

    void add_from_community(const std::vector<Card> & cards) {
        evaluated_hand = make_optional<PokerHand>(cards);
        #pragma unroll 4
        for(auto & card : hand) {
            evaluated_hand.value().addCard(card);
        }
    }

    double collect_money(double wanted) {
        add_to_balance(-wanted);
        return wanted;
    }

    double earn_money(double earning) {
        add_to_balance(earning);
        return 0;
    }

    void return_cards() {
        hand.clear();
        evaluated_hand = {};
    }

    const PokerHand& get_hand() const {
        return evaluated_hand.value();
    }

    const HandValue& hand_value() const {
        return evaluated_hand.value().get_hand_value();
    }

    const auto& get_cards() const {return this->hand;}

    const auto& get_balance() const {return this->balance_usd;}

    friend std::ostream& operator<<(std::ostream& ostr, const PokerPlayer& player) {
        ostr << "$" << std::fixed << std::setprecision(2) << player.balance_usd;
        ostr << " - ";
        #pragma unroll 4
        for(int i = 0; i < player.hand.size(); ++i) {
            if(i != 0) {ostr << " ";}
            ostr << player.hand[i];
        }
        ostr << "\n";
        ostr << "  Evaluated hand: " << player.evaluated_hand;

        return ostr;
    }
};