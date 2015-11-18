#include <iostream>
#include <util>
#include <array>
#include "element_wise.hpp"

int main(int argc, char const *argv[])
{
    // static_assert(util::has_iterator<int>::value, "has no iterator");
    static_assert(util::has_iterator<std::array<int, 5>>::value, "has no iterator");
    constexpr size_t n = 5;
    constexpr char delim = ',';

    using double_array = std::array<std::array<int, n>, n>;

    auto display = [&](const double_array& a){
        std::cout << a[0][0];
        for (size_t j = 1; j < n; j++) {
            std::cout << delim << a[0][j];
        }
        for (size_t i = 1; i < n; i++) {
            std::cout << std::endl << a[i][0];
            for (size_t j = 1; j < n; j++) {
                std::cout << delim << a[i][j];
            }
        }
        std::cout << std::endl << std::endl;
    };

    double_array hoge;
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            hoge[i][j] = i + j;
        }
    }
    display(hoge);

    double_array piyo;
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            piyo[i][j] = i * j;
        }
    }
    display(piyo);

    // std::array<double, n> fuga;
    // fuga.fill(2.);
    // _DISPLAY_SEQ(fuga)

    display(hoge + piyo);
    display(hoge - piyo);
    display(hoge * piyo);

    return 0;
}
