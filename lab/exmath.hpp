/*! @file
    @brief 数学関係のユーティリティ関数群
*/
#ifndef EXMATH_HPP
#define EXMATH_HPP

#include <cstddef>
#include <type_traits>
#include <cmath>
#include <constexpr/cmath>

namespace std {
    /*! @brief 剰余．標準の剰余演算子は
         -2 % 5 = -2
         のように0に近い方に丸める．このmodは有限環への写像を定めるため
         mod(-2, 5) = 3
    */
    template <typename Lhs, typename Rhs, typename std::enable_if<std::is_integral<Lhs>::value>::type* = nullptr>
    constexpr Lhs mod(const Lhs &lhs, const Rhs &rhs)
    {
        return lhs % rhs + (lhs < 0 ? rhs : 0);
    }

    /*! @brief 実数版．角度を[0, 2\PI)に納めるのに使ったりする
    */
    template <typename Lhs, typename Rhs, typename std::enable_if<std::is_floating_point<Lhs>::value>::type* = nullptr>
    constexpr Lhs mod(const Lhs &lhs, const Rhs &rhs)
    {
        return lhs - (cpstd::floor(lhs/rhs)) * rhs;
    }

    /*! @brief 円周率
    */
    template <typename Floating = double, typename std::enable_if<std::is_floating_point<Floating>::value>::type* = nullptr>
    constexpr Floating PI()
    {
        return std::acos(static_cast<Floating>(-1.));
    }

    /*! @brief 自然対数の底
    */
    template <typename Floating = double, typename std::enable_if<std::is_floating_point<Floating>::value>::type* = nullptr>
    constexpr Floating E()
    {
        return std::exp(static_cast<Floating>(1.));
    }

    /*! @brief 浮動小数点型は自身を，整数型はdoubleを返すメタ関数
    */
    template <typename T, typename Ignored = void>
    struct floatify;

    template <typename F>
    struct floatify <F, typename std::enable_if<std::is_floating_point<F>::value>::type> {
        using type = F;
    };

    template <typename I>
    struct floatify <I, typename std::enable_if<std::is_integral<I>::value>::type> {
        using type = double;
    };

    /*! @brief 2次方程式の判別式を求める関数
    */
    template <typename T>
    constexpr typename floatify<T>::type D(T a, T b, T c)
    {
        return b*b - 4.*a*c;
    }

    /*! @brief 2次方程式の大きい根を求める関数．桁落ちが小さい表式で求める．実根を持たないときエラー
    */
    template <typename T>
    constexpr typename floatify<T>::type quad_realroot1(const T a, const T b, const T c)
    {
        return (b>=0.) ? ((-2.*c) / (b + cpstd::sqrt(b*b-4.*a*c))) : ((-b + cpstd::sqrt(b*b - 4.*a*c)) / (2.*a));
    }

    /*! @brief a == 1のとき
    */
    template <typename T>
    constexpr typename floatify<T>::type quad_realroot1(const T b, const T c)
    {
        return (b>=0.) ? ((-2.*c) / (b + cpstd::sqrt(b*b-4.*c))) : ((-b + cpstd::sqrt(b*b - 4.*c)) / (2.));
    }

    /*! @brief 2次方程式の小さい根を求める関数．桁落ちが小さい表式で求める．実根を持たないときエラー
    */
    template <typename T>
    constexpr typename floatify<T>::type quad_realroot2(const T a, const T b, const T c)
    {
        return (b>=0.) ? ((-b - cpstd::sqrt(b*b - 4.*a*c)) / (2.*a)) : ((2.*c) / (-b + cpstd::sqrt(b*b-4.*a*c)));
    }

    /*! @brief a == 1のとき
    */
    template <typename T>
    constexpr typename floatify<T>::type quad_realroot2(const T b, const T c)
    {
        return (b>=0.) ? ((-b - cpstd::sqrt(b*b - 4.*c)) / (2.)) : ((2.*c) / (-b + cpstd::sqrt(b*b-4.*c)));
    }
}

#endif
