#ifndef STATISTIC_UTILTEST
#define STATISTIC_UTILTEST

#include "statistic_util.hpp"

namespace statistic_util {
    void testexpand_array()
    {
        static_assert(std::is_same<statistic::expand_array<1, int, 2, 3>, util::multi_array<int, 3, 4>>::value, "expand_array error!");
    }
}

#endif
