#ifndef DESTRUCTIVE_SCAN
#define DESTRUCTIVE_SCAN

#include <vector>
#include <iostream>
#include <algorithm>

namespace destructive_scan {
    void test_destructive_scan()
    {
        auto display = [](const std::vector <int> &_v) {
                           std::for_each(_v.begin(), _v.end(), [](int a) {
                std::cout << a << " ";
            });
                           std::cout << std::endl;
                       };
        std::vector <int> v(10);
        std::iota(v.begin(), v.end(), 0);
        int counter = 0;
        std::for_each(v.begin(), v.end(), [&](int a) {
            display(v); v.resize(10 - counter); counter++;
        });
        std::cout << std::endl;
    }
}

#endif
