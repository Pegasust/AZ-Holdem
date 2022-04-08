#include <iostream>
#include <iomanip>
#include "PokerGameModel.hpp"
#include "PokerPlayer.hpp"

static constexpr double STARTING_BALANCE = 100;
// template<typename T>
// std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
//     os << "(" << v.size() << ") " << "[";
//     for(auto itr = v.cbegin(); itr != v.cend(); ++itr) {
//         if(itr != v.cbegin()) {
//             os << "\n";
//         }
//         os << *itr;
//     }
//     return os << "]";
// }
class CLIPokerGameObserver: public virtual PokerGameModel::Observer {
private:
    std::ostream& print_player(int i, const PokerPlayer& player) {
        return std::cout << "Player " << (i+1) << ": $" 
                  << std::fixed << std::setprecision(2) 
                  << player.get_balance();
    }
    std::ostream& print_best_hand(const PokerPlayer& player) {
        return std::cout << player.get_hand().get_best_hand()
                      << "   " << player.get_hand().get_hand_value().type;
    }
public:
    void win_determined(
            const std::vector<Card>& community_cards,
            const std::vector<PokerPlayer>& players,
            const std::vector<int>& winners) 
    {
        std::cout << "Community Cards: " << community_cards << std::endl;
        std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
        for(int i = 0; i < players.size(); ++i) {
            auto& player = players[i];
            print_player(i, player)
                      << " - " << player.get_cards();
            std::cout << "\n    Best hand: ";
            print_best_hand(player);
            std::cout << std::endl << std::endl;
        }
        std::cout << "Winner(s):";
        for(int i = 0; i < winners.size(); ++i) {
            std::cout << " ";
            print_player(winners[i], players[winners[i]]);
        }
        std::cout << std::endl;
        std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
        for(int i = 0; i < winners.size(); ++i) {
            std::cout << " ";
            print_best_hand(players[winners[i]]) << "   ";
            print_player(winners[i], players[winners[i]]) << std::endl;
        }
        std::cout << std::endl;
    }

};

int main() {
    int n_players;
    std::cout << "How many players? " << std::flush;
    std::cin >> n_players;

    // construct default players (if we were to extend the game to save players, we can do it here)
    std::vector<PokerPlayer> players;
    for(int i = 0; i < n_players; ++i) {
        players.emplace_back(STARTING_BALANCE);
    }

    // construct model
    PokerGameModel model {std::move(players)};
    CLIPokerGameObserver obs;
    model.add_observer(obs);
    
    bool play = true;
    do {
        std::cout << std::endl << std::endl << std::endl;
        model.action();
        // std::cout << model << std::endl;
        std::cout << "Play another game? <y or n> " << std::flush;
        std::string continue_response;
        std::cin >> continue_response;
        play = toupper(continue_response[0]) == 'Y';
    } while(play);
}