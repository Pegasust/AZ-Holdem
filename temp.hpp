class SortedHand: std::vector<Card> {
public:
    /// Iterators
    using Collection::begin; using Collection::end;
    using Collection::cbegin; using Collection::cend;
    using Collection::rbegin; using Collection::rend;
    using Collection::crbegin; using Collection::crend;

    /// Common operators
    using Collection::size;
    using Collection::operator[];
    using Collection::at;
    using Collection = std::vector<Card>;
private:
    int ace_value;
    HandType _hand_type;

    const Collection& cself() const {
        return *static_cast<const Collection*>(this);
    }
    const Collection& self() const {
        return cself();
    }
    Collection& self() {
        return const_cast<Collection&>(cself());
    }

    /**
     * @brief The suit that's causing this hand to be a flush
     * 
     * @return Optional<Suit> 
     */
    Optional<Suit> flushing_suit() {
        static constexpr int n_suits = 4; // there are 4 suits in a deque.
        std::array<Suit, n_suits> suit_count= {0};
        /// populate suit count
        for(auto itr = self().cbegin(); itr != self().cend(); ++itr) {
            suit_count[static_cast<int>(itr->getSuit())] += 1;
        }
        /// determine if there is a flushing suit (suit with count >= 5)
        auto flush_suit_itr = std::find_if(suit_count.begin(), suit_count.end(), [](const auto& suit) {
            return suit >= 5;
        });

        if(flush_suit_itr == suit_count.end()) {
            return {}; // no suit
        }
        return make_optional<Suit>(static_cast<Suit>(flush_suit_itr - suit_count.begin()));
    }

    Card _straight_flush_lowest_card(Suit flushing_suit) {        

    }

    int find_ace_value(Optional<Suit> flushing_suit) {
        assert(hand.size() >= 5 && "Attempting to evaluate a poker hand with less than 5 size");
        // assume: hand is sorted.
        // check if there is ace
        if(highest_card().getRank() != Rank::ACE) {
            // no need to check, card is correctly sorted.
            return ace_value;
        }
        // TODO: assume lower value of ace is 1.

        // The only way ace takes on value of 1 is when there is
        // straight flush containing 2-3-4-5
        if(!flushing_suit.has_value()) { // no flush, no need to take-low
            return ace_value;
        }

        // Also has to be a straight flush to take low.
        auto has_ace = std::any_of(self().cbegin(), self().cend(), [&flushing_suit](const Card& card) {
            return card.getRank() == Rank::ACE && card.getSuit() == suit;
        });
        
        return 1;
    }
    
    void init() {
        // initial sort (ace might be at the end)
        std::sort(Collection::begin(), Collection::end());
        // figure out ace's value

        // if flush, we check for straight flush
        Optional<Suit> flush = flushing_suit();
        if(flush.has_value()) {
            // see if we're doing straight flush
            for(int i = 1; i < sorted_hand.size(); ++i) {
                int higher_rank = static_cast<int>(sorted_hand[i].getRank());
                auto _lower_rank = sorted_hand[i-1].getRank();
                // tidbits to figure out ace's value
                int lower_rank = ace_value;
                if(_lower_rank == Rank::ACE && higher_rank == static_cast<int>(Rank::DEUCE)) {
                    ace_value = 1;
                    lower_rank = ace_value;
                }
                // check if every card is separated by 1 (straight flush)
                if(higher_rank - lower_rank != 1) {
                    // not straight flush, only a flush
                    return HandType::FLUSH;
                }
            }
            // it is a straight flush, every card in sorted hand is separated by 1
            return HandType::STRAIGHT_FLUSH;
        }

        /// Rank check
        auto rank_value = [&ace_value](const Card& card) {
            const auto rank = card.getRank();
            return rank == Rank::ACE? ace_value: static_cast<int>(rank);
        };

        int most_common_rank = 1; // == 4 if 4 of a kind, == 3 if 3 of a kind
        int next_common_rank = 1; // == 2 if full house or 2-pair
        int &common_ranks[] = {most_common_rank, next_common_rank};

        for(int i = 0; i < )
    }

public:
    template<typename... TConstructs>
    SortedHand(TConstructs&&... args): 
        vector(std::forward<TConstructs>(args)...)
    {
        init();
    }

    const Card& highest_card() const {
        return operator[](size()-1);
    }

    Card& highest_card() {
        return const_cast<Card&>(static_cast<const SortedHand*>(this)->highest_card());
    }
    const HandType hand_type() const {
        return _hand_type;
    }

    int card_rank(const Card& card) {
        auto rank = card.getRank();
        if(rank == Rank::ACE) {
            return ace_value;
        }
        return static_cast<int>(rank);
    }
};