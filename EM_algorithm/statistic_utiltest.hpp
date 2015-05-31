#ifndef STATISTIC_UTILTEST
#define STATISTIC_UTILTEST

#include "statistic_util.hpp"

namespace statistic {
    void testexpand_array()
    {
        static_assert(std::is_same<expand_array<1, int, 2, 3>, util::multi_array<int, 3, 4>>::value, "expand_array error!");
    }

    void testdescrete()
    {
        const auto f = [](double x, double y) -> double {return x * x + y * y;};
        constexpr int dim = 2;
        const Range<dim> range({-10., 10.}, {-10., 10.});
        constexpr int xmeshnum = 20;
        constexpr int ymeshnum = 20;
        auto fd = Descretize<xmeshnum, ymeshnum>::descretize(f, range);
        for (int i = 0; i <= xmeshnum; i++) {
            for (int j = 0; j <= ymeshnum; j++) {
                std::cout << fd[i][j] << ((j != ymeshnum) ? ',' : '\n');
            }
        }
    }
}

#endif
