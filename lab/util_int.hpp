#ifndef UTIL_INT
#define UTIL_INT

#include <cstddef>
#include <cmath>
#include <sprout/assert.hpp>
#include <random>
#include <util>
#include <util_math>

namespace util_int {
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


    /*! @brief ユークリッドの互除法によって2数の絶対値の最大公約数を求める関数．少なくとも一方が0のとき1を返す
    */
    template <typename T, typename std::enable_if <std::is_integral <T>::value>::type * = nullptr>
    constexpr T gcd(const T& n, const T& m)
    {
        if (n == 0 || m == 0) {
            return 1;
        }
        T _n = std::abs(n);
        T _m = std::abs(m);
        if (_n < _m) {
            std::swap(_n, _m);
        }
        const size_t max_rep = _m;
        for (size_t i = 0; i < max_rep; i++) {
            // mは必ず小さくなるため高々m回の反復で終了する．
            int r = _n % _m;
            _n = _m;
            _m = r;
            if (_m == 0) {
                return _n;
            }
        }
        // 終了しなかった場合，エラー．
        SPROUT_ASSERT(false, "not finished.");

        return 0;
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
    std::ostream&operator<<(std::ostream &os, const PF <p> &pf);

    template <size_t p>
    constexpr PF <p> inverse(const PF <p> &pf);

    template <size_t p>
    constexpr PF <p> power(const PF <p> &base, int exponent);

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

        friend std::ostream & operator<< <>(std::ostream & os, const PF <p> &pf);

        // 同値関係
        constexpr bool operator==(const PF <p> &rhs) const
        {
            return n == rhs.n;
        }

        // 半順序関係
        constexpr bool operator<(const PF <p> &rhs) const
        {
            return n < rhs.n;
        }

        constexpr bool operator>(const PF <p> &rhs) const
        {
            return n > rhs.n;
        }

        // 加法
        constexpr PF <p> operator+(const PF <p> &rhs) const
        {
            return PF(n + rhs.n);
        }

        // 加法的逆元
        constexpr PF <p> operator-() const
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
        friend constexpr PF <p> inverse <>(const PF <p> &pf);

        // 除法
        constexpr PF <p> operator/(const PF <p> &rhs) const
        {
            return *this * inverse(rhs);
        }

        // べき乗
        friend constexpr PF <p> power <>(const PF <p> &base, int exponent);
    };

    template <size_t p>
    std::ostream&operator<<(std::ostream &os, const PF <p> &pf)
    {
        os << pf.n;

        return os;
    }

    template <size_t p>
    constexpr PF <p> inverse(const PF <p> &pf)
    {
        SPROUT_ASSERT(pf.n != 0, "zero has no inverse.");
        int unit  = 1;
        int upper = pf.n * (p - 1);
        for (; unit <= upper; unit += p) {
            if (unit % pf.n == 0) {
                return PF <p>(unit / pf.n);
            }
        }
        SPROUT_ASSERT(false, "no inverse.");

        return PF <p>(0);
    }

    template <size_t p>
    constexpr PF <p> power(const PF <p> &base, int exponent)
    {
        // base.nは必ず0 <= n < pを満たす．
        int result = base.n;
        for (int i = 1; i < exponent; i++) {
            result = (result * base.n) % p;
        }

        return result;
    }

    // ・power関数
    // 剰余環におけるpower．
    template <typename T, typename std::enable_if <std::is_integral <T>::value>::type * = nullptr>
    constexpr T power(T base, int exponent, int mod)
    {
        // base.nは必ず0 <= n < pを満たす．
        int result = base;
        for (int i = 1; i < exponent; i++) {
            result = (result * base) % mod;
        }

        return result;
    }

    // ・fermat_test関数
    // Fermat testによって素数判定を行う．
    bool fermat_test(int p, size_t n=10)
    {
        if (p <= 1) {
            return false;
        } else if (p == 2) {
            return true;
        }

        std::random_device               rnd;
        std::mt19937                     mt(rnd());
        std::uniform_int_distribution <> randint(2, p - 1);
        for (size_t i = 0; i < n; i++) {
            const int a = randint(mt);
            if (gcd(p, a) != 1 || power(a, p - 1, p) != 1) {
                return false;
            }
        }

        return true;
    }

    // ・fermat_test_p関数
    // Fermat testによって素数と判定される厳密確率を求める．
    double fermat_test_p(int p)
    {
        if (p <= 1) {
            return 0.;
        } else if (p == 2) {
            return 1.;
        }

        int counter = 0;
        for (int a = 2; a < p; a++) {
            if (gcd(p, a) == 1 && power(a, p - 1, p) == 1) {
                // 素数判定された回数を数える．
                counter++;
            }
        }
        return static_cast <double>(counter) / (p - 2);
    }
}

#endif
