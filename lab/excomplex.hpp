/*! @file
    @brief std::complexのユーティリティ関数群
    @author yanteyon10
    @date 10/28
*/
#ifndef EXCOMPLEX_HPP
#define EXCOMPLEX_HPP

#include <complex>

namespace std {
    /*! @struct decomplexify
        @brief 複素数型を脱複素化するメタ関数
    */
    template <typename T>
    struct decomplexify {
        using type = T;
    };

    template <typename Floating_point>
    struct decomplexify<std::complex<Floating_point>> {
        using T = Floating_point;
    };
}

#endif
