/*! @file
    @brief 数学関係のユーティリティ関数群
*/
#ifndef CONSTEXPR_CMATH_HPP
#define CONSTEXPR_CMATH_HPP

#ifndef CMATH_EPSILON
#define CMATH_EPSILON 1e-6
#endif

#ifndef CMATH_MAX_REP
#define CMATH_MAX_REP 10000
#endif

namespace cpstd {
    /*! @brief コンパイル時最小値関数
    */
    template <typename T>
    constexpr T min(const T &lhs, const T &rhs)
    {
        return (lhs < rhs) ? lhs : rhs;
    }

    /*! @brief コンパイル時最大値関数
    */
    template <typename T>
    constexpr T max(const T &lhs, const T &rhs)
    {
        return (lhs > rhs) ? lhs : rhs;
    }

    /*! @brief コンパイル時絶対値関数
        @param d 実数
        @returnn 絶対値
    */
    template <typename Numeric>
    constexpr Numeric abs(const Numeric d)
    {
        return (d > static_cast<Numeric>(0)) ? d : -d;
    }

    /*! @brief コンパイル時べき乗関数
        @param base 基数
        @param exponent 指数
        @returnn べき乗
    */
    template <typename Numeric>
    constexpr Numeric power(const Numeric base, int exponent)
    {
        Numeric result = static_cast<Numeric>(1);
        for (int i = 0; i < exponent; i++) {
            result *= base;
        }

        return result;
    }

    /*! @brief コンパイル時2進対数関数
        @param n 真数
        @return 2進対数
    */
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

    /*! @brief コンパイル時床関数
        @param d 実数
        @return dを超えない最大の整数（int）
    */
    template <typename Numeric>
    constexpr int floor(const Numeric d)
    {
        if (d > 0) {
            // 正の場合
            return static_cast <int>(d);
        } else {
            // 負の場合
            return static_cast <int>(d + CMATH_EPSILON) - 1;
        }
    }

    template <typename Result, typename Numeric>
    constexpr Result floor(const Numeric d)
    {
        return static_cast<Result>(floor(d));
    }

    /*! @brief コンパイル時天井関数
        @param d 実数
        @return d以上の最小の整数（int）
    */
    template <typename Numeric>
    constexpr int ceil(const Numeric d)
    {
        if (d > 0) {
            // 正の場合
            return static_cast <int>(d - CMATH_EPSILON) + 1;
        } else {
            // 負の場合
            return static_cast <int>(d);
        }
    }

    template <typename Result, typename Numeric>
    constexpr Result ceil(const Numeric d)
    {
        return static_cast<Result>(ceil(d));
    }

    /*! @brief コンパイル時四捨五入関数
        @param d 実数
        @return dを四捨五入した整数（int）
    */
    template <typename Numeric>
    constexpr int round(const Numeric d)
    {
        if (d > 0) {
            // 正の場合
            return static_cast <int>(d + 0.5);
        } else {
            // 負の場合
            return static_cast <int>(d - 0.5);
        }
    }

    template <typename Result, typename Numeric>
    constexpr Result round(const Numeric d)
    {
        return static_cast<Result>(round(d));
    }

    /*! @brief コンパイル時平方根関数．Newton法による実装
        @param d 非負実数
        @return dの平方根
    */
    template <typename Numeric>
    constexpr double sqrt(const Numeric d)
    {
        if (d < 0) {
            return 0.;
        }
        // 初期値．
        double x = d;
        for (int i = 0; i < CMATH_MAX_REP; i++) {
            if (x*x - d < CMATH_EPSILON) {
                break;
            }
            x = (x*x + d) / x * 0.5;
        }
        return x;
    }

    template <typename Result, typename Numeric>
    constexpr Result sqrt(const Numeric d)
    {
        return static_cast<Result>(sqrt(d));
    }
}

#endif
