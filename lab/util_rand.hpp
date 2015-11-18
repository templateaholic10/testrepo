/*! @file
    @brief std::randomのユーティリティクラス
    @auther yanteyon10
    @date 11/2
*/

#ifndef UTIL_RAND_HPP
#define UTIL_RAND_HPP

#include <random>
#include <type_traits>
#include <complex>

namespace util {
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
    };

    /*
    template <typename T>
    class Uniform <std::complex <T>, typename std::enable_if <std::is_integral <T>::value>::type> {
    public:
        using result_type = std::complex <T>;
    private:
        std::mt19937                      mt;
        std::uniform_int_distribution <T> rv_re;
        std::uniform_int_distribution <T> rv_im;
        const result_type                 a;
        const result_type                 b;
    public:
        Uniform(result_type a_=std::complex <T>(0, 0), result_type b_=std::complex <T>(std::numeric_limits <T>::max(), std::numeric_limits <T>::max()), std::random_device::result_type seed=std::random_device()())
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
    };
    */

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
        std::mt19937                                 mt;
        std::normal_distribution <result_type> rv;
        const result_type                            mu;
        const result_type                            sigma;
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
    };
}

#endif
