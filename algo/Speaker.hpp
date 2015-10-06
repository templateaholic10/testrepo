#include "algo.hpp"

namespace algo {
    namespace game {
        // CPUの挙動を出力するラッパー．
        // Speaker<Randy>は喋るRandyを定義する．
        template <class CPU>
        class Speaker : public Character {
        private:
            CPU cpu;
            std::istream &is;
            std::ostream &os;
        public:
            Speaker(const std::string &name_, system::Playside playside_, std::istream &is_, std::ostream &os_)
                : Character(name_, playside_), cpu(name_, playside_), is(is_), os(os_)
            {
            }

            system::Guess guess(const Game &game) const;
            bool          one_more(const Game &game) const;
        };

        template <class CPU>
        system::Guess Speaker<CPU>::guess(const Game &game) const
        {
            auto _guess = cpu.guess(game);
            os << name << ">" << std::endl;
            os << "Guess:" << std::endl;
            os << system::Guess::Header() << std::endl;
            os << _guess << std::endl;
            os << std::endl;
            return _guess;
        }

        template <class CPU>
        bool Speaker<CPU>::one_more(const Game &game) const
        {
            os << name << " guesses right!" << std::endl;
            bool _one_more =  cpu.one_more(game);
            os << name << " chooses " << (_one_more ? "attack" : "stay") << std::endl;
            os << std::endl;
            return _one_more;
        }
    }
}
