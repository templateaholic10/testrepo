/*! @file
    @brief 数学関係のユーティリティ関数群
*/
#ifndef UTIL_MATH
#define UTIL_MATH

#include <cstddef>
#include <array>
#include <bitset>
#include <sprout/assert.hpp>
#include <sprout/array.hpp>
#include <sprout/bitset.hpp>

namespace util {
    /*! @brief デフォルトの十分小さい正の数
    */
    constexpr double epsilon = 10e-6;

    /*! @brief 離散畳み込みを求める関数
        @param x 関数1（std::array）
        @param y 関数2（std::array）
        @return x*y
    */
    template <typename T, size_t n>
    T conv(const std::array<T, n>& x, const std::array<T, n>& y)
    {
        T sum = x[0] * y[n-1];
        for (size_t i = 1; i < n; i++) {
            sum += x[i] * y[n-1-i];
        }
        return sum;
    }

    /*! @brief 内積を求める関数
        @param x 関数1（std::array）
        @param y 関数2（std::array）
        @return x.y
    */
    template <typename T, size_t n>
    T inner(const std::array<T, n>& x, const std::array<T, n>& y)
    {
        T sum = x[0] * y[0];
        for (size_t i = 1; i < n; i++) {
            sum += x[i] * y[i];
        }
        return sum;
    }

    /*! @brief 2次方程式の判別式を求める関数
    */
    constexpr double D(double a, double b, double c)
    {
        return b*b - 4.*a*c;
    }

    /*! @brief 2次方程式の大きい根を求める関数．桁落ちが小さい表式で求める．実根を持たないときエラー
    */
    double quad_root_real1(double a, double b, double c)
    {
        return (b>=0) ? ((-2.*c) / (b + sqrt(b*b-4.*a*c))) : ((-b + sqrt(b*b - 4.*a*c)) / (2.*a));
    }

    /*! @brief 2次方程式の小さい根を求める関数．桁落ちが小さい表式で求める．実根を持たないときエラー
    */
    constexpr double quad_root_real2(const double a, const double b, const double c)
    {
        return (b>=0) ? ((-b - sqrt(b*b - 4.*a*c)) / (2.*a)) : ((2.*c) / (-b + sqrt(b*b-4.*a*c)));
    }

    /*! @brief 整数列の，各要素の右からdigit桁目を並べた整数列を返す関数
        @param org_array 整数列（std::array）
        @param radix 基数
        @param digit 抽出する桁（右から数える）
        @return 整数列（std::array）
    */
    template <typename Numeric, size_t length>
    std::array <Numeric, length> extract_digit(const std::array <Numeric, length> &org_array, const size_t radix, const size_t digit)
    {
        std::array <Numeric, length> result = std::array <Numeric, length>();
        if (digit == 0) {
            return result;
        }

        for (size_t i = 0; i < length; i++) {
            result[i] = (org_array[i] / power(radix, digit - 1)) % power(radix, digit);
        }

        return result;
    }

    /*! @brief 整数列の，各要素の右からdigit桁目を並べた整数列を返す関数，のコンパイル時版
        @param org_array 整数列（sprout::array）
        @param radix 基数
        @param digit 抽出する桁（右から数える）
        @return 整数列（sprout::array）
    */
    template <typename Numeric, size_t length>
    constexpr sprout::array <Numeric, length> extract_digit(const sprout::array <Numeric, length> &org_array, const size_t radix, const size_t digit)
    {
        sprout::array <Numeric, length> result = std::array <Numeric, length>();
        if (digit == 0) {
            return result;
        }

        for (size_t i = 0; i < length; i++) {
            result[i] = (org_array[i] / power(radix, digit - 1)) % power(radix, digit);
        }

        return result;
    }

    /*! @brief 整数列の，各要素の2進表示のdigit桁目を並べた整数列を返す関数
        @param org_array 整数列（std::array）
        @param digit 抽出する桁
        @param from_top 左から数えるかどうか
        @return 整数列（std::bitset）
    */
    template <typename Numeric, size_t length>
    std::bitset <length> extract_bit(const std::array <Numeric, length> &org_array, const size_t digit, const bool from_top=false)
    {
        std::bitset <length> result = std::bitset <length>();
        if (digit == 0) {
            return result;
        }

        // ビットシフトは既存の領域を利用するためコピーしなければならない．
        for (size_t i = 0; i < length; i++) {
            Numeric buffer_for_bitshift = org_array[i];
            buffer_for_bitshift >>= (from_top) ? (8 * sizeof(Numeric) - digit) : (digit - 1);
            result[i]             = buffer_for_bitshift & 1;
        }

        return result;
    }

    /*! @brief 整数列の，各要素の2進表示のdigit桁目を並べた整数列を返す関数，の右辺値版
        @param org_array 整数列（std::array）．右辺値なのでin placeにビットシフトする
        @param digit 抽出する桁
        @param from_top 左から数えるかどうか
        @return 整数列（std::bitset）
    */
    template <typename Numeric, size_t length>
    std::bitset <length> extract_bit(std::array <Numeric, length> &&org_array, const size_t digit, const bool from_top=false)
    {
        std::bitset <length> result = std::bitset <length>();
        if (digit == 0) {
            return result;
        }

        // 右辺値の場合は破壊してよい．
        for (size_t i = 0; i < length; i++) {
            org_array[i] >>= (from_top) ? (8 * sizeof(Numeric) - digit) : (digit - 1);
            result[i]      = org_array[i] & 1;
        }

        return result;
    }

    /*! @brief 整数列の，各要素の2進表示のdigit桁目を並べた整数列を返す関数，のコンパイル時版
        @param org_array 整数列（sprout::array）
        @param digit 抽出する桁
        @param from_top 左から数えるかどうか
        @return 整数列（sprout::bitset）
    */
    template <typename Numeric, size_t length>
    constexpr sprout::bitset <length> extract_bit(const sprout::array <Numeric, length> &org_array, const size_t digit, const bool from_top=false)
    {
        sprout::bitset <length> result = std::bitset <length>();
        if (digit == 0) {
            return result;
        }

        // ビットシフトは既存の領域を利用するためコピーしなければならない．
        for (size_t i = 0; i < length; i++) {
            Numeric buffer_for_bitshift = org_array[i];
            buffer_for_bitshift >>= (from_top) ? (8 * sizeof(Numeric) - digit) : (digit - 1);
            result[i]             = buffer_for_bitshift & 1;
        }

        return result;
    }

    /*! @brief 整数列の，各要素の2進表示のdigit桁目を並べた整数列を返す関数，の右辺値，コンパイル版
        @param org_array 整数列（sprout::array）．右辺値なのでin placeにビットシフトする
        @param digit 抽出する桁
        @param from_top 左から数えるかどうか
        @return 整数列（sprout::bitset）
    */
    template <typename Numeric, size_t length>
    constexpr sprout::bitset <length> extract_bit(sprout::array <Numeric, length> &&org_array, const size_t digit, const bool from_top=false)
    {
        sprout::bitset <length> result = std::bitset <length>();
        if (digit == 0) {
            return result;
        }

        // 右辺値の場合は破壊してよい．
        for (size_t i = 0; i < length; i++) {
            org_array[i] >>= (from_top) ? (8 * sizeof(Numeric) - digit) : (digit - 1);
            result[i]      = org_array[i] & 1;
        }

        return result;
    }

}

#endif
