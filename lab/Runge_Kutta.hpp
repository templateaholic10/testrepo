/*! @file
    @brief Runge_Kutta法により初期値問題を解くクラス
    @author templateaholic10
    @date 1/6
*/
#ifndef RUNGE_KUTTA_HPP
#define RUNGE_KUTTA_HPP

#include <cassert>
#include <iterative>
#include <exeigen>
#include <eigen_io>
#include <debug>

namespace {
    constexpr int default_length = 10000;
    constexpr char default_delim = ',';
    constexpr int default_max_rep = 10000;
}

namespace ODE {
    enum class Options
    {
        Runge,
        Kutta,
    };

    template <typename T_type, typename X_type, int dim = 4, Options options = Options::Runge, int length = default_length>
    class Runge_Kutta;

    template <typename T_type, typename X_scalar_type, int p, int length>
    class Runge_Kutta<T_type, Eigen::Vector<X_scalar_type, p>, 4, Options::Runge, length> : public optimization::Iterative <Runge_Kutta<T_type, Eigen::Vector<X_scalar_type, p>, 4, Options::Runge, length>, length, Eigen::Vector <X_scalar_type, p> > {
    public:
        static constexpr int dim = 4;
        static constexpr Options options = Options::Runge;
        using t_type = T_type;
        using x_type = Eigen::Vector<X_scalar_type, p>;
        using f_type = std::function<x_type(const t_type&, const x_type&)>;
        using type = Runge_Kutta<t_type, x_type, dim, options, length>;
        using base_type = optimization::Iterative<type, length, x_type>;
    protected:
        const f_type f;
        const t_type t_0;
        const t_type t_1;
        t_type t;
        const int n;
        const t_type h;
    public:
        /*! @brief Runge_Kuttaクラスのコンストラクタ
            @param f x'(t) = f(t, x(t))のf
            @param x_0 初期値x(t_0)
            @param t_0 開始時刻
            @param t_1 終了時刻
            @param n メッシュ数
            @param os xの出力ストリーム
            @param delim デリミタ
            @param max_rep 最大反復回数
        */
        Runge_Kutta(const f_type &f_, const x_type &x_0_, const t_type &t_0_, const t_type &t_1_, const int n_, std::ostream &os=std::cout, const char delim_=default_delim, const int max_rep_=default_max_rep)
            : base_type(1, os, delim_, max_rep_), f(f_), t_0(t_0_), t_1(t_1_), n(n_), h((t_1_ - t_0_) / n_)
        {
            init(x_0_);
        }

        void init(const x_type &x_0);
        void forward();
        void record(const int k) const;
        bool stop_cond() const;
    };

    /*! @brief 初期化する関数
    */
    template <typename T_type, typename X_scalar_type, int p, int length>
    void Runge_Kutta<T_type, Eigen::Vector<X_scalar_type, p>, 4, Options::Runge, length>::init(const x_type &x_0)
    {
        base_type &base = static_cast <base_type &>(*this);
        base.x[0] = x_0;
        t = t_0;
    }

    /*! @brief 1ステップごとの出力関数
    */
    template <typename T_type, typename X_scalar_type, int p, int length>
    void Runge_Kutta<T_type, Eigen::Vector<X_scalar_type, p>, 4, Options::Runge, length>::record(const int k) const
    {
        const base_type &base = static_cast <const base_type &>(*this);
        out(base.os, base.x[k], false, base.delim);
        base.os << std::endl;
    }

    /*! @brief 漸化式を進める関数
    */
    template <typename T_type, typename X_scalar_type, int p, int length>
    void Runge_Kutta<T_type, Eigen::Vector<X_scalar_type, p>, 4, Options::Runge, length>::forward()
    {
        base_type &base = static_cast <base_type &>(*this);

        const x_type &x_old = base.x[base.step-1];
        x_type &x_new = base.x[base.step];

        const x_type K_1 = f(t, x_old);
        const x_type K_2 = f(t+0.5*h, x_old+0.5*h*K_1);
        const x_type K_3 = f(t+0.5*h, x_old+0.5*h*K_2);
        const x_type K_4 = f(t+h, x_old+h*K_3);

        x_new = x_old + h*(K_1/6.+K_2/3.+K_3/3.+K_4/6.);

        t += h;
    }

