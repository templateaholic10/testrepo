/*! @file
    @brief std::complexのユーティリティ関数群
    @author yanteyon10
    @date 10/28
*/
#ifndef EXCOMPLEX_HPP
#define EXCOMPLEX_HPP

#include <complex>

namespace std {
    /*! @struct complexify
        @brief 実数型を複素化するメタ関数
    */
    template <typename Floating_point>
    struct complexify {
        using type = std::complex<Floating_point>;
    };

    template <typename Floating_point>
    struct complexify<std::complex<Floating_point>> {
        using type = std::complex<Floating_point>;
    };

    /*! @struct decomplexify
        @brief 複素数型を脱複素化するメタ関数
    */
    template <typename Floating_point>
    struct decomplexify {
        using type = Floating_point;
    };

    template <typename Floating_point>
    struct decomplexify<std::complex<Floating_point>> {
        using type = Floating_point;
    };
}

#endif
