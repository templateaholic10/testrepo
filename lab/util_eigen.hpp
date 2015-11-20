/*! @file
    @brief Eigenを使用する上でのユーティリティ関数およびエイリアス群
    @author yanteyon10
    @date 10/28
*/
#ifndef UTIL_EIGEN
#define UTIL_EIGEN

#include <algorithm>
#include <complex>
#include <functional>
#include <util_rand>
#define EIGEN_NO_DEBUG
#include <Eigen/Dense>
#include <util>

/*! @brief エイリアス定義部
*/
namespace Eigen {
    template <typename T, int p>
    using Vector = Matrix <T, p, 1>;

    template <typename T, int p>
    using RowVector = Matrix <T, 1, p>;

    template <int m, int n>
    using dMatrix = Matrix <double, m, n>;

    template <int n>
    using dVector = Vector <double, n>;

    template <int n>
    using dRowVector = RowVector <double, n>;

    template <int m, int n>
    using cMatrix = Matrix <std::complex<double>, m, n>;

    template <int n>
    using cVector = Vector <std::complex<double>, n>;

    template <int n>
    using cRowVector = RowVector <std::complex<double>, n>;
}

/*! @brief std関数のEigen対応
*/
namespace std {
    template <typename T, int m, int n>
    Eigen::Matrix <T, m, n> real(const Eigen::Matrix <T, m, n> &M)
    {
        return M;
    }

    template <typename T, int m, int n>
    Eigen::Matrix <T, m, n> real(const Eigen::Matrix <std::complex <T>, m, n> &M)
    {
        Eigen::Matrix <T, m, n> retval;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                retval(i, j) = std::real(M(i, j));
            }
        }

        return retval;
    }

    template <typename T, int m, int n>
    Eigen::Matrix <T, m, n> imag(const Eigen::Matrix <T, m, n> &M)
    {
        return Eigen::Matrix<T, m, n>::Zero();
    }

    template <typename T, int m, int n>
    Eigen::Matrix <T, m, n> imag(const Eigen::Matrix <std::complex <T>, m, n> &M)
    {
        Eigen::Matrix <T, m, n> retval;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                retval(i, j) = std::imag(M(i, j));
            }
        }

        return retval;
    }

    template <typename T, int m, int n>
    Eigen::Matrix <T, m, n> abs(const Eigen::Matrix <T, m, n> &M)
    {
        Eigen::Matrix <T, m, n> retval;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                retval(i, j) = std::abs(M(i, j));
            }
        }

        return retval;
    }

    template <typename T, int m, int n>
    Eigen::Matrix <T, m, n> abs(const Eigen::Matrix <std::complex <T>, m, n> &M)
    {
        Eigen::Matrix <T, m, n> retval;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                retval(i, j) = std::abs(M(i, j));
            }
        }

        return retval;
    }

    template <typename T, int m, int n>
    Eigen::Matrix <T, m, n> norm(const Eigen::Matrix <T, m, n> &M)
    {
        Eigen::Matrix <T, m, n> retval;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                retval(i, j) = std::norm(M(i, j));
            }
        }

        return retval;
    }

    template <typename T, int m, int n>
    Eigen::Matrix <T, m, n> norm(const Eigen::Matrix <std::complex <T>, m, n> &M)
    {
        Eigen::Matrix <T, m, n> retval;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                retval(i, j) = std::norm(M(i, j));
            }
        }

        return retval;
    }

    template <typename T, int m, int n>
    Eigen::Matrix<T, m, n> arg(const Eigen::Matrix<std::complex<T>, m, n>& M)
    {
        Eigen::Matrix<T, m, n> retval;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                retval(i, j) = std::arg(M(i, j));
            }
        }
        return retval;
    }

    template <typename T, int m, int n>
    Eigen::Matrix<T, m, n> conj(const Eigen::Matrix<T, m, n>& M)
    {
        return M;
    }

    template <typename T, int m, int n>
    Eigen::Matrix<T, m, n> conj(const Eigen::Matrix<std::complex<T>, m, n>& M)
    {
        Eigen::Matrix<T, m, n> retval;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                retval(i, j) = std::conj(M(i, j));
            }
        }
        return retval;
    }

    template <typename T, int m, int n>
    T min(const Eigen::Matrix <T, m, n> &M)
    {
        T retval = M(0, 0);
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (retval > M(i, j)) {
                    retval = M(i, j);
                }
            }
        }
        return retval;
    }

    template <typename T, int m, int n>
    T min(const Eigen::Matrix <std::complex <T>, m, n> &M)
    {
        return min(abs(M));
    }

    template <typename T, int m, int n>
    T max(const Eigen::Matrix <T, m, n> &M)
    {
        T retval = M(0, 0);
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (retval < M(i, j)) {
                    retval = M(i, j);
                }
            }
        }
        return retval;
    }

    template <typename T, int m, int n>
    T max(const Eigen::Matrix <std::complex <T>, m, n> &M)
    {
        return max(abs(M));
    }
}

namespace Eigen {
    /*! @brief 行列のエルミート転置を返す関数．実行列の場合は転置するだけ
    */
    template <typename T, int m, int n>
    Matrix <std::complex <T>, n, m> Hermite(const Matrix <std::complex <T>, m, n> &M)
    {
        auto _conj = [](std::complex <T> z) {
                         return std::conj(z);
                     };

        return M.unaryExpr(_conj).transpose();
    }

    template <int m, int n>
    dMatrix <n, m> Hermite(const dMatrix <m, n> &M)
    {
        return M.transpose();
    }

    /*! @brief 行列を対称化する関数
    */
    template <typename T, int n>
    Matrix <T, n, n> symmetrize(const Matrix <T, n, n> &M)
    {
        return (M + M.transpose()) / 2;
    }

