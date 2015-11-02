char *gets(char *str);

#include <random>
#include "util.hpp"
#include "util_rand.hpp"

named_enum(RGB, R, G, B);

int main(int argc, char const *argv[])
{
    static_assert(std::is_same<util::Uniform<int>::result_type, int>::value, "wrong specialization");
    constexpr size_t max_rep = 10;
    util::Uniform<double> uniform(1, max_rep);
    for (size_t i = 0; i < max_rep; i++) {
        std::cout << uniform() << std::endl;
    }
    return 0;
}
