#ifndef UTIL_INT
#define UTIL_INT

#include <cstddef>
#include <cmath>
#include <array>
#include <bitset>
#include <sprout/array.hpp>
#include <sprout/bitset.hpp>
#include "util_math.hpp"

namespace util {
    // ・mod関数
    // C++では整数除算を0に近い方向に丸めるため，剰余がずれる．
    // 例えば -2 % 5 = -2．
    // これを是正する関数．
    // 戻り値は必ずLHSより小さいので型はLHSに合わせる．
    template <typename T, typename S>
    constexpr T mod(const T n, const S m)
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
    class PF {
        static_assert(is_prime(p), "characteristic of Field must be prime.");

    private:
        int n;

    public:
        template <typename T>
        PF(T n_)
        {
            n = mod(n_, p);
        }

        std::ostream& operator<<(std::ostream& os);

        PF<p> operator+(const PF<p>& rhs)
        {
            return PF(n + rhs.n);
        }
    };
}

#endif
