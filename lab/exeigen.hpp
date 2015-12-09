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

/*! @brief エイリアス群
*/
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
    template <int _Rows, int _Cols>
    using dMatrix = Matrix <double, _Rows, _Cols>;

    template <int _Rows>
    using dVector = Vector<double, _Rows>;

    template <int _Cols>
    using dRowVector = RowVector<double, _Cols>;

    /*! @alias
        @brief std::complex<double>型
    */
    template <int _Rows, int _Cols>
    using cMatrix = Matrix <std::complex<double>, _Rows, _Cols>;

    template <int _Rows>
    using cVector = Vector<std::complex<double>, _Rows>;

    template <int _Cols>
    using cRowVector = RowVector<std::complex<double>, _Cols>;
}

/*! @brief 要素演算群
*/
namespace std {
    /*! @brief Eigen::Matrixに対するmin/max
        複素行列をノルムで評価する仕様にすると，実負値の評価が曖昧になる
    */
    template <typename _Scalar, int _Rows, int _Cols>
    _Scalar min(const Eigen::Matrix <_Scalar, _Rows, _Cols> &M)
    {
        return M.minCoeff();
    }

    template <typename _Scalar, int _Rows, int _Cols>
    _Scalar max(const Eigen::Matrix <_Scalar, _Rows, _Cols> &M)
    {
        return M.maxCoeff();
    }

    /*! @brief Eigen::Matrixに対するelement-wise関数
        主にstd関数の拡張
    */
    template <typename _Scalar, int _Rows, int _Cols>
    Eigen::Matrix <_Scalar, _Rows, _Cols> real(const Eigen::Matrix <_Scalar, _Rows, _Cols> &M)
    {
        return M;
    }

    template <typename F, int _Rows, int _Cols>
    Eigen::Matrix <F, _Rows, _Cols> real(const Eigen::Matrix <std::complex<F>, _Rows, _Cols> &M)
    {
        const int rows = M.rows();
        const int cols = M.cols();
        Eigen::Matrix <F, _Rows, _Cols> retval;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                retval(i, j) = std::real(M(i, j));
            }
        }

        return retval;
    }

    template <typename _Scalar, int _Rows, int _Cols>
    Eigen::Matrix <_Scalar, _Rows, _Cols> imag(const Eigen::Matrix <_Scalar, _Rows, _Cols> &M)
    {
        const int rows = M.rows();
        const int cols = M.cols();
        return Eigen::Matrix <_Scalar, _Rows, _Cols>::Zero(rows, cols);
    }

    template <typename F, int _Rows, int _Cols>
    Eigen::Matrix <F, _Rows, _Cols> imag(const Eigen::Matrix <std::complex<F>, _Rows, _Cols> &M)
    {
        const int rows = M.rows();
        const int cols = M.cols();
        Eigen::Matrix <F, _Rows, _Cols> retval;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                retval(i, j) = std::imag(M(i, j));
            }
        }

        return retval;
    }

    template <typename _Scalar, int _Rows, int _Cols>
    Eigen::Matrix <typename std::decomplexify<_Scalar>::type, _Rows, _Cols> abs(const Eigen::Matrix <_Scalar, _Rows, _Cols> &M)
    {
        const int rows = M.rows();
        const int cols = M.cols();
        Eigen::Matrix <typename std::decomplexify<_Scalar>::type, _Rows, _Cols> retval;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                retval(i, j) = std::abs(M(i, j));
            }
        }

        return retval;
    }

    template <typename _Scalar, int _Rows, int _Cols>
    Eigen::Matrix <typename std::decomplexify<_Scalar>::type, _Rows, _Cols> norm(const Eigen::Matrix <_Scalar, _Rows, _Cols> &M)
    {
        const int rows = M.rows();
        const int cols = M.cols();
        Eigen::Matrix <typename std::decomplexify<_Scalar>::type, _Rows, _Cols> retval;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                retval(i, j) = std::norm(M(i, j));
            }
        }

        return retval;
    }

    template <typename F, int _Rows, int _Cols>
    Eigen::Matrix <F, _Rows, _Cols> arg(const Eigen::Matrix <std::complex<F>, _Rows, _Cols> &M)
    {
        const int rows = M.rows();
        const int cols = M.cols();
        Eigen::Matrix <F, _Rows, _Cols> retval;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                retval(i, j) = std::arg(M(i, j));
            }
        }

        return retval;
    }

    template <typename _Scalar, int _Rows, int _Cols>
    Eigen::Matrix <_Scalar, _Rows, _Cols> conj(const Eigen::Matrix <_Scalar, _Rows, _Cols> &M)
    {
        return M;
    }

    template <typename F, int _Rows, int _Cols>
    Eigen::Matrix <std::complex<F>, _Rows, _Cols> conj(const Eigen::Matrix <std::complex<F>, _Rows, _Cols> &M)
    {
        const int rows = M.rows();
        const int cols = M.cols();
        Eigen::Matrix <std::complex<F>, _Rows, _Cols> retval;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                retval(i, j) = std::conj(M(i, j));
            }
        }

        return retval;
    }
}

/*! @brief 行列演算群
*/
namespace Eigen {
    /*! @brief k番目の標準基底ベクトルを返す関数．
        Dynamicの場合は第二引数でサイズを指定する
    */
    template <typename T, int n>
    Vector <T, n> standard_base(const int k, const int size = n)
    {
        Vector<T, n> retval = Vector<T, n>::Zero(size);
        retval(k) = 1.;
        return retval;
    }

