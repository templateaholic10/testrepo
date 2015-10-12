#include <iostream>
#include <random>
#include <cassert>
#include "lab/util_math.hpp"

namespace prime_test {
    int gcd(int n, int m)
    {
        assert(n > 0 && m > 0);
        if (n < m) {
            std::swap(n, m);
        }
        while (m != 0) {
            int r = n % m;
            n = m;
            m = r;
        }

        return n;
    }

    bool unit_fermat_test(int p)
    {
        if (p <= 1) {
            return false;
        } else if (p == 2) {
            return true;
        }

        std::random_device               rnd;
        std::mt19937                     mt(rnd());
        std::uniform_int_distribution <> randint(2, p - 1);
        int                              a = randint(mt);

        return gcd(p, a) == 1 && util::power(a, p-1) % p == 1;
    }

    bool fermat_test(int p)
    {
        if (p <= 1) {
            return false;
        } else if (p == 2) {
            return true;
        }

        std::random_device               rnd;
        std::mt19937                     mt(rnd());
        std::uniform_int_distribution <> randint(2, p - 1);
        constexpr size_t                 max_rep = 10;
        for (size_t i = 0; i < max_rep; i++) {
            int a = randint(mt);
            if (gcd(p, a) != 1 || util::power(a, p-1) % p != 1) {
                return false;
            }
        }

        return true;
    }

    int fermat_test_detail(int p, size_t n)
    {
        if (p <= 1) {
            return 0;
        } else if (p == 2) {
            return n;
        }

        std::random_device               rnd;
        std::mt19937                     mt(rnd());
        std::uniform_int_distribution <> randint(2, p - 1);
        int counter = 0;
        for (size_t i = 0; i < n; i++) {
            int a = randint(mt);
            if (gcd(p, a) == 1 && util::power(a, p-1) % p == 1) {
                // 素数判定された回数を数える．
                counter++;
            }
        }
        return counter;
    }
}
