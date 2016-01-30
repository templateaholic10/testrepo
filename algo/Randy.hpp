#include "algo.hpp"

namespace algo {
    namespace game {
        // あてずっぽうに予想するプレイヤー．
        // 名前の由来はrandom．
        class Randy : public Character {
        private:
        public:
            Randy(const std::string &name_, system::Playside playside_)
                : Character(name_, playside_)
            {
            }

            system::Guess guess(const Game &game) const;
            bool          one_more(const Game &game) const;
        };

        system::Guess Randy::guess(const Game &game) const
        {
            auto &opp_hand = game.board.hands[system::Playsidetoi(system::invert(playside))];

            // 入力
            size_t place;
            int    number;

            std::random_device rnd;
            std::mt19937       mt(rnd());

            std::uniform_int_distribution <> rnd_place(0, opp_hand.size() - 1);
            while (true) {
                place = rnd_place(mt);
                if (!opp_hand[place].is_front) {
                    break;
                }
            }

            std::uniform_int_distribution <> rnd_number(0, 11);
            number = rnd_number(mt);

            return system::Guess(place, number);
        }

        bool Randy::one_more(const Game &game) const
        {
            return true;
        }
    }
}
