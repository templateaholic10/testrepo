char* gets(char* str);

#include <fstream>
#include "lab/util.hpp"
#include "fermat_test.hpp"

namespace prime_test {
    constexpr size_t n = 10000;

    void accum()
    {
        std::ofstream fout("fermat_work2.csv");
        for (size_t i = 1; i <= n; i++) {
            fout << fermat_test_detail(i, n) << std::endl;
        }
        fout.close();
    }

    void precise_fermat_probability()
    {
        std::ofstream fout("fermat_work3.csv");
        for (size_t i = 1; i <= n; i++) {
            fout << fermat_test_p(i) << std::endl;
        }
        fout.close();
    }

    void prime_list()
    {
        std::ofstream fout("fermat_work4.csv");
        for (size_t i = 1; i <= n; i++) {
            fout << primitive(i) << std::endl;
        }
        fout.close();
    }

    void variable_test(int p)
    {
        std::cout << util::Repeat("-", 20) << std::endl;
        std::cout << "p = " << p << std::endl;
        std::cout << "in fact: " << (primitive(p) ? "prime" : "not prime") << std::endl;
        std::cout << "fermat_test: " << (fermat_test(p) ? "prime" : "not prime") << std::endl;
        std::cout << "P(p is prime) with fermat_test: " << fermat_test_p(p) << std::endl;
        std::cout << util::Repeat("-", 20) << std::endl;
    }
}

int main(int argc, char* argv[])
{
    if (argc == 1) {
        prime_test::prime_list();
    } else {
        prime_test::variable_test(std::stoi(argv[1]));
    }
}
