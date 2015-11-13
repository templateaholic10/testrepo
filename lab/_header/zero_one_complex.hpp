/*! @file
    @brief std::complexのデフォルト値を定義
    @auther yanteyon10
    @date 11/10
*/

#ifndef ZERO_ONE_COMPLEX_HPP
#define ZERO_ONE_COMPLEX_HPP

#include "../zero_one.hpp"
#include <complex>

namespace util {
    template <typename T>
    struct Zero<std::complex<T>>
    {
        static std::complex<T> eval()
        {
            return std::complex<T>(0., 0.);
        }
    };
    template <typename T>
    struct One<std::complex<T>>
    {
        static std::complex<T> eval()
        {
            return std::complex<T>(1., 0.);
        }
    };
}

#endif