    /*! @brief 収束判定する関数
    */
    template <typename T_type, typename X_scalar_type, int p, int length>
    bool Runge_Kutta<T_type, Eigen::Vector<X_scalar_type, p>, 4, Options::Runge, length>::stop_cond() const
    {
        const T_type epsilon = 1.e-6;
        return (t >= t_1 - epsilon);
    }

    template <typename T_type, typename X_scalar_type, int p, int length>
    class Runge_Kutta<T_type, Eigen::Vector<X_scalar_type, p>, 4, Options::Kutta, length> : public optimization::Iterative <Runge_Kutta<T_type, Eigen::Vector<X_scalar_type, p>, 4, Options::Kutta, length>, length, Eigen::Vector <X_scalar_type, p> > {
    public:
        static constexpr int dim = 4;
        static constexpr Options options = Options::Kutta;
        using t_type = T_type;
        using x_type = Eigen::Vector<X_scalar_type, p>;
        using f_type = std::function<x_type(const t_type&, const x_type&)>;
        using type = Runge_Kutta<t_type, x_type, dim, options, length>;
        using base_type = optimization::Iterative<type, length, x_type>;
    protected:
        const f_type f;
        const t_type t_0;
        const t_type t_1;
        t_type t;
        const int n;
        const t_type h;
    public:
        /*! @brief Runge_Kuttaクラスのコンストラクタ
            @param f x'(t) = f(t, x(t))のf
            @param x_0 初期値x(t_0)
            @param t_0 開始時刻
            @param t_1 終了時刻
            @param n メッシュ数
            @param os xの出力ストリーム
            @param delim デリミタ
            @param max_rep 最大反復回数
        */
        Runge_Kutta(const f_type &f_, const x_type &x_0_, const t_type &t_0_, const t_type &t_1_, const int n_, std::ostream &os=std::cout, const char delim_=default_delim, const int max_rep_=default_max_rep)
            : base_type(1, os, delim_, max_rep_), f(f_), t_0(t_0_), t_1(t_1_), n(n_), h((t_1_ - t_0_) / n_)
        {
            init(x_0_);
        }

        void init(const x_type &x_0);
        void forward();
        void record(const int k) const;
        bool stop_cond() const;
    };

    /*! @brief 初期化する関数
    */
    template <typename T_type, typename X_scalar_type, int p, int length>
    void Runge_Kutta<T_type, Eigen::Vector<X_scalar_type, p>, 4, Options::Kutta, length>::init(const x_type &x_0)
    {
        base_type &base = static_cast <base_type &>(*this);
        base.x[0] = x_0;
        t = t_0;
    }

    /*! @brief 1ステップごとの出力関数
    */
    template <typename T_type, typename X_scalar_type, int p, int length>
    void Runge_Kutta<T_type, Eigen::Vector<X_scalar_type, p>, 4, Options::Kutta, length>::record(const int k) const
    {
        const base_type &base = static_cast <const base_type &>(*this);
        out(base.os, base.x[k], false, base.delim);
        base.os << std::endl;
    }

    /*! @brief 漸化式を進める関数
    */
    template <typename T_type, typename X_scalar_type, int p, int length>
    void Runge_Kutta<T_type, Eigen::Vector<X_scalar_type, p>, 4, Options::Kutta, length>::forward()
    {
        base_type &base = static_cast <base_type &>(*this);

        const x_type &x_old = base.x[base.step-1];
        x_type &x_new = base.x[base.step];

        const x_type K_1 = f(t, x_old);
        const x_type K_2 = f(t+h/3., x_old+h*K_1/3.);
        const x_type K_3 = f(t+2.*h/3., x_old+h*K_2-h*K_1/3.);
        const x_type K_4 = f(t+h, x_old+h*K_3-h*K_2+h*K_1);

        x_new = x_old + h*(K_1/8.+3.*K_2/8.+3.*K_3/8.+K_4/8.);

        t += h;
    }

    /*! @brief 収束判定する関数
    */
    template <typename T_type, typename X_scalar_type, int p, int length>
    bool Runge_Kutta<T_type, Eigen::Vector<X_scalar_type, p>, 4, Options::Kutta, length>::stop_cond() const
    {
        const T_type epsilon = 1.e-6;
        return (t >= t_1 - epsilon);
    }
}

#endif
