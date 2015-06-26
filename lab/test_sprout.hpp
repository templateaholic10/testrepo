#ifndef TEST_SPROUT
#define TEST_SPROUT

#include <sprout/array.hpp>

namespace test_sprout {
    template <size_t length>
    constexpr int test_sprout()
    {
        sprout::array<int, length> a_int = sprout::array<int, length>();

        for (size_t i = 0; i < length; i++) {
            a_int[i] = i;
        }

        int sum = 0;
        for (size_t i = 0; i < length; i++) {
            sum += a_int[i];
        }

        return sum;
    }
}

#endif
