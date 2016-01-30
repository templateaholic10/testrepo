/*! @file
    @brief std::randomのユーティリティクラス
    @auther yanteyon10
    @date 11/2
*/

#ifndef EXRANDOM_HPP
#define EXRANDOM_HPP

#include <random>
#include <type_traits>
#include <complex>
#include <cmath>

namespace std {
    /*! @class
        @brief std::bernoulli_distributionのラッパ関数オブジェクト．メルセンヌツイスタを使用して生成を行う
    */
    class Bernoulli {
    public:
        using result_type = std::bernoulli_distribution::result_type;
        using param_type = double;
    private:
        std::mt19937                                mt;
        std::bernoulli_distribution rv;
        const param_type p;
    public:
        Bernoulli(param_type p_=0.5, std::random_device::result_type seed=std::random_device()())
            : p(p_), mt(seed), rv(p_)
        {
        }

        result_type operator()()
        {
            return rv(mt);
        }

        result_type min() const
        {
            return false;
        }

        result_type max() const
        {
            return true;
        }
    };

    /*! @class
        @brief std::uniform_int_distributionとstd::uniform_real_distributionのラッパ関数オブジェクト．メルセンヌツイスタを使用して生成を行う
        @tparam T 数値型またはstd::complex
    */
    template <typename T, class Ignored = void>
    class Uniform;

    template <typename T>
    class Uniform <T, typename std::enable_if <std::is_integral <T>::value>::type> {
    public:
        using result_type = T;
    private:
        std::mt19937                                mt;
        std::uniform_int_distribution <result_type> rv;
        const result_type                           a;
        const result_type                           b;
    public:
        Uniform(result_type a_=0, result_type b_=std::numeric_limits <result_type>::max(), std::random_device::result_type seed=std::random_device()())
            : a(a_), b(b_), mt(seed), rv(a_, b_)
        {
        }

        result_type operator()()
        {
            return rv(mt);
        }

        result_type min() const
        {
            return a;
        }

        result_type max() const
        {
            return b;
        }

        static result_type default_min()
        {
            return 0;
        }

        static result_type default_max()
        {
            return std::numeric_limits <result_type>::max();
        }
    };

    template <typename T>
    class Uniform <T, typename std::enable_if <std::is_floating_point <T>::value>::type> {
    public:
        using result_type = T;
    private:
        std::mt19937                                 mt;
        std::uniform_real_distribution <result_type> rv;
        const result_type                            a;
        const result_type                            b;
    public:
        Uniform(result_type a_=0., result_type b_=1., std::random_device::result_type seed=std::random_device()())
            : a(a_), b(b_), mt(seed), rv(a_, b_)
        {
        }

        result_type operator()()
        {
            return rv(mt);
        }

        result_type min() const
        {
            return a;
        }

        result_type max() const
        {
            return b;
        }

        static result_type default_min()
        {
            return 0.;
        }

        static result_type default_max()
        {
            return 1.;
        }
    };

    template <typename T>
    class Uniform <std::complex <T>, typename std::enable_if <std::is_floating_point <T>::value>::type> {
    public:
        using result_type = std::complex <T>;
    private:
        std::mt19937                       mt;
        std::uniform_real_distribution <T> rv_re;
        std::uniform_real_distribution <T> rv_im;
        const result_type                  a;
        const result_type                  b;
    public:
        Uniform(result_type a_=std::complex <T>(0., 0.), result_type b_=std::complex <T>(1., 1.), std::random_device::result_type seed=std::random_device()())
            : a(a_), b(b_), mt(seed), rv_re(a_.real(), b_.real()), rv_im(a_.imag(), b_.imag())
        {
        }

        result_type operator()()
        {
            return std::complex <T>(rv_re(mt), rv_im(mt));
        }

        result_type min() const
        {
            return a;
        }

        result_type max() const
        {
            return b;
        }

        static result_type default_min()
        {
            return std::complex <T>(0., 0.);
        }

        static result_type default_max()
        {
            return std::complex <T>(1., 1.);
        }
    };

    /*! @class
        @brief std::normal_distributionのラッパ関数オブジェクト．メルセンヌツイスタを使用して生成を行う
        @tparam T double
    */
    template <typename T, class Ignored = void>
    class Gaussian;

    template <typename T>
    class Gaussian <T, typename std::enable_if <std::is_floating_point <T>::value>::type> {
    public:
        using result_type = T;
    private:
        std::mt19937                           mt;
        std::normal_distribution <result_type> rv;
        const result_type                      mu;
        const result_type                      sigma;
    public:
        Gaussian(result_type mu_=0., result_type sigma_=1., std::random_device::result_type seed=std::random_device()())
            : mu(mu_), sigma(sigma_), mt(seed), rv(mu_, sigma_)
        {
        }

        result_type operator()()
        {
            return rv(mt);
        }

        result_type mean() const
        {
            return mu;
        }

        result_type stddev() const
        {
            return sigma;
        }

        result_type variance() const
        {
            return sigma * sigma;
        }
    };

    /*! @brief 1変量複素正規分布．ただしcircularly symmetricな場合
    */
    template <typename T>
    class Gaussian <std::complex<T>, typename std::enable_if <std::is_floating_point <T>::value>::type> {
    public:
        using result_type = std::complex<T>;
        using real_type = T;
    private:
        std::mt19937                           mt;
        std::normal_distribution <real_type> rv_re;
        std::normal_distribution <real_type> rv_im;
        const result_type                      mu;
        const result_type                      sigma; // 複素数型だが，実際は実正数
    public:
        Gaussian(result_type mu_=result_type(0., 0.), result_type sigma_=result_type(1., 0.), std::random_device::result_type seed=std::random_device()())
            : mu(mu_), sigma(sigma_), mt(seed), rv_re(mu_.real(), sigma_.real()/std::sqrt(2.)), rv_im(mu_.imag(), sigma_.real()/std::sqrt(2.))
        {
        }

        result_type operator()()
        {
            return result_type(rv_re(mt), rv_im(mt));
        }

        result_type mean() const
        {
            return mu;
        }

        result_type stddev() const
        {
            return sigma;
        }

        result_type variance() const
        {
            return std::norm(sigma);
        }
    };

    /*! @class
        @brief std::chi_squared_distributionのラッパ関数オブジェクト．メルセンヌツイスタを使用して生成を行う
        @tparam T double
    */
    template <typename T, class Ignored = void>
    class Chisq;

    template <typename T>
    class Chisq <T, typename std::enable_if <std::is_floating_point <T>::value>::type> {
    public:
        using result_type = T;
    private:
        std::mt19937                           mt;
        std::chi_squared_distribution <result_type> rv;
        const result_type                      n;
    public:
        Chisq(result_type n_=1.0, std::random_device::result_type seed=std::random_device()())
            : n(n_), mt(seed), rv(n_)
        {
        }

        result_type operator()()
        {
            return rv(mt);
        }

        result_type df() const  // degrees of freedom
        {
            return n;
        }
    };
}

#endif
