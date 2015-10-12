char* gets(char* str);

#include <fstream>
#include "fermat_test.hpp"

namespace prime_test {
    void accum()
    {
        constexpr size_t n = 10000;
        std::ofstream fout("fermat_work2.txt");
        for (size_t i = 1; i <= n; i++) {
            fout << fermat_test_detail(i, n) << ((i != n) ? ',' : '\n');
        }
        fout.close();
    }

    void precise_fermat_probability()
    {
        constexpr size_t n = 10000;
        std::ofstream fout("fermat_work3.txt");
        for (size_t i = 1; i <= n; i++) {
            fout << fermat_test_p(i) << ((i != n) ? ',' : '\n');
        }
        fout.close();
    }
}

int main()
{
    prime_test::precise_fermat_probability();
}
