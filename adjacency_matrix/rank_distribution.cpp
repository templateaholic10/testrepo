#include <fstream>
#include <array_matrix>
#include <element_wise>
#include <util>
#include <array_io>
#include <os>
#include "rank_distribution.hpp"

int main()
{
    constexpr int n = 8;
    constexpr int rank_num = 10;
    std::matrix<int, rank_num, n+1> result;

    const double interval = 1. / rank_num;
    double lower_bound = 0.0;
    double upper_bound = lower_bound + interval;
    for (size_t i = 0; i < rank_num; i++) {
        result[i] = graph::rank_distribution<n>(lower_bound, upper_bound);
        lower_bound += interval;
        upper_bound += interval;
    }

    std::array<int, n+1> sum;
    sum.fill(0);
    for (auto row : result) {
        sum += row;
    }

    std::array<int, n+1> all;
    all = graph::rank_distribution<n>();

    const std::string dirname = "result";
    const std::string foutname = os::path::join(dirname, std::to_string(n) + ".out");
    std::ofstream fout(foutname);
    fout << result << std::endl;
    fout << sum << std::endl;
    fout << all << std::endl;
    fout.close();

    return 0;
}
