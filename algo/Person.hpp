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

            system::Guess guess(const Game &game) const;
            bool          one_more(const Game &game) const;
        };

        system::Guess Person::guess(const Game &game) const
        {
            os << name << ">" << std::endl;

            // 報告
            os << "     ";
            for (size_t i = 0; i < system::Board::cards_num / 2; i++) {
                os << " " << std::setw(2) << i << " ";
            }
            os << std::endl;
            os << "Opp: ";
            display_hand(game.board, system::invert(playside), false, os);
            os << "You: ";
            display_hand(game.board, playside, true, os);
            os << "Drew: ";
            os << game.board.pop_deck();
            os << std::endl;

            // 入力
            const char command_history = 'h';
            std::string word;
            int place;
            int    number;

            auto show_history = [&](){
                os << Record::Header() << std::endl;
                for (auto record : game.history) {
                    os << record << std::endl;
                }
            };

            os << "Guess a number of any face-down card." << std::endl;
            os << "To check history, input \"h\"." << std::endl;

            os << "place(0-" << game.board.hands[system::Playsidetoi(system::invert(playside))].size() << "): ";
            while (true) {
                is >> word;
                if (word[0] == command_history) {
                    show_history();
                } else {
                    try {
                        place = std::stoi(word);
                        if (place >= 0 && place < game.board.hands[system::Playsidetoi(system::invert(playside))].size()) {
                            if (game.board.hands[system::Playsidetoi(system::invert(playside))][place].is_front) {
                                os << "Already open." << std::endl;
                            } else {
                                break;
                            }
                        }
                    } catch (...) {
                    }
                    os << "Input 0-" << game.board.hands[system::Playsidetoi(system::invert(playside))].size() << " number." << std::endl;
                }
                os << "place(0-" << game.board.hands[system::Playsidetoi(system::invert(playside))].size() << "): ";
            }

            os << "number(0-11): ";
            while (true) {
                is >> word;
                if (word[0] == command_history) {
                    show_history();
                } else {
                    try {
                        number = std::stoi(word);
                        if (number >= 0 && number < 12) {
                            break;
                        }
                    } catch (...) {
                    }
                    os << "Input 0-11 number." << std::endl;
                }
                os << "number(0-11): ";
            }

            os << std::endl;

            return system::Guess(place, number);
        }

        bool Person::one_more(const Game &game) const
        {
            // 報告
            os << "Great guess!" << std::endl;
            os << "Drew: ";
            os << game.board.pop_deck();
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
