/*! @file
    @brief std::complexのユーティリティ関数群
    @author templateaholic10
    @date 10/28
*/
#ifndef EXCOMPLEX_HPP
#define EXCOMPLEX_HPP

#include <complex>

namespace std {
    /*! @struct is_complex
        @brief 複素数型かどうか判定するメタ関数
    */
    template <typename T, class Ignored = void>
    struct is_complex {
        static constexpr bool value = false;
    };

    template <typename Floating>
    struct is_complex<std::complex<Floating>, typename std::enable_if<std::is_floating_point<Floating>::value>::type> {
        static constexpr bool value = true;
    };

    /*! @struct complexify
        @brief 実数型を複素化するメタ関数
    */
    template <typename T, class Ignored = void>
    struct complexify;

    template <typename Floating>
    struct complexify<Floating, typename std::enable_if<std::is_floating_point<Floating>::value>::type> {
        using type = std::complex<Floating>;
    };

    template <typename Floating>
    struct complexify<std::complex<Floating>, typename std::enable_if<std::is_floating_point<Floating>::value>::type> {
        using type = std::complex<Floating>;
    };

    /*! @struct decomplexify
        @brief 複素数型を脱複素化するメタ関数
    */
    template <typename T, class Ignored = void>
    struct decomplexify;

    template <typename Floating>
    struct decomplexify<Floating, typename std::enable_if<std::is_floating_point<Floating>::value>::type> {
        using type = Floating;
    };

    template <typename Floating>
    struct decomplexify<std::complex<Floating>, typename std::enable_if<std::is_floating_point<Floating>::value>::type> {
        using type = Floating;
    };

    /*! brief 虚数単位
    */
    template <typename Complex = std::complex<double>, typename std::enable_if<std::is_complex<Complex>::value>::type* = nullptr>
    constexpr Complex I()
    {
        return Complex(0., 1.);
    }
}

#endif
