#include "algo.hpp"

namespace algo {
    namespace game {
        // 人クラス．
        class Person : public Character {
        private:
            std::istream &is;
            std::ostream &os;
        public:
            Person(const std::string &name_, system::Playside playside_, std::istream &is_, std::ostream &os_)
                : Character(name_, playside_), is(is_), os(os_)
            {
            }

            system::Guess guess(const system::Board &board) const;
            bool          one_more(const system::Board &board) const;
        };

        system::Guess Person::guess(const system::Board &board) const
        {
            os << name << ">" << std::endl;

            // 報告
            os << "     ";
            for (size_t i = 0; i < system::Board::cards_num / 2; i++) {
                os << " " << std::setw(2) << i << " ";
            }
            os << std::endl;
            os << "Opp: ";
            display_hand(board, system::invert(playside), false, os);
            os << "You: ";
            display_hand(board, playside, true, os);
            os << "Drew: ";
            os << board.pop_deck();
            os << std::endl;

            // 入力
            size_t place;
            int    number;

            os << "Guess a number of any face-down card." << std::endl;
            os << "To check history, input \"h\"." << std::endl;
            os << "place(0-" << board.hands[system::Playsidetoi(system::invert(playside))].size() << "): ";
            while (true) {
                is >> place;
                if (board.hands[system::Playsidetoi(system::invert(playside))][place].is_front) {
                    os << "Already open." << std::endl;
                    os << "place(0-" << board.hands[system::Playsidetoi(system::invert(playside))].size() << "): ";
                } else {
                    break;
                }
            }
            os << "number(0-11): ";
            is >> number;

            os << std::endl;

            return system::Guess(place, number);
        }

        bool Person::one_more(const system::Board &board) const
        {
            // 報告
            os << "Great guess!" << std::endl;
            os << "Drew: ";
            os << board.pop_deck();
            os << std::endl;

            // 入力
            char c;

            os << "Guess more?" << std::endl;
            os << "y/n: ";
            is >> c;

            os << std::endl;

            return util::ctob(c);
        }
    }
}
