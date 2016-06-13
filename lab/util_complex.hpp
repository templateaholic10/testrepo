/*! @file
    @brief std::complexのユーティリティクラス
    @auther templateaholic10
    @date 11/2
*/

#ifndef UTIL_COMPLEX
#define UTIL_COMPLEX

#include <complex>

namespace util {
    /*! @brief std::complex<T>がintで特殊化できないだけでした！　不要でしたねたはー
    */
    template <typename T>
    constexpr T real(const std::complex <T> &z)
    {
        return z.real();
    }

    template <typename T>
    constexpr T imag(const std::complex <T> &z)
    {
        return z.imag();
    }
}

// template <typename T>
// std::ostream& operator<<(std::ostream& os, const std::complex<T>& z)
// {
//     os << "(" << std::real(z) << "," << z.imag() << ")";
//     return os;
// }

#endif
