/*! @file
    @brief std::complexのデフォルト値を定義
    @auther yanteyon10
    @date 11/10
*/

#ifndef DEFAULT_VALUE_COMPLEX_HPP
#define DEFAULT_VALUE_COMPLEX_HPP

#include "../default_value.hpp"
#include <complex>

namespace util {
    template <typename T>
    struct Default_value<std::complex<T>>
    {
        static std::complex<T> eval()
        {
            return std::complex<T>(0., 0.);
        }
    };
}

#endif
