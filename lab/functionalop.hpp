/*! @file
    @brief 関数空間の上の演算子
    @auther yanteyon10
    @date 11/13
*/

#ifndef FUNCTIONALOP_HPP
#define FUNCTIONALOP_HPP

#include <functional>
#include <zero_one>

/*! @namespace util
    @brief 何も考えずラムダ式で実装すると深くなった時にセグフォする
*/
namespace util {
    /*! @brief 関数版+演算子のための関数ファンクタ
    */
    template <typename Result, typename ... Args>
    struct Add {
    private:
        const std::function <Result(Args ...)> f;
        const std::function <Result(Args ...)> g;
    public:
        Add(const std::function <Result(Args ...)> &f_, const std::function <Result(Args ...)> &g_)
            : f(f_), g(g_)
        {
        }

        Result operator()(Args ... args) const
        {
            return f(args ...) + g(args ...);
        }
    };

    /*! @brief 関数版二項-演算子のための関数ファンクタ
    */
    template <typename Result, typename ... Args>
    struct Sub {
    private:
        const std::function <Result(Args ...)> f;
        const std::function <Result(Args ...)> g;
    public:
        Sub(const std::function <Result(Args ...)> &f_, const std::function <Result(Args ...)> &g_)
            : f(f_), g(g_)
        {
        }

        Result operator()(Args ... args) const
        {
            return f(args ...) - g(args ...);
        }
    };

    /*! @brief 関数版単項-演算子のための関数ファンクタ
    */
    template <typename Result, typename ... Args>
    struct Opp {
    private:
        const std::function <Result(Args ...)> f;
    public:
        Opp(const std::function <Result(Args ...)> &f_)
            : f(f_)
        {
        }

        Result operator()(Args ... args) const
        {
            return -f(args ...);
        }
    };

    /*! @brief 関数版*演算子のための関数ファンクタ
    */
    template <typename Result, typename ... Args>
    struct Mult {
    private:
        const std::function <Result(Args ...)> f;
        const std::function <Result(Args ...)> g;
    public:
        Mult(const std::function <Result(Args ...)> &f_, const std::function <Result(Args ...)> &g_)
            : f(f_), g(g_)
        {
        }

        Result operator()(Args ... args) const
        {
            return f(args ...) * g(args ...);
        }
    };

    /*! @brief 関数版スカラー倍演算子のための関数ファンクタ
    */
    template <typename Result, typename ... Args>
    struct Scal_multi {
    private:
        const Result                           a;
        const std::function <Result(Args ...)> f;
    public:
        Scal_multi(const Result &a_, const std::function <Result(Args ...)> &f_)
            : a(a_), f(f_)
        {
        }

        Scal_multi(const std::function <Result(Args ...)> &f_, const Result &a_)
            : a(a_), f(f_)
        {
        }

        Result operator()(Args ... args) const
        {
            return a * f(args ...);
        }
    };

    /*! @brief 関数版/演算子のための関数ファンクタ．ゼロ除算チェックなし
    */
    template <typename Result, typename ... Args>
    struct Div {
    private:
        const std::function <Result(Args ...)> f;
        const std::function <Result(Args ...)> g;
    public:
        Div(const std::function <Result(Args ...)> &f_, const std::function <Result(Args ...)> &g_)
            : f(f_), g(g_)
        {
        }

        Result operator()(Args ... args) const
        {
            return f(args ...) / g(args ...);
        }
    };

    /*! @brief 関数版逆数ファンクタ．ゼロ除算チェックなし
    */
    template <typename Result, typename ... Args>
    struct Inv {
    private:
        const std::function <Result(Args ...)> f;
    public:
        Inv(const std::function <Result(Args ...)> &f_)
            : f(f_)
        {
        }

        Result operator()(Args ... args) const
        {
            return util::one <Result>() / f(args ...);
        }
    };