    /*! @brief 行列のエルミート転置を返す関数
    */
    template <typename _Scalar, int _Rows, int _Cols>
    Eigen::Matrix <_Scalar, _Cols, _Rows> Hermite(const Eigen::Matrix <_Scalar, _Rows, _Cols> &M)
    {
        return std::conj(M).transpose();
    }

    /*! @brief 行列を対称化する関数
    */
    template <typename _Scalar, int _Size>
    Eigen::Matrix <_Scalar, _Size, _Size> symmentrize(const Eigen::Matrix <_Scalar, _Size, _Size> &M)
    {
        return (M + M.transpose()) / 2;
    }

    /*! @brief 行列を歪対称化する関数
    */
    template <typename _Scalar, int _Size>
    Eigen::Matrix <_Scalar, _Size, _Size> antisymmetrise(const Eigen::Matrix <_Scalar, _Size, _Size> &M)
    {
        return (M - M.transpose()) / 2;
    }

    /*! @brief 行列をエルミート化する関数
    */
    template <typename _Scalar, int _Size>
    Eigen::Matrix <_Scalar, _Size, _Size> Hermitize(const Eigen::Matrix <_Scalar, _Size, _Size> &M)
    {
        return (M + Hermite(M)) / 2;
    }

    /*! @brief 行列を歪エルミート化する関数
    */
    template <typename _Scalar, int _Size>
    Eigen::Matrix <_Scalar, _Size, _Size> antiHermitize(const Eigen::Matrix <_Scalar, _Size, _Size> &M)
    {
        return (M - Hermite(M)) / 2;
    }

    /*! @brief 行列のk番目（0が最大，n-1が最小）の特異値を返す関数
        @param M 行列
        @param k 求める特異値の序列
    */
    template <typename _Scalar, int _Rows, int _Cols>
    typename std::decomplexify<_Scalar>::type singular_value(const Eigen::Matrix <_Scalar, _Rows, _Cols> &M, const int k)
    {
        Eigen::JacobiSVD <Eigen::Matrix <_Scalar, _Rows, _Cols>> svd(M);

        return svd.singularValues().eval()(k);
    }

    /*! @brief 行列のk番目の固有値を返す関数
        @param M 行列
        @param k 求める固有値の序列
    */
    template <typename _Scalar, int _Rows, int _Cols>
    typename std::decomplexify<_Scalar>::type eigenvalue(const Eigen::Matrix <_Scalar, _Rows, _Cols> &M, const int k)
    {
        //todo: 順序は適当
        Eigen::EigenSolver <Eigen::Matrix <_Scalar, _Rows, _Cols>> es(M);

        return es.eigenValues().eval()(k);
    }

    /*! @brief エルミート行列のk番目の固有値（0が最大，n-1が最小）を返す関数
        @param M 行列
        @param k 求める固有値の序列
    */
    template <typename _Scalar, int _Rows, int _Cols>
    typename std::decomplexify<_Scalar>::type selfadjoint_eigenvalue(const Eigen::Matrix <_Scalar, _Rows, _Cols> &M, const int k)
    {
        assert(M.rows() == M.cols());
        Eigen::SelfAdjointEigenSolver <Eigen::Matrix <_Scalar, _Rows, _Cols>> es(M);
        if (es.info() != Eigen::Success) {
            assert(false);
            return 0;
        }

        return es.eigenvalues().eval()(M.rows() - 1 - k);
    }

    /*! @brief 行列の正定値性を判定する関数
        @param M エルミート行列
    */
    template <typename _Scalar, int _Rows, int _Cols>
    bool positive_definite(const Eigen::Matrix <_Scalar, _Rows, _Cols> &M)
    {
        assert(M.rows() == M.cols());
        Eigen::SelfAdjointEigenSolver <Eigen::Matrix <_Scalar, _Rows, _Cols>> es(M);
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
    template <typename _Scalar, int _Rows, int _Cols>
    bool positive_semidefinite(const Eigen::Matrix <_Scalar, _Rows, _Cols> &M)
    {
        assert(M.rows() == M.cols());
        Eigen::SelfAdjointEigenSolver <Eigen::Matrix <_Scalar, _Rows, _Cols>> es(M);
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
    template <typename _Scalar, int _Rows, int _Cols>
    bool negative_definite(const Eigen::Matrix <_Scalar, _Rows, _Cols> &M)
    {
        assert(M.rows() == M.cols());
        Eigen::SelfAdjointEigenSolver <Eigen::Matrix <_Scalar, _Rows, _Cols>> es(M);
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
    template <typename _Scalar, int _Rows, int _Cols>
    bool negative_semidefinite(const Eigen::Matrix <_Scalar, _Rows, _Cols> &M)
    {
        assert(M.rows() == M.cols());
        Eigen::SelfAdjointEigenSolver <Eigen::Matrix <_Scalar, _Rows, _Cols>> es(M);
        if (es.info() != Eigen::Success) {
            std::cerr << "EigenSolver error" << std::endl;
            assert(false);

            return false;
        }

        return (es.eigenvalues().array() <= 0.).all();
    }

    /*! @brief 行列のpositive diagonal QR decompositionを求める関数
    */
    template <typename _Scalar, int _Rows, int _Cols>
    Eigen::Matrix <_Scalar, _Rows, _Rows> householderQ_unique(const Eigen::Matrix <_Scalar, _Rows, _Cols> &M)
    {
        using M_type = Eigen::Matrix <_Scalar, _Rows, _Cols>;
        using Q_type = Eigen::Matrix <_Scalar, _Rows, _Rows>;
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
