/*! @file
    @brief std::randomのユーティリティクラス
    @auther yanteyon10
    @date 11/2
*/

#include <random>
#include <type_traits>

namespace util {
    /*! @class
        @brief std::uniform_int_distributionとstd::uniform_real_distributionのラッパ関数オブジェクト．メルセンヌツイスタを使用して生成を行う
        @tparam T intまたはdouble
    */
    template <typename T, class Ignored = void>
    class Uniform;

    template <typename T>
    class Uniform<T, typename std::enable_if<std::is_integral<T>::value>::type> {
    public:
        using result_type = T;
    private:
        std::mt19937 mt;
        std::uniform_int_distribution<result_type> rv;
        const result_type a;
        const result_type b;
    public:
        Uniform(result_type a_ = 0, result_type b_ = std::numeric_limits<result_type>::max(), std::random_device::result_type seed = std::random_device()())
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
    class Uniform<T, typename std::enable_if<std::is_floating_point<T>::value>::type> {
    public:
        using result_type = T;
    private:
        std::mt19937 mt;
        std::uniform_real_distribution<result_type> rv;
        const result_type a;
        const result_type b;
    public:
        Uniform(result_type a_ = 0., result_type b_ = 1., std::random_device::result_type seed = std::random_device()())
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
}
