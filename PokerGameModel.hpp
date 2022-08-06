#pragma once

#ifndef POKER_GAME_MODEL_HPP
#define POKER_GAME_MODEL_HPP

#include <algorithm>

#include "PokerPlayer.hpp"
#include "metaprogramming.hpp"
#include "Card.hpp"


template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
    os << "(" << v.size() << ") " << "[";
    for(auto itr = v.cbegin(); itr != v.cend(); ++itr) {
        if(itr != v.cbegin()) {
            os << ", ";
        }
        os << *itr;
    }
    return os << "]";
}


class PokerGameModel: public ToStringCRTP<PokerGameModel> {
public:
    class Observer {
    public:
        // void on_open_community_hand(const Card& community_card);
        /**
         * @brief Called when win is determined in the PokerGameModel
         * 
         * @param community_cards 
         * @param players 
         * @param winners 
         */
        virtual void win_determined(
            const std::vector<Card>& community_cards,
            const std::vector<PokerPlayer>& players,
            const std::vector<int>& winners) = 0;
    };
private:
    std::vector<Card> deck;
    std::vector<PokerPlayer> players;
    std::vector<Card> community_cards;
    std::vector<std::reference_wrapper<Observer>> observers;
    static constexpr double ANTE_USD = 2;
    double pot;

    void initialize_deck() {
        deck.clear();
        for(int r = static_cast<int>(Rank::BEGIN); r != static_cast<int>(Rank::LAST); ++r) {
            for(int s = static_cast<int>(Suit::BEGIN); s != static_cast<int>(Suit::LAST); ++s) {
                deck.emplace_back(static_cast<Rank>(r), static_cast<Suit>(s));
            }
        }        
    }

    void shuffle_deck() {
        std::random_shuffle(deck.begin(), deck.end());
    }

    void initialize_round() {
        for(int i = 0; i < players.size(); ++i) {
            players[i].return_cards();
        }
        community_cards.clear();
        initialize_deck(); shuffle_deck();
    }

public:
    template<typename... Players_Forward>
    PokerGameModel(Players_Forward&&... players): // support std::move
        deck(), 
        community_cards(), 
        players(std::forward<Players_Forward>(players)...), 
        pot(0),
        observers()
    { }

    template<typename... ForwardArgs>
    Observer& add_observer(ForwardArgs&&... args) {
        observers.emplace_back(std::forward<ForwardArgs>(args)...);
        return observers.back();
    }

    template<typename Func>
    void notify_observers(Func f) {
        for(int i = 0; i < observers.size(); ++i) {
            f(observers[i]);
        }
    }

    void action() {
        // collect ante
        for(int i = 0; i < players.size(); ++i) {
            auto amt = players[i].collect_money(ANTE_USD);            
            pot += amt;
        }

        // give cards to players
        initialize_round();
        int offset = 0;
        for(int i = 0; i < players.size(); ++i) {
            for(int j = 0; j < 2; ++j) { // 2 cards per person
                players[i].add_to_hand(deck[deck.size()-1-offset]);
                ++offset;
            }
        }

        // 5 community cards
        for(int j = 0; j < 5; ++j) {
            community_cards.emplace_back(deck[deck.size()-1-offset]);
            ++offset;
        }

        // Determine winners
        // 1-person evaluation first
        players[0].add_from_community(community_cards);
        std::vector<int> winners = {0};
        for(int i = 1; i < players.size(); ++i) {
            players[i].add_from_community(community_cards);
            auto ref = players[winners.back()].hand_value();
            if(players[i].hand_value() < ref) {
                continue;
            }
            if(players[i].hand_value() > ref) {
                winners.clear(); // this is the sole winner.
            }
            winners.emplace_back(i); // sole winner or there is tie.
        }

        // Distribute rewards
        auto div = pot / static_cast<decltype(pot)>(winners.size());
        for(auto winner = winners.begin(); winner != winners.end(); ++winner) {
            players[*winner].earn_money(div);
        }
        pot = 0;
        notify_observers([this, &winners](Observer& observer) {
            observer.win_determined(this->community_cards, this->players, winners);
        });;
    }

    friend std::ostream& operator<<(std::ostream& os, const PokerGameModel& model) {
        os << "Model {" << std::endl;
        os << "Community Cards: " << model.community_cards << std::endl;
        os << "++++++++++++++++++++++++++++++" << std::endl;
        for(int i = 0; i < model.players.size(); ++i) {
            os << "Player " << (i+1) << ": " << model.players[i] << std::endl << std::endl;
        }
        return os << "}";
    }
};

#endif