/*! @file
    @brief std::functionのユーティリティクラス
    @auther yanteyon10
    @date 11/13
*/

#ifndef UTIL_FUNC_HPP
#define UTIL_FUNC_HPP

#include <functional>
#include <util>
#include <zero_one>

namespace util {
    /*! @brief 関数版+演算子のための関数ファンクタ
    */
    template <typename From, typename To>
    struct Add {
    private:
        const std::function<To(From)> f;
        const std::function<To(From)> g;
    public:
        Add(const std::function<To(From)>& f_, const std::function<To(From)>& g_)
        : f(f_), g(g_)
        {
        }
        To operator()(From x) const
        {
            return f(x) + g(x);
        }
    };

    /*! @brief 関数版二項-演算子のための関数ファンクタ
    */
    template <typename From, typename To>
    struct Sub {
    private:
        const std::function<To(From)> f;
        const std::function<To(From)> g;
    public:
        Sub(const std::function<To(From)>& f_, const std::function<To(From)>& g_)
        : f(f_), g(g_)
        {
        }
        To operator()(From x) const
        {
            return f(x) - g(x);
        }
    };

    /*! @brief 関数版単項-演算子のための関数ファンクタ
    */
    template <typename From, typename To>
    struct Opp {
    private:
        const std::function<To(From)> f;
    public:
        Opp(const std::function<To(From)>& f_)
        : f(f_)
        {
        }
        To operator()(From x) const
        {
            return - f(x);
        }
    };

    /*! @brief 関数版*演算子のための関数ファンクタ
    */
    template <typename From, typename To>
    struct Mult {
    private:
        const std::function<To(From)> f;
        const std::function<To(From)> g;
    public:
        Mult(const std::function<To(From)>& f_, const std::function<To(From)>& g_)
        : f(f_), g(g_)
        {
        }
        To operator()(From x) const
        {
            return f(x) * g(x);
        }
    };

    /*! @brief 関数版スカラー倍演算子のための関数ファンクタ
    */
    template <typename From, typename To>
    struct Scal_multi {
    private:
        const To a;
        const std::function<To(From)> f;
    public:
        Scal_multi(const To& a_, const std::function<To(From)>& f_)
        : a(a_), f(f_)
        {
        }
        Scal_multi(const std::function<To(From)>& f_, const To& a_)
        : a(a_), f(f_)
        {
        }
        To operator()(From x) const
        {
            return a * f(x);
        }
    };

    /*! @brief 関数版/演算子のための関数ファンクタ．ゼロ除算チェックなし
    */
    template <typename From, typename To>
    struct Div {
    private:
        const std::function<To(From)> f;
        const std::function<To(From)> g;
    public:
        Div(const std::function<To(From)>& f_, const std::function<To(From)>& g_)
        : f(f_), g(g_)
        {
        }
        To operator()(From x) const
        {
            return f(x) / g(x);
        }
    };

    /*! @brief 関数版逆数ファンクタ．ゼロ除算チェックなし
    */
    template <typename From, typename To>
    struct Inv {
    private:
        const std::function<To(From)> f;
    public:
        Inv(const std::function<To(From)>& f_)
        : f(f_)
        {
        }
        To operator()(From x) const
        {
            return util::one<To>() / f(x);
        }
    };

    /*! @brief 逆数関数をつくる関数
    */
    template <typename From, typename To>
    const std::function<To(From)> inverse(const std::function<To(From)>& f)
    {
        return std::function<To(From)>(Inv<From, To>(f));
    }

    /*! @brief 関数版スカラー除算演算子のための関数ファンクタ
    */
    template <typename From, typename To>
    struct Scal_div {
    private:
        const std::function<To(From)> f;
        const To a;
    public:
        Scal_div(const std::function<To(From)>& f_, const To& a_)
        : f(f_), a(a_)
        {
        }
        To operator()(From x) const
        {
            return f(x) / a;
        }
    };

    /*! @brief スカラー関数除算演算子のための関数ファンクタ
    */
    template <typename From, typename To>
    struct Div_scal {
    private:
        const std::function<To(From)> f;
        const To a;
    public:
        Div_scal(const To& a_, const std::function<To(From)>& f_)
        : a(a_), f(f_)
        {
        }
        To operator()(From x) const
        {
            return a / f(x);
        }
    };

    /*! @brief 定数関数をつくる関数ファンクタ
    */
    template <typename From, typename To>
    struct Constant {
    private:
        const To c;
    public:
        Constant(const To& c_)
        : c(c_)
        {
        }
        To operator()(From x) const
        {
            return c;
        }
    };

    /*! @brief 定数関数をつくる関数
    */
    template <typename From, typename To>
    const std::function<To(From)> constant(const To& c)
    {
        return std::function<To(From)>(Constant<From, To>(c));
    }
}

// 演算子はグローバル空間におく

/*! @brief 関数版+演算子
*/
template <typename From, typename To>
const std::function<To(From)> operator+(const std::function<To(From)>& f, const std::function<To(From)>& g)
{
    return std::function<To(From)>(util::Add<From, To>(f, g));
}

/*! @brief 関数版二項-演算子
*/
template <typename From, typename To>
const std::function<To(From)> operator-(const std::function<To(From)>& f, const std::function<To(From)>& g)
{
    return std::function<To(From)>(util::Sub<From, To>(f, g));
}

