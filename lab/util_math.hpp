#ifndef UTIL_MATH
#define UTIL_MATH

#include <cstddef>
#include <array>
#include <bitset>
#include <sprout/array.hpp>
#include <sprout/bitset.hpp>

namespace util {
    // ・epsilon
    constexpr double epsilon = 10e-6;

    // ・power関数
    template <typename T>
    constexpr T power(const T base, int exponent)
    {
        T result = base;
        for (int i = 1; i < exponent; i++) {
            result *= base;
        }

        return result;
    }

    // ・lg関数
    // ビット長を求める．
    template <typename Numeric>
    constexpr Numeric lg(const Numeric n)
    {
        Numeric _n  = n;
        Numeric lgn = 0;
        while (_n) {
            lgn++;
            _n >>= 1;
        }

        return lgn;
    }

    // ・floor関数
    constexpr int floor(const double d)
    {
        if (d > 0) {
            // 正の場合
            return static_cast <int>(d);
        } else {
            // 負の場合
            return static_cast <int>(d + epsilon) - 1;
        }
    }

    // ・ceil関数
    constexpr int ceil(const double d)
    {
        if (d > 0) {
            // 正の場合
            return static_cast <int>(d - epsilon) + 1;
        } else {
            // 負の場合
            return static_cast <int>(d);
        }
    }

    // ・round関数
    constexpr int round(const double d)
    {
        if (d > 0) {
            // 正の場合
            return static_cast <int>(d + 0.5);
        } else {
            // 負の場合
            return static_cast <int>(d - 0.5);
        }
    }

    // ・sqrt関数
    // Newton法で求める．
    constexpr int sqrt(const double d)
    {
        // 初期値．
        double x = d;
        while (x*x - d > epsilon) {
            x = (x*x + d) / x * 0.5;
        }
        return x;
    }

    // ・extract_digit関数
    // 整数列のn桁目を抽出して整数列を作る．
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

    // ・extract_bit関数
    // extract_digit関数の2進数版．
    // テンプレート関数を部分特殊化させろ（おこ）
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
