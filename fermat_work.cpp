char* gets(char* str);

#include <fstream>
#include "fermat_test.hpp"

int main()
{
    constexpr size_t n = 10000;
    std::ofstream fout("fermat_work2.txt");
    for (size_t i = 1; i <= n; i++) {
        fout << prime_test::fermat_test_detail(i, n) << ((i != n) ? ',' : '\n');
    }
    fout.close();
}
