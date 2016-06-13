/*! @file
    @brief 修正Newton-Raphson法により非線形最適化問題を解くクラス
    @author templateaholic10
    @data 1/24
*/
#ifndef NEWTON_RAPHSON2_HPP
#define NEWTON_RAPHSON2_HPP

#include <functional>
#include <exeigen>
#include <eigen_io>

/*! @macro
    @brief 毎ステップのXを出力するかどうか
*/
#ifndef LOGGINGX
#define LOGGINGX 1
#endif

/*! @macro
    @brief 毎ステップの誤差を出力するかどうか
*/
#ifndef LOGGINGERR
#define LOGGINGERR 0
#endif

// このほか，OFFSTREAMをマクロ定義することにより出力をオフにできる

namespace equation {
    /*! @class
        @brief 修正Newton-Raphson法クラスの本体
        @tparam Func 関数の型
    */
    template <typename Func>
    class Newton_Raphson2;

    template <typename T, int n, int m>
    class Newton_Raphson2 <std::function<Eigen::Vector<T, m>(const Eigen::Vector<T, n> &)>>
    {
    public:
        // 記法
        using scalar_type = T;
        static constexpr int cp_var_dim = n;
        static constexpr int cp_eq_dim = m;
        using var_type = Eigen::Vector<scalar_type, cp_var_dim>;
        using eq_type = Eigen::Vector<scalar_type, cp_eq_dim>;
        using matrix_type = Eigen::Matrix<scalar_type, cp_eq_dim, cp_var_dim>;
        using func_type = std::function<eq_type(const var_type &)>;
        using Jacobi_type = std::function<matrix_type(const var_type &)>;
        using type = Newton_Raphson2<func_type>;
    protected:
        const int var_dim;  // 変数の次元
        const int eq_dim;   // 方程式の次元
        const func_type f;  // 零点を求めたい関数
        const Jacobi_type J;    // ヤコビ行列
        const var_type x_0; // 初期解
        const matrix_type J_0;  // 初期解におけるヤコビ行列
        const Eigen::Transpose_t<matrix_type> J_0_oinv; // 初期解におけるヤコビ行列のouter inverse
        bool success;   // 正常終了フラグ
        int _step;   // ステップ数
    public:
        static double epsilon;  // 非常に小さい数
        static char delim;  // デリミタ
        static int max_rep; // 最大反復回数
    public:
        /*! @brief コンストラクタ
        */
        Newton_Raphson2(const func_type &f_, const Jacobi_type &J_, const int var_dim_=n, const int eq_dim_=m)
        : var_dim(var_dim_), eq_dim(eq_dim_), f(f_), J(J_), success(true), _step(0)
        {
        }

        /*! @brief 解く
        */
        var_type solve(std::ostream &os=std::cout, const var_type &x_0=var_type::Ones())
        {
            const matrix_type J_0 = J(x_0);
            const Eigen::Transpose_t<matrix_type> J_0_oinv = Eigen::pinverse(J_0);
            var_type x = x_0;
            _step = 0;

            for (size_t i = 0; i < max_rep; i++) {
                _step++;

                #ifndef OFFSTREAM
                #if LOGGINGX
                out(os, x, false, delim);
                os << std::endl;
                #endif
                #endif
                const eq_type eq = J_0_oinv * f(x);

                // 停止条件
                const scalar_type err = eq.norm();
                #ifndef OFFSTREAM
                #if LOGGINGERR
                os << err << std::endl;
                #endif
                #endif
                if (err < epsilon) {
                    success = true;
                    return x;
                }

                // 漸化式の更新
                x = x + (matrix_type::Identity(var_dim, var_dim) + J_0_oinv * (J(x) - J_0)).colPivHouseholderQr().solve(- eq);
            };

            success = false;
            return x;
        }

        /*! @brief 正常終了したかどうか
        */
        bool fail() const
        {
            return !success;
        }

        /*! @brief ステップ数
        */
        int step() const
        {
            return _step;
        }
    };

    template <typename T, int n, int m>
    double Newton_Raphson2 <std::function<Eigen::Vector<T, m>(const Eigen::Vector<T, n> &)>>::epsilon = 1.e-5;

    template <typename T, int n, int m>
    char Newton_Raphson2 <std::function<Eigen::Vector<T, m>(const Eigen::Vector<T, n> &)>>::delim = ',';

    template <typename T, int n, int m>
    int Newton_Raphson2 <std::function<Eigen::Vector<T, m>(const Eigen::Vector<T, n> &)>>::max_rep = 1000;
}

#endif
