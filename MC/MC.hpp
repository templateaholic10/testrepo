#include <exeigen>
#include <eigen_io>
#include <Eigen/IterativeLinearSolvers>
#include <unsupported/Eigen/KroneckerProduct>
#include <debug>
#include <constexpr/cmath>

using Scalar = double;

template <typename T, int m, int n>
Eigen::Matrix<T, m, n> make_mask(const Eigen::Matrix<T, m, n> &M)
{
    return (M.array()>0.0).matrix().template cast<T>();
}

template <typename T>
int sgn(const T &x)
{
    return (static_cast<T>(0) < x) - (x < static_cast<T>(0));
}

template <typename T>
T soften(const T &x, const T &l)
{
    return std::max(std::abs(x)-l, 0.)*sgn(x);
}

// template <typename Scalar, int m, int n>
// Eigen::Vector<Scalar, m*n> vectorize(const Eigen::Matrix<Scalar, m, n>)
// B(Map<VectorXd>(A.data(), A.cols()*A.rows()));

template <int m, int n>
Eigen::Matrix<Scalar, m, n> prox_nu(const Eigen::Matrix<Scalar, m, n> &M, const Scalar &l)
{
    using M_type = Eigen::Matrix<Scalar, m, n>;
    constexpr int minmn = cpstd::min(m, n);
    Eigen::JacobiSVD<Eigen::MatrixXd> svd(M, Eigen::ComputeThinU | Eigen::ComputeThinV);
    Eigen::Vector<Scalar, minmn> softenSV = svd.singularValues();
    for (int i = 0; i < minmn; i++) {
        softenSV[i] = soften(softenSV[i], l);
    }
    return svd.matrixU()*softenSV.asDiagonal()*svd.matrixV().transpose();
}

// MCクラス
template <int m, int n>
class MC {
public:
    using M_type = Eigen::Matrix<Scalar, m, n>;
    using rM_type = Eigen::Matrix<Scalar, m, m>;
    using cM_type = Eigen::Matrix<Scalar, n, n>;
    constexpr static int mn = m*n;
    constexpr static Scalar sqrtmn = cpstd::sqrt(mn);
    using bigV_type = Eigen::Vector<Scalar, mn>;
    using bigM_type = Eigen::Matrix<Scalar, mn, mn>;
public:
    M_type M; // 観測行列．平均ゼロ
    M_type A; // マスク行列
    M_type X; // 真の行列
    M_type Y;
    M_type Y_old;
    M_type Z;
    rM_type Lr; // 行グラフラグランジアン
    cM_type Lc; // 列グラフラグランジアン
    Scalar gamma_n;
    Scalar gamma_r;
    Scalar gamma_c;
    Scalar rho;
    Eigen::ConjugateGradient<bigM_type> cg;
    bigM_type coef;
public:
    constexpr static int max_rep = 100;
    constexpr static Scalar abs_tol = 1.e-6;
    constexpr static Scalar rel_tol = 1.e-6;
public:
    MC()
    {
    }
    void init()
    {
        Y = Y_old = M_type::Zero();
        Eigen::Transpose_t<M_type> tmp2 = M.transpose();
        Z = Eigen::Map<M_type>(tmp2.data());
        coef = Eigen::Map<bigV_type>(A.data()).asDiagonal();
        coef += gamma_r*Eigen::kroneckerProduct(Lr, cM_type::Identity()).eval()+gamma_c*Eigen::kroneckerProduct(rM_type::Identity(), Lc)+rho*bigM_type::Identity();
        _PRINT(coef)
        cg.compute(coef);
    }
    void X_opt()
    {
        // Y-Zにgamma_n/rhoで近接させる
        X = prox_nu((Y-Z).eval(), gamma_n/rho);
    }
    void Y_opt()
    {
        Y_old = Y;
        M_type tmp = M+rho*(X+Z);
        Eigen::Map<bigV_type>(tmp.data());
        bigV_type tmp2 = cg.solve(Eigen::Map<bigV_type>(tmp.data()));
        Y = Eigen::Map<M_type>(tmp2.data());
    }
    void step()
    {
        X_opt();
        Y_opt();
        Z += X - Y;
    }
    bool stop_cond() const
    {
        Scalar err_P = (X-Y).norm();
        Scalar err_D = rho*(Y_old-Y).norm();
        Scalar tor_P = sqrtmn * abs_tol + rel_tol * std::max(X.norm(), Y.norm());
        Scalar tor_D = sqrtmn * abs_tol + rel_tol * Z.norm();

        return err_P < tor_P && err_D < tor_D;
    }
    void go()
    {
        init();
        for (int i = 0; i < max_rep; i++) {
            _PRINT(i)
            step();
            if (stop_cond()) {
                break;
            }
        }
    }
};
