#include "algo.hpp"
#include "Person.hpp"
#include "Randy.hpp"

namespace algo {
    namespace test {
        void PvP()
        {
            game::Game G = game::Game(std::unique_ptr <game::Character>(new game::Person("Akari", system::Playside::Alice, std::cin, std::cout)), std::unique_ptr <game::Character>(new game::Person("Sumire", system::Playside::Bob, std::cin,
                                                                                                                                                                                                     std::cout)));
            G.main();
        }

        void PvC()
        {
            game::Game G = game::Game(std::unique_ptr <game::Character>(new game::Randy("Akari", system::Playside::Alice)), std::unique_ptr <game::Character>(new game::Person("Me", system::Playside::Bob, std::cin,
                                                                                                                                                                               std::cout)));
            G.main();
        }
    }
}