    /*! @brief 逆数関数をつくる関数
    */
    template <typename Result, typename ... Args>
    const std::function <Result(Args ...)> inverse(const std::function <Result(Args ...)> &f)
    {
        return std::function <Result(Args ...)>(Inv <Result, Args ...>(f));
    }

    /*! @brief 関数版スカラー除算演算子のための関数ファンクタ
    */
    template <typename Result, typename ... Args>
    struct Scal_div {
    private:
        const std::function <Result(Args ...)> f;
        const Result                           a;
    public:
        Scal_div(const std::function <Result(Args ...)> &f_, const Result &a_)
            : f(f_), a(a_)
        {
        }

        Result operator()(Args ... args) const
        {
            return f(args ...) / a;
        }
    };

    /*! @brief スカラー関数除算演算子のための関数ファンクタ
    */
    template <typename Result, typename ... Args>
    struct Div_scal {
    private:
        const std::function <Result(Args ...)> f;
        const Result                           a;
    public:
        Div_scal(const Result &a_, const std::function <Result(Args ...)> &f_)
            : a(a_), f(f_)
        {
        }

        Result operator()(Args ... args) const
        {
            return a / f(args ...);
        }
    };

    /*! @brief 定数関数をつくる関数ファンクタ．引数は捨てられる
    */
    template <typename Result, typename ... Args>
    struct Constant {
    private:
        const Result c;
    public:
        Constant(const Result &c_)
            : c(c_)
        {
        }

        Result operator()(Args ... args) const
        {
            return c;
        }
    };

    /*! @brief 定数関数をつくる関数
    */
    template <typename Result, typename ... Args>
    const std::function <Result(Args ...)> constant(const Result &c)
    {
        return std::function <Result(Args ...)>(Constant <Result, Args ...>(c));
    }
}

// 演算子はグローバル空間におく

/*! @brief 関数版+演算子
*/
template <typename Result, typename ... Args>
const std::function <Result(Args ...)> operator+(const std::function <Result(Args ...)> &f, const std::function <Result(Args ...)> &g)
{
    return std::function <Result(Args ...)>(util::Add <Result, Args ...>(f, g));
}

/*! @brief 関数版二項-演算子
*/
template <typename Result, typename ... Args>
const std::function <Result(Args ...)> operator-(const std::function <Result(Args ...)> &f, const std::function <Result(Args ...)> &g)
{
    return std::function <Result(Args ...)>(util::Sub <Result, Args ...>(f, g));
}

/*! @brief 関数版単項-演算子
*/
template <typename Result, typename ... Args>
const std::function <Result(Args ...)> operator-(const std::function <Result(Args ...)> &f)
{
    return std::function <Result(Args ...)>(util::Opp <Result, Args ...>(f));
}

/*! @brief 関数版*演算子
*/
template <typename Result, typename ... Args>
const std::function <Result(Args ...)> operator*(const std::function <Result(Args ...)> &f, const std::function <Result(Args ...)> &g)
{
    return std::function <Result(Args ...)>(util::Mult <Result, Args ...>(f, g));
}

/*! @brief 関数版スカラー倍演算子
*/
template <typename Result, typename ... Args>
const std::function <Result(Args ...)> operator*(const Result &a, const std::function <Result(Args ...)> &f)
{
    return std::function <Result(Args ...)>(util::Scal_multi <Result, Args ...>(a, f));
}

template <typename Result, typename ... Args>
const std::function <Result(Args ...)> operator*(const std::function <Result(Args ...)> &f, const Result &a)
{
    return std::function <Result(Args ...)>(util::Scal_multi <Result, Args ...>(f, a));
}

/*! @brief 関数版/演算子．ゼロ除算チェックなし
*/
template <typename Result, typename ... Args>
const std::function <Result(Args ...)> operator/(const std::function <Result(Args ...)> &f, const std::function <Result(Args ...)> &g)
{
    return std::function <Result(Args ...)>(util::Div <Result, Args ...>(f, g));
}

