/*! @file
    @brief Eigenのためのエイリアス，要素演算，行列演算群
    @author yanteyon10
    @date 11/21
*/
#ifndef EXEIGEN_HPP
#define EXEIGEN_HPP

#include <iostream>
#include <excomplex>
#include <Eigen/Core>
#include <Eigen/SVD>
#include <Eigen/Eigenvalues>

namespace Eigen {
    /*! @alias
        @brief ベクトル
    */
    template <typename _Scalar, int _Rows>
    using Vector = Matrix <_Scalar, _Rows, 1>;

    template <typename _Scalar, int _Cols>
    using RowVector = Matrix <_Scalar, 1, _Cols>;

    /*! @alias
        @brief double型
    */
    template <int _Rows, int _Cols, int _Options = Eigen::ColMajor, int _MaxRows = _Rows, int _MaxCols = _Cols>
    using dMatrix = Matrix <double, _Rows, _Cols, _Options, _MaxRows, _MaxCols>;

    template <int _Rows>
    using dVector = Vector<double, _Rows>;

    template <int _Cols>
    using dRowVector = RowVector<double, _Cols>;

    /*! @alias
        @brief std::complex<double>型
    */
    template <int _Rows, int _Cols, int _Options = Eigen::ColMajor, int _MaxRows = _Rows, int _MaxCols = _Cols>
    using cMatrix = Matrix <std::complex<double>, _Rows, _Cols, _Options, _MaxRows, _MaxCols>;

    template <int _Rows>
    using cVector = Vector<std::complex<double>, _Rows>;

    template <int _Cols>
    using cRowVector = RowVector<std::complex<double>, _Cols>;
}

namespace std {
    /*! @brief Eigen::Matrixに対するmin/max
        複素行列をノルムで評価する仕様にすると，実負値の評価が曖昧になる
    */
    template <typename _Scalar, int _Rows, int _Cols, int _Options = Eigen::ColMajor, int _MaxRows = _Rows, int _MaxCols = _Cols>
    _Scalar min(const Eigen::Matrix <_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols> &M)
    {
        return M.minCoeff();
    }

    template <typename _Scalar, int _Rows, int _Cols, int _Options = Eigen::ColMajor, int _MaxRows = _Rows, int _MaxCols = _Cols>
    _Scalar max(const Eigen::Matrix <_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols> &M)
    {
        return M.maxCoeff();
    }

    /*! @brief Eigen::Matrixに対するelement-wise関数
        主にstd関数の拡張
    */
    template <typename _Scalar, int _Rows, int _Cols, int _Options = Eigen::ColMajor, int _MaxRows = _Rows, int _MaxCols = _Cols>
    Eigen::Matrix <_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols> real(const Eigen::Matrix <_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols> &M)
    {
        return M;
    }

