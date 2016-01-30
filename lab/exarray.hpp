/*! @file
    @brief std::arrayのユーティリティ関数
    @auther yanteyon10
    @date 11/29
*/

#ifndef EXARRAY_HPP
#define EXARRAY_HPP

#include <array>

namespace std {
    template <typename T, int n>
    std::array<T, n> C_array(const T &value)
    {
        std::array<T, n> retval;
        retval.fill(value);
        return retval;
    }
}

#endif
