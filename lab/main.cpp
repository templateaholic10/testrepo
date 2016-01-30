#include <iostream>
#include <fstream>
#include "exrandom.hpp"
#include <debug>

int main(int argc, char const *argv[])
{
    using T = double;
    std::Chisq<T> chisq;
    constexpr int n = 1000;
    const std::string filename = "test_chi.csv";
    std::ofstream fout(filename);

    for (int i = 0; i < n; i++) {
        fout << chisq() << std::endl;
    }

    fout.close();

    return 0;
}