    template <typename F, int _Rows, int _Cols, int _Options = Eigen::ColMajor, int _MaxRows = _Rows, int _MaxCols = _Cols>
    Eigen::Matrix <F, _Rows, _Cols, _Options, _MaxRows, _MaxCols> real(const Eigen::Matrix <std::complex<F>, _Rows, _Cols, _Options, _MaxRows, _MaxCols> &M)
    {
        const int rows = M.rows();
        const int cols = M.cols();
        Eigen::Matrix <F, _Rows, _Cols, _Options, _MaxRows, _MaxCols> retval;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                retval(i, j) = std::real(M(i, j));
            }
        }

        return retval;
    }

    template <typename _Scalar, int _Rows, int _Cols, int _Options = Eigen::ColMajor, int _MaxRows = _Rows, int _MaxCols = _Cols>
    Eigen::Matrix <_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols> imag(const Eigen::Matrix <_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols> &M)
    {
        const int rows = M.rows();
        const int cols = M.cols();
        return Eigen::Matrix <_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols>::Zero(rows, cols);
    }

    template <typename F, int _Rows, int _Cols, int _Options = Eigen::ColMajor, int _MaxRows = _Rows, int _MaxCols = _Cols>
    Eigen::Matrix <F, _Rows, _Cols, _Options, _MaxRows, _MaxCols> imag(const Eigen::Matrix <std::complex<F>, _Rows, _Cols, _Options, _MaxRows, _MaxCols> &M)
    {
        const int rows = M.rows();
        const int cols = M.cols();
        Eigen::Matrix <F, _Rows, _Cols, _Options, _MaxRows, _MaxCols> retval;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                retval(i, j) = std::imag(M(i, j));
            }
        }

        return retval;
    }

    template <typename _Scalar, int _Rows, int _Cols, int _Options = Eigen::ColMajor, int _MaxRows = _Rows, int _MaxCols = _Cols>
    Eigen::Matrix <typename std::decomplexify<_Scalar>::type, _Rows, _Cols, _Options, _MaxRows, _MaxCols> abs(const Eigen::Matrix <_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols> &M)
    {
        const int rows = M.rows();
        const int cols = M.cols();
        Eigen::Matrix <typename std::decomplexify<_Scalar>::type, _Rows, _Cols, _Options, _MaxRows, _MaxCols> retval;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                retval(i, j) = std::abs(M(i, j));
            }
        }

        return retval;
    }

    template <typename _Scalar, int _Rows, int _Cols, int _Options = Eigen::ColMajor, int _MaxRows = _Rows, int _MaxCols = _Cols>
    Eigen::Matrix <typename std::decomplexify<_Scalar>::type, _Rows, _Cols, _Options, _MaxRows, _MaxCols> norm(const Eigen::Matrix <_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols> &M)
    {
        const int rows = M.rows();
        const int cols = M.cols();
        Eigen::Matrix <typename std::decomplexify<_Scalar>::type, _Rows, _Cols, _Options, _MaxRows, _MaxCols> retval;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                retval(i, j) = std::norm(M(i, j));
            }
        }

        return retval;
    }

    template <typename F, int _Rows, int _Cols, int _Options = Eigen::ColMajor, int _MaxRows = _Rows, int _MaxCols = _Cols>
    Eigen::Matrix <F, _Rows, _Cols, _Options, _MaxRows, _MaxCols> arg(const Eigen::Matrix <std::complex<F>, _Rows, _Cols, _Options, _MaxRows, _MaxCols> &M)
    {
        const int rows = M.rows();
        const int cols = M.cols();
        Eigen::Matrix <F, _Rows, _Cols, _Options, _MaxRows, _MaxCols> retval;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                retval(i, j) = std::arg(M(i, j));
            }
        }

        return retval;
    }

    template <typename _Scalar, int _Rows, int _Cols, int _Options = Eigen::ColMajor, int _MaxRows = _Rows, int _MaxCols = _Cols>
    Eigen::Matrix <_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols> conj(const Eigen::Matrix <_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols> &M)
    {
        return M;
    }

    template <typename F, int _Rows, int _Cols, int _Options = Eigen::ColMajor, int _MaxRows = _Rows, int _MaxCols = _Cols>
    Eigen::Matrix <std::complex<F>, _Rows, _Cols, _Options, _MaxRows, _MaxCols> conj(const Eigen::Matrix <std::complex<F>, _Rows, _Cols, _Options, _MaxRows, _MaxCols> &M)
    {
        const int rows = M.rows();
        const int cols = M.cols();
        Eigen::Matrix <std::complex<F>, _Rows, _Cols, _Options, _MaxRows, _MaxCols> retval;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                retval(i, j) = std::conj(M(i, j));
            }
        }

        return retval;
    }
}

namespace Eigen {
    /*! @brief 行列のエルミート転置を返す関数
    */
    template <typename _Scalar, int _Rows, int _Cols, int _Options = Eigen::ColMajor, int _MaxRows = _Rows, int _MaxCols = _Cols>
    Eigen::Matrix <_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols> Hermite(const Eigen::Matrix <_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols> &M)
    {
        return std::conj(M.transpose());
    }

    /*! @brief 行列を対称化する関数
    */
    template <typename _Scalar, int _Rows, int _Cols, int _Options = Eigen::ColMajor, int _MaxRows = _Rows, int _MaxCols = _Cols>
    Eigen::Matrix <_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols> symmentrize(const Eigen::Matrix <_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols> &M)
    {
        return (M + M.transpose()) / 2;
    }

    /*! @brief 行列を歪対称化する関数
    */
    template <typename _Scalar, int _Rows, int _Cols, int _Options = Eigen::ColMajor, int _MaxRows = _Rows, int _MaxCols = _Cols>
    Eigen::Matrix <_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols> antisymmetrise(const Eigen::Matrix <_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols> &M)
    {
        return (M - M.transpose()) / 2;
    }

    /*! @brief 行列をエルミート化する関数
    */
    template <typename _Scalar, int _Rows, int _Cols, int _Options = Eigen::ColMajor, int _MaxRows = _Rows, int _MaxCols = _Cols>
    Eigen::Matrix <_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols> Hermitize(const Eigen::Matrix <_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols> &M)
    {
        return (M + Hermite(M)) / 2;
    }

    /*! @brief 行列を歪エルミート化する関数
    */
    template <typename _Scalar, int _Rows, int _Cols, int _Options = Eigen::ColMajor, int _MaxRows = _Rows, int _MaxCols = _Cols>
    Eigen::Matrix <_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols> antiHermitize(const Eigen::Matrix <_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols> &M)
    {
        return (M - Hermite(M)) / 2;
    }