/*! @brief 関数版スカラー除算演算子
*/
template <typename Result, typename ... Args>
const std::function <Result(Args ...)> operator/(const std::function <Result(Args ...)> &f, const Result &a)
{
    return std::function <Result(Args ...)>(util::Scal_div <Result, Args ...>(f, a));
}

/*! @brief スカラー関数除算演算子
*/
template <typename Result, typename ... Args>
const std::function <Result(Args ...)> operator/(const Result &a, const std::function <Result(Args ...)> &f)
{
    return std::function <Result(Args ...)>(util::Div_scal <Result, Args ...>(a, f));
}

#ifdef INCLUDE_EIGEN

    #include <Eigen/Core>

    namespace util {
        /*! @brief 関数版スカラー倍演算子のための関数ファンクタ
        */
        template <typename Result, int m, int n, typename ... Args>
        struct Eigen_multi_scal {
        public:
            using value_type = Eigen::Matrix <Result, m, n>;
        private:
            const Eigen::Matrix <Result, m, n>     a;
            const std::function <Result(Args ...)> f;
        public:
            Eigen_multi_scal(const Eigen::Matrix <Result, m, n> &a_, const std::function <Result(Args ...)> &f_)
                : a(a_), f(f_)
            {
            }

            Eigen_multi_scal(const std::function <Result(Args ...)> &f_, const Eigen::Matrix <Result, m, n> &a_)
                : a(a_), f(f_)
            {
            }

            value_type operator()(Args ... args) const
            {
                return a * f(args ...);
            }
        };

        template <typename Result, int m, int n, typename ... Args>
        struct Scal_multi_eigen {
        public:
            using value_type = Eigen::Matrix <Result, m, n>;
        private:
            const Result                                                 a;
            const std::function <Eigen::Matrix <Result, m, n>(Args ...)> f;
        public:
            Scal_multi_eigen(const Result &a_, const std::function <Eigen::Matrix <Result, m, n>(Args ...)> &f_)
                : a(a_), f(f_)
            {
            }

            Scal_multi_eigen(const std::function <Eigen::Matrix <Result, m, n>(Args ...)> &f_, const Result &a_)
                : a(a_), f(f_)
            {
            }

            value_type operator()(Args ... args) const
            {
                return a * f(args ...);
            }
        };
    }

/*! @brief 関数版スカラー倍演算子
*/
    template <typename Result, int m, int n, typename ... Args>
    const std::function <Eigen::Matrix <Result, m, n>(Args ...)> operator*(const Eigen::Matrix <Result, m, n> &a, const std::function <Result(Args ...)> &f)
    {
        return std::function <Eigen::Matrix <Result, m, n>(Args ...)>(util::Eigen_multi_scal <Result, m, n, Args ...>(a, f));
    }

    template <typename Result, int m, int n, typename ... Args>
    const std::function <Eigen::Matrix <Result, m, n>(Args ...)> operator*(const std::function <Result(Args ...)> &f, const Eigen::Matrix <Result, m, n> &a)
    {
        return std::function <Eigen::Matrix <Result, m, n>(Args ...)>(util::Eigen_multi_scal <Result, m, n, Args ...>(f, a));
    }

    template <typename Result, int m, int n, typename ... Args>
    const std::function <Eigen::Matrix <Result, m, n>(Args ...)> operator*(const Result &a, const std::function <Eigen::Matrix <Result, m, n>(Args ...)> &f)
    {
        return std::function <Eigen::Matrix <Result, m, n>(Args ...)>(util::Scal_multi_eigen <Result, m, n, Args ...>(a, f));
    }

    template <typename Result, int m, int n, typename ... Args>
    const std::function <Eigen::Matrix <Result, m, n>(Args ...)> operator*(const std::function <Eigen::Matrix <Result, m, n>(Args ...)> &f, const Result &a)
    {
        return std::function <Eigen::Matrix <Result, m, n>(Args ...)>(util::Scal_multi_eigen <Result, m, n, Args ...>(f, a));
    }

#endif

#endif
