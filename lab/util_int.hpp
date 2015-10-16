#ifndef UTIL_INT
#define UTIL_INT

#include <cstddef>
#include <cmath>
#include <sprout/assert.hpp>
#include "util.hpp"
#include "util_math.hpp"

namespace util {
    // ・mod関数
    // C++では整数除算を0に近い方向に丸めるため，剰余がずれる．
    // 例えば -2 % 5 = -2．
    // これを是正する関数．
    // 戻り値は必ずLHSより小さいので型はLHSに合わせる．
    // nがint，mがunsigned longのとき汎整数拡張でまずいことになるのでmはintに固定．
    template <typename T>
    constexpr T mod(const T n, const int m)
    {
        return n % m + (n < 0 ? m : 0);
    }

    // ・is_prime関数
    // 厳密な素数判定を行う．
    template <typename T>
    constexpr bool is_prime(const T p)
    {
        if (p <= 1) {
            return false;
        }
        size_t n = util::floor(util::sqrt(p));
        for (size_t i = 2; i <= n; i++) {
            if (p % i == 0) {
                return false;
            }
        }
        return true;
    }

    // ・PFテンプレートクラス
    // 有限素体（limited Prime Field）を実装するテンプレートクラス．
    // 標数はそう大きくならないと考えられるので内部的にはintで保持する．
    template <size_t p>
    class PF;
    template <size_t p>
    std::ostream &operator<<(std::ostream &os, const PF<p>& pf);
    template <size_t p>
    constexpr PF<p> inverse(const PF<p>& pf);
    template <size_t p>
    constexpr PF <p> power(const PF<p>& base, int exponent);

    template <size_t p>
    class PF {
        static_assert(is_prime(p), "characteristic of Field must be prime.");
    private:
        int n;
    public:
        template <typename T>
        constexpr PF(T n_)
        : n(mod(n_, p))
        {
        }

        friend std::ostream &operator<<<>(std::ostream &os, const PF<p>& pf);

        // 同値関係
        constexpr bool operator==(const PF<p> &rhs) const
        {
            return n == rhs.n;
        }

        // 半順序関係
        constexpr bool operator<(const PF<p> &rhs) const
        {
            return n < rhs.n;
        }
        constexpr bool operator>(const PF<p> &rhs) const
        {
            return n > rhs.n;
        }

        // 加法
        constexpr PF <p> operator+(const PF <p> &rhs) const
        {
            return PF(n + rhs.n);
        }

        // 加法的逆元
        constexpr PF<p> operator-() const
        {
            return PF(-n);
        }

        // 減法
        constexpr PF <p> operator-(const PF <p> &rhs) const
        {
            return PF(n - rhs.n);
        }

        // 乗法
        constexpr PF <p> operator*(const PF <p> &rhs) const
        {
            return PF(n * rhs.n);
        }

        // 乗法的逆元
        friend constexpr PF<p> inverse<>(const PF<p>& pf);

        // 除法
        constexpr PF <p> operator/(const PF <p> &rhs) const
        {
            return *this * inverse(rhs);
        }

        // べき乗
        friend constexpr PF <p> power<>(const PF<p>& base, int exponent);
    };

    template <size_t p>
    std::ostream& operator<<(std::ostream& os, const PF<p>& pf)
    {
        os << pf.n;
        return os;
    }

    template <size_t p>
    constexpr PF<p> inverse(const PF<p>& pf)
    {
        SPROUT_ASSERT(pf.n != 0, "zero has no inverse.");
        int unit = 1;
        int upper = pf.n * (p-1);
        for (; unit <= upper; unit += p) {
            if (unit % pf.n == 0) {
                return PF<p>(unit / pf.n);
            }
        }
        SPROUT_ASSERT(false, "no inverse.");
        return PF<p>(0);
    }

    template <size_t p>
    constexpr PF<p> power(const PF<p>& base, int exponent)
    {
        // base.nは必ず0 <= n < pを満たす．
        int result = base.n;
        for (int i = 1; i < exponent; i++) {
            result = (result * base.n) % p;
        }
        return result;
    }
}

#endif
