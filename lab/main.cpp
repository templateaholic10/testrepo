char *gets(char *str);

#include <random>
#include "util.hpp"
#include "util_rand.hpp"
#include "util_complex.hpp"

named_enum(RGB, R, G, B);

int main(int argc, char const *argv[])
{
    static_assert(std::is_same<util::Uniform<int>::result_type, int>::value, "wrong specialization");
    constexpr size_t max_rep = 10;
    util::Uniform<std::complex<double>> uniform;
    for (size_t i = 0; i < max_rep; i++) {
        auto hoge = uniform();
        std::cout << "(" << hoge.real() << "," << hoge.imag() << ")" << std::endl;
    }
    return 0;
}
