#include <exeigen>
#include <eigen_io>
#include <Eigen/Sparse>
#include <Eigen/IterativeLinearSolvers>
#include <unsupported/Eigen/KroneckerProduct>
#include <debug>
#include <constexpr/cmath>

using Scalar = double;
constexpr Scalar epsilon = 1.e-6;
namespace Eigen {
    template <typename T>
    using MatrixX = Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>;
    template <typename T>
    using VectorX = Eigen::Vector<T, Eigen::Dynamic>;
    template <typename T>
    using SMatrix = Eigen::SparseMatrix<T>;
}

template <typename T>
Eigen::SMatrix<T> make_mask(const Eigen::SMatrix<T> &M)
{
    Eigen::SMatrix<T> retval(M);
    for (int j = 0; j < retval.outerSize(); ++j) {
        for (typename Eigen::SMatrix <T>::InnerIterator it(retval, j); it; ++it)
        {
            it.valueRef() = static_cast<T>(it.value() > epsilon || it.value() < -epsilon);
        }
    }
    return retval;
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

template <typename T>
Eigen::MatrixX<T> prox_nu(const Eigen::MatrixX<T> &M, const Scalar &l)
{
    using M_type = Eigen::MatrixX<T>;
    const int m = M.rows();
    const int n = M.cols();
    const int minmn = std::min(m, n);
    Eigen::JacobiSVD<M_type> svd(M, Eigen::ComputeThinU | Eigen::ComputeThinV);
    Eigen::VectorX<T> softenSV = svd.singularValues();
    for (int i = 0; i < minmn; i++) {
        softenSV[i] = soften(softenSV[i], l);
    }
    return svd.matrixU()*softenSV.asDiagonal()*svd.matrixV().transpose();
}

// MCクラス
template <typename T>
class MC {
public:
    using Scalar = T;
    using M_type = Eigen::MatrixX<Scalar>;
    using SM_type = Eigen::SMatrix<Scalar>;
    using rM_type = Eigen::MatrixX<Scalar>;
    using cM_type = Eigen::MatrixX<Scalar>;
    using bigV_type = Eigen::VectorX<Scalar>;
    using bigM_type = Eigen::MatrixX<Scalar>;
    using bigSM_type = Eigen::SMatrix<Scalar>;
public:
    const int m;
    const int n;
    const int mn;
    const Scalar sqrtmn;
    SM_type M; // 観測行列．平均ゼロ
    SM_type A; // マスク行列
    M_type X; // 真の行列
    M_type Y;
    M_type Y_old;
    M_type Z;
    SM_type Lr; // 行グラフラグランジアン
    SM_type Lc; // 列グラフラグランジアン
    Scalar gamma_n;
    Scalar gamma_r;
    Scalar gamma_c;
    Scalar rho;
    bigSM_type coef;
    Eigen::ConjugateGradient<bigSM_type> cg;
public:
    static int max_rep;
    static Scalar abs_tol;
    static Scalar rel_tol;
public:
    MC(const int m_, const int n_)
    : m(m_), n(n_), mn(m_*n_), sqrtmn(std::sqrt(m_*n_)), M(SM_type(m_, n_)), A(SM_type(m_, n_)), X(M_type(m_, n_)), Y(M_type(m_, n_)), Y_old(M_type(m_, n_)), Z(M_type(m_, n_)), Lr(SM_type(m_, m_)), Lc(SM_type(n_, n_)), coef(bigSM_type(m_*n_, m_*n_))
    {
    }
    void init()
    {
        Y = Y_old = M_type::Zero(m, n);
        M_type tmp = M_type(M.transpose());
        Z = Eigen::Map<M_type>(tmp.data(), Z.rows(), Z.cols());
        _PRINT(M_type(M))
        _PRINT(Z)

        // Aを行列化ののち対角行列にする
        coef.setIdentity();
        coef *= rho;
        for (int j = 0; j < A.outerSize(); ++j) {
            for (typename Eigen::SMatrix <Scalar>::InnerIterator it(A, j); it; ++it)
            {
                const int i = it.row();
                coef.coeffRef(m*j+i, m*j+i) += it.value();
            }
        }
        SM_type matidm(m, m);
        matidm.setIdentity();
        matidm *= gamma_r;
        SM_type matidn(n, n);
        matidn.setIdentity();
        matidn *= gamma_c;
        coef += Eigen::kroneckerProduct(Lr, matidn);
        coef += Eigen::kroneckerProduct(matidm, Lc);
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
        M_type tmp = rho*(X+Z);
        tmp += M;
        bigV_type tmp2 = cg.solve(Eigen::Map<bigV_type>(tmp.data(), m*n));
        Y = Eigen::Map<M_type>(tmp2.data(), m, n);
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

template <typename T>
int MC<T>::max_rep = 100;
template <typename T>
T MC<T>::abs_tol = 1.e-6;
template <typename T>
T MC<T>::rel_tol = 1.e-6;
