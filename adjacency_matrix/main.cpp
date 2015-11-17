#include <iostream>
#include <fstream>
#include <util>
#include "rank_distribution.hpp"

int main()
{
    std::cout << util::Repeat("-", 20) << std::endl;
    std::cout << "AKARI..." << std::endl;

    constexpr int n = 4;
    // auto result = graph::rank_distribution<n>();
    // _DISPLAY_SEQ(result)
    graph::Adjacency_matrix<n> M;
    const std::string finname = "sample.in";
    std::ifstream fin(finname);
    fin >> M;
    fin.close();

    _DISPLAY(M)
    M(0, 1) = 0;
    _DISPLAY(M)
    M.at(0, 1) = 1;
    _DISPLAY(M)

    std::cout << util::Repeat("-", 20) << std::endl;
    return 0;
}
