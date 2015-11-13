/*! @file
    @brief std::functionのユーティリティクラス
    @auther yanteyon10
    @date 11/13
*/

#ifndef UTIL_FUNC_HPP
#define UTIL_FUNC_HPP

#include <functional>
#include <util>

namespace util {
    /*! @brief 関数版+演算子のための関数ファンクタ
    */
    template <typename From, typename To>
    struct add {
    private:
        const std::function<To(From)> f;
        const std::function<To(From)> g;
    public:
        add(const std::function<To(From)>& f_, const std::function<To(From)>& g_)
        : f(f_), g(g_)
        {
        }
        To operator()(From x)
        {
            return f(x) + g(x);
        }
    };

    /*! @brief 関数版+演算子
    */
    template <typename From, typename To>
    const std::function<To(From)> operator+(const std::function<To(From)>& f, const std::function<To(From)>& g)
    {
        return std::function<To(From)>(add<From, To>(f, g));
    }

    // template <typename From, typename To>
    // const std::function<To(const From&)> operator+(const std::function<To(const From&)>& f, const std::function<To(const From&)>& g)
    // {
    //     _HERE
    //     return [&](const From& from)->To{return f(from) + g(from);};
    // }
    // template <typename From, typename To>
    // const std::function<To(const From&)> operator+(const std::function<To(const From&)>& f, std::function<To(const From&)>&& g)
    // {
    //     _HERE
    //     return [&](const From& from)->To{return f(from) + g(from);};
    // }
    // template <typename From, typename To>
    // const std::function<To(const From&)> operator+(std::function<To(const From&)>&& f, const std::function<To(const From&)>& g)
    // {
    //     _HERE
    //     return [&](const From& from)->To{return f(from) + g(from);};
    // }
    // template <typename From, typename To>
    // const std::function<To(const From&)> operator+(std::function<To(const From&)>&& f, std::function<To(const From&)>&& g)
    // {
    //     _HERE
    //     return [&](const From& from)->To{return f(from) + g(from);};
    // }

    void test_util_func()
    {
        const std::function<double(const double)> f = [](const double x){return x*x;};
        const std::function<double(const double)> g = [](const double x){return 2*x;};
        const std::function<double(const double)> h = [](const double x){return 1;};
        const std::function<double(const double)> f_g_h = f + g + h;
        _DISPLAY(f_g_h(1.))
    }
}

#endif