/*! @brief 関数版単項-演算子
*/
template <typename From, typename To>
const std::function<To(From)> operator-(const std::function<To(From)>& f)
{
    return std::function<To(From)>(util::Opp<From, To>(f));
}

/*! @brief 関数版*演算子
*/
template <typename From, typename To>
const std::function<To(From)> operator*(const std::function<To(From)>& f, const std::function<To(From)>& g)
{
    return std::function<To(From)>(util::Mult<From, To>(f, g));
}

/*! @brief 関数版スカラー倍演算子
*/
template <typename From, typename To>
const std::function<To(From)> operator*(const To& a, const std::function<To(From)>& f)
{
    return std::function<To(From)>(util::Scal_multi<From, To>(a, f));
}

template <typename From, typename To>
const std::function<To(From)> operator*(const std::function<To(From)>& f, const To& a)
{
    return std::function<To(From)>(util::Scal_multi<From, To>(f, a));
}

/*! @brief 関数版/演算子．ゼロ除算チェックなし
*/
template <typename From, typename To>
const std::function<To(From)> operator/(const std::function<To(From)>& f, const std::function<To(From)>& g)
{
    return std::function<To(From)>(util::Div<From, To>(f, g));
}

/*! @brief 関数版スカラー除算演算子
*/
template <typename From, typename To>
const std::function<To(From)> operator/(const std::function<To(From)>& f, const To& a)
{
    return std::function<To(From)>(util::Scal_div<From, To>(f, a));
}

/*! @brief スカラー関数除算演算子
*/
template <typename From, typename To>
const std::function<To(From)> operator/(const To& a, const std::function<To(From)>& f)
{
    return std::function<To(From)>(util::Div_scal<From, To>(a, f));
}

namespace util {
    void test_util_func()
    {
        const std::function<double(const double)> f = [](const double x){return x*x;};
        const std::function<double(const double)> g = [](const double x){return 2*x;};
        const std::function<double(const double)> h = [](const double x){return 1;};
        const std::function<double(const double)> hoge = f * g + h * 5.;
        _DISPLAY(hoge(-2.))
    }
}

#ifdef INCLUDE_EIGEN

#include <Eigen/Core>

namespace util {
    /*! @brief 関数版スカラー倍演算子のための関数ファンクタ
    */
    template <typename From, typename To, int m, int n>
    struct Eigen_multi_scal {
    public:
        using value_type = Eigen::Matrix<To, m, n>;
    private:
        const Eigen::Matrix<To, m, n> a;
        const std::function<To(From)> f;
    public:
        Eigen_multi_scal(const Eigen::Matrix<To, m, n>& a_, const std::function<To(From)>& f_)
        : a(a_), f(f_)
        {
        }
        Eigen_multi_scal(const std::function<To(From)>& f_, const Eigen::Matrix<To, m, n>& a_)
        : a(a_), f(f_)
        {
        }
        value_type operator()(From x) const
        {
            return a * f(x);
        }
    };

    template <typename From, typename To, int m, int n>
    struct Scal_multi_eigen {
    public:
        using value_type = Eigen::Matrix<To, m, n>;
    private:
        const To a;
        const std::function<Eigen::Matrix<To, m, n>(From)> f;
    public:
        Scal_multi_eigen(const To& a_, const std::function<Eigen::Matrix<To, m, n>(From)>& f_)
        : a(a_), f(f_)
        {
        }
        Scal_multi_eigen(const std::function<Eigen::Matrix<To, m, n>(From)>& f_, const To& a_)
        : a(a_), f(f_)
        {
        }
        value_type operator()(From x) const
        {
            return a * f(x);
        }
    };
}

/*! @brief 関数版スカラー倍演算子
*/
template <typename From, typename To, int m, int n>
const std::function<Eigen::Matrix<To, m, n>(From)> operator*(const Eigen::Matrix<To, m, n>& a, const std::function<To(From)>& f)
{
    return std::function<Eigen::Matrix<To, m, n>(From)>(util::Eigen_multi_scal<From, To, m, n>(a, f));
}

template <typename From, typename To, int m, int n>
const std::function<Eigen::Matrix<To, m, n>(From)> operator*(const std::function<To(From)>& f, const Eigen::Matrix<To, m, n>& a)
{
    return std::function<Eigen::Matrix<To, m, n>(From)>(util::Eigen_multi_scal<From, To, m, n>(f, a));
}

template <typename From, typename To, int m, int n>
const std::function<Eigen::Matrix<To, m, n>(From)> operator*(const To& a, const std::function<Eigen::Matrix<To, m, n>(From)>& f)
{
    return std::function<Eigen::Matrix<To, m, n>(From)>(util::Scal_multi_eigen<From, To, m, n>(a, f));
}

template <typename From, typename To, int m, int n>
const std::function<Eigen::Matrix<To, m, n>(From)> operator*(const std::function<Eigen::Matrix<To, m, n>(From)>& f, const To& a)
{
    return std::function<Eigen::Matrix<To, m, n>(From)>(util::Scal_multi_eigen<From, To, m, n>(f, a));
}

#endif

#endif
