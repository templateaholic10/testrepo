#include <iostream>
#include <fstream>
#include <util>
#include "adjacency_matrix.hpp"

int main()
{
    std::cout << util::Repeat("-", 20) << std::endl;
    std::cout << "ADJACENCY_MATRIX" << std::endl;

    constexpr size_t n = 4;
    graph::Adjacency_matrix<n> M;

    const std::string finname = "sample.in";
    std::ifstream fin(finname);
    if (fin.fail()) {
        return 1;
    }
    fin >> M;
    fin.close();

    _DISPLAY(M)
    _DISPLAY(M.self_check())
    M.Ubuild();
    _DISPLAY(M)

    const std::string foutname = "sample.out";
    std::ofstream fout(foutname);
    if (fout.fail()) {
        return 1;
    }
    fout << M;
    fout.close();

    std::cout << util::Repeat("-", 20) << std::endl;
    return 0;
}