    /*! @brief 行列のk番目（0が最大，n-1が最小）の特異値を返す関数
        @param M 行列
        @param k 求める特異値の序列
    */
    template <typename _Scalar, int _Rows, int _Cols, int _Options = Eigen::ColMajor, int _MaxRows = _Rows, int _MaxCols = _Cols>
    typename std::decomplexify<_Scalar>::type singular_value(const Eigen::Matrix <_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols> &M, const int k)
    {
        Eigen::JacobiSVD <Eigen::Matrix <_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols>> svd(M);

        return svd.singularValues().eval()(k);
    }

    /*! @brief 行列のk番目の固有値を返す関数
        @param M 行列
        @param k 求める固有値の序列
    */
    template <typename _Scalar, int _Rows, int _Cols, int _Options = Eigen::ColMajor, int _MaxRows = _Rows, int _MaxCols = _Cols>
    typename std::decomplexify<_Scalar>::type eigenvalue(const Eigen::Matrix <_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols> &M, const int k)
    {
        //todo: 順序は適当
        Eigen::EigenSolver <Eigen::Matrix <_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols>> es(M);

        return es.eigenValues().eval()(k);
    }

    /*! @brief エルミート行列のk番目の固有値（0が最大，n-1が最小）を返す関数
        @param M 行列
        @param k 求める固有値の序列
    */
    template <typename _Scalar, int _Rows, int _Cols, int _Options = Eigen::ColMajor, int _MaxRows = _Rows, int _MaxCols = _Cols>
    typename std::decomplexify<_Scalar>::type selfadjoint_eigenvalue(const Eigen::Matrix <_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols> &M, const int k)
    {
        assert(M.rows() == M.cols());
        Eigen::SelfAdjointEigenSolver <Eigen::Matrix <_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols>> es(M);
        if (es.info() != Eigen::Success) {
            assert(false);
            return 0;
        }

        return es.eigenvalues().eval()(M.rows() - 1 - k);
    }

    /*! @brief 行列の正定値性を判定する関数
        @param M エルミート行列
    */
    template <typename _Scalar, int _Rows, int _Cols, int _Options = Eigen::ColMajor, int _MaxRows = _Rows, int _MaxCols = _Cols>
    bool positive_definite(const Eigen::Matrix <_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols> &M)
    {
        assert(M.rows() == M.cols());
        Eigen::SelfAdjointEigenSolver <Eigen::Matrix <_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols>> es(M);
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
    template <typename _Scalar, int _Rows, int _Cols, int _Options = Eigen::ColMajor, int _MaxRows = _Rows, int _MaxCols = _Cols>
    bool positive_semidefinite(const Eigen::Matrix <_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols> &M)
    {
        assert(M.rows() == M.cols());
        Eigen::SelfAdjointEigenSolver <Eigen::Matrix <_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols>> es(M);
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
    template <typename _Scalar, int _Rows, int _Cols, int _Options = Eigen::ColMajor, int _MaxRows = _Rows, int _MaxCols = _Cols>
    bool negative_definite(const Eigen::Matrix <_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols> &M)
    {
        assert(M.rows() == M.cols());
        Eigen::SelfAdjointEigenSolver <Eigen::Matrix <_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols>> es(M);
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
    template <typename _Scalar, int _Rows, int _Cols, int _Options = Eigen::ColMajor, int _MaxRows = _Rows, int _MaxCols = _Cols>
    bool negative_semidefinite(const Eigen::Matrix <_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols> &M)
    {
        assert(M.rows() == M.cols());
        Eigen::SelfAdjointEigenSolver <Eigen::Matrix <_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols>> es(M);
        if (es.info() != Eigen::Success) {
            std::cerr << "EigenSolver error" << std::endl;
            assert(false);

            return false;
        }

        return (es.eigenvalues().array() <= 0.).all();
    }

    /*! @brief 行列のpositive diagonal QR decompositionを求める関数
    */
    template <typename _Scalar, int _Rows, int _Cols, int _Options = Eigen::ColMajor, int _MaxRows = _Rows, int _MaxCols = _Cols>
    Eigen::Matrix <_Scalar, _Rows, _Rows, _Options, _MaxRows, _MaxRows> householderQ_unique(const Eigen::Matrix <_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols> &M)
    {
        using M_type = Eigen::Matrix <_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols>;
        using Q_type = Eigen::Matrix <_Scalar, _Rows, _Rows, _Options, _MaxRows, _MaxRows>;
        Q_type Q = Eigen::HouseholderQR<M_type>(M).householderQ();
        const M_type R = Q.transpose() * M;

        const int rows = M.rows();
        for (int i = 0; i < rows; i++) {
            if (R(i, i) < 0.) {
                Q.col(i) = -Q.col(i);
            }
        }
        return Q;
    }
}

#endif