    /*! @brief 行列を歪対称化する関数
    */
    template <typename T, int n>
    Matrix <T, n, n> antisymmetrize(const Matrix <T, n, n> &M)
    {
        return (M - M.transpose()) / 2;
    }

    /*! @brief 行列をエルミート化する関数
    */
    template <typename T, int m, int n>
    Matrix <T, n, m> Hermitize(const Matrix <T, m, n> &M)
    {
        return (M + Hermite(M)) / 2;
    }

    /*! @brief 行列を歪エルミート化する関数
    */
    template <typename T, int m, int n>
    Matrix <T, n, m> antiHermitize(const Matrix <T, m, n> &M)
    {
        return (M - Hermite(M)) / 2;
    }

    /*! @brief 行列のスペクトル半径を返す関数
        @param M 行列
    */
    template <typename T, int n>
    double spectral_radius(const Matrix <T, n, n> &M)
    {
        Eigen::EigenSolver<Matrix<T, n, n>> es(M);
        if (es.info() != Eigen::Success) {
            std::cerr << "EigenSolver error" << std::endl;
            assert(false);
            return false;
        }
        return std::max(es.eigenvalues().eval());
    }

    /*! @brief エルミート行列のスペクトル半径を返す関数
        @param M エルミート行列
    */
    template <typename T, int n>
    double selfadjoint_spectral_radius(const Matrix <T, n, n> &M)
    {
        Eigen::SelfAdjointEigenSolver<Matrix<T, n, n>> es(M);
        if (es.info() != Eigen::Success) {
            std::cerr << "EigenSolver error" << std::endl;
            assert(false);
            return false;
        }
        return std::max(es.eigenvalues().eval());
    }

    /*! @brief 行列の正定値性を判定する関数
        @param M エルミート行列
    */
    template <typename T, int n>
    bool positive_definite(const Matrix <T, n, n> &M)
    {
        Eigen::SelfAdjointEigenSolver<Matrix<T, n, n>> es(M);
        if (es.info() != Eigen::Success) {
            std::cerr << "EigenSolver error" << std::endl;
            assert(false);
            return false;
        }
        return (es.eigenvalues().array() > 0.).all();
    }

    /*! @brief 行列の半正定値性を判定する関数
        @param M エルミート行列
    */
    template <typename T, int n>
    bool positive_semidefinite(const Matrix <T, n, n> &M)
    {
        Eigen::SelfAdjointEigenSolver<Matrix<T, n, n>> es(M);
        if (es.info() != Eigen::Success) {
            std::cerr << "EigenSolver error" << std::endl;
            assert(false);
            return false;
        }
        return (es.eigenvalues().array() >= 0.).all();
    }

    /*! @brief 行列の負定値性を判定する関数
        @param M エルミート行列
    */
    template <typename T, int n>
    bool negative_definite(const Matrix <T, n, n> &M)
    {
        Eigen::SelfAdjointEigenSolver<Matrix<T, n, n>> es(M);
        if (es.info() != Eigen::Success) {
            std::cerr << "EigenSolver error" << std::endl;
            assert(false);
            return false;
        }
        return (es.eigenvalues().array() < 0.).all();
    }

    /*! @brief 行列の半負定値性を判定する関数
        @param M エルミート行列
    */
    template <typename T, int n>
    bool negative_semidefinite(const Matrix <T, n, n> &M)
    {
        Eigen::SelfAdjointEigenSolver<Matrix<T, n, n>> es(M);
        if (es.info() != Eigen::Success) {
            std::cerr << "EigenSolver error" << std::endl;
            assert(false);
            return false;
        }
        return (es.eigenvalues().array() <= 0.).all();
    }

    /*! @brief 各要素が一様分布に従う確率変数によって決定される行列を返す関数
        @tparam rv_type 確率変数の型
        @tparam m 行列の行数
        @tparam n 行列の列数
        @tparam elem_type 要素の型．デフォルトはelem_typeと同じ
    */
    template <typename rv_type, int m, int n, typename elem_type = rv_type>
    Matrix <elem_type, m, n> rand_uniform(const rv_type &a, const rv_type &b, const std::random_device::result_type seed=std::random_device()())
    {
        Matrix <elem_type, m, n> retval;
        util::Uniform <rv_type>  uniform(a, b, seed);
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                retval(i, j) = uniform();
            }
        }

        return retval;
    }
}

namespace util {
    template <class Seq, int n>
    Seq stlize(const Eigen::Vector <typename Seq::value_type, n> &v)
    {
        Seq retval;
        for (int i = 0; i < n; i++) {
            retval[i] = v[i];
        }

        return retval;
    }

    template <typename T, int m, int n>
    std::vector <T> row_vector(const Eigen::Matrix <T, m, n> &M, const int row_index)
    {
        std::vector <T> retval(n);
        for (int j = 0; j < n; j++) {
            retval[j] = M(row_index, j);
        }

        return retval;
    }

    template <typename T, int m, int n>
    std::vector <T> col_vector(const Eigen::Matrix <T, m, n> &M, const int col_index)
    {
        std::vector <T> retval(m);
        for (int i = 0; i < m; i++) {
            retval[i] = M(i, col_index);
        }

        return retval;
    }

    template <typename T, int m, int n, class Seq>
    void set_row(Eigen::Matrix <T, m, n> &M, const int row_index, const Seq &seq)
    {
        for (int j = 0; j < n; j++) {
            M(row_index, j) = seq[j];
        }
    }

    template <typename T, int m, int n, class Seq>
    void set_col(Eigen::Matrix <T, m, n> &M, const int col_index, const Seq &seq)
    {
        for (int i = 0; i < m; i++) {
            M(i, col_index) = seq[i];
        }
    }
}

#endif
