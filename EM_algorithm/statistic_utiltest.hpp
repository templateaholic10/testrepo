#ifndef STATISTIC_UTILTEST
#define STATISTIC_UTILTEST

#include "statistic_util.hpp"

namespace statistic_util {
    void testexpand_array()
    {
        static_assert(std::is_same <expand_array <1, int, 2, 3>, util::multi_array <int, 3, 4> >::value, "expand_array error!");
    }

    void testdescrete1()
    {
        const auto f = [](double x) -> double {
                           return x * x;
                       };
        constexpr int     dim = 1;
        const Range <dim> range({ -10. }, { 10. });
        constexpr int     xmeshnum = 20;
        auto              fd       = Descretize <xmeshnum>::descretize(f, range);
        static_assert(std::is_same <decltype(fd), util::multi_array <double, 21> >::value, "fd error!");
        for (int i = 0; i <= xmeshnum; i++) {
            std::cout << fd[i] << ((i != xmeshnum) ? ',' : '\n');
        }
    }

    void testdescrete2()
    {
        const auto f = [](double x, double y) -> double {
                           return x * x + y * y;
                       };
        constexpr int     dim = 2;
        const Range <dim> range({ -10., -10. }, { 10., 10. });
        constexpr int     xmeshnum = 200;
        constexpr int     ymeshnum = 200;
        auto              fd       = Descretize <xmeshnum, ymeshnum>::descretize(f, range);
        static_assert(std::is_same <decltype(fd), util::multi_array <double, 201, 201> >::value, "fd error!");
        for (int i = 0; i <= xmeshnum; i++) {
            for (int j = 0; j <= ymeshnum; j++) {
                std::cout << fd[i][j] << ((j != ymeshnum) ? ',' : '\n');
            }
        }
    }

    void testdescreteDV1()
    {
        constexpr int     dim = 1;
        const auto f = [](const dvector<dim> &v) -> double {
                           return v(0) * v(0);
                       };
        const Range <dim> range({ -10. }, { 10. });
        constexpr int     xmeshnum = 20;
        auto              fd       = Descretize <xmeshnum>::descretizeDV(f, range);
        static_assert(std::is_same <decltype(fd), util::multi_array <double, 21> >::value, "fd error!");
        for (int i = 0; i <= xmeshnum; i++) {
            std::cout << fd[i] << ((i != xmeshnum) ? ',' : '\n');
        }
    }

    void testdescreteDV2()
    {
        constexpr int     dim = 2;
        const auto f = [](const dvector<dim> &v) -> double {
                           return v(0) * v(0) + v(1) * v(1);
                       };
        const Range <dim> range({ -10., -10. }, { 10., 10. });
        constexpr int     xmeshnum = 200;
        constexpr int     ymeshnum = 200;
        auto              fd       = Descretize <xmeshnum, ymeshnum>::descretizeDV(f, range);
        static_assert(std::is_same <decltype(fd), util::multi_array <double, 201, 201> >::value, "fd error!");
        for (int i = 0; i <= xmeshnum; i++) {
            for (int j = 0; j <= ymeshnum; j++) {
                std::cout << fd[i][j] << ((j != ymeshnum) ? ',' : '\n');
            }
        }
    }
}

#endif
