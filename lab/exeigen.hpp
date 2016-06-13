/*! @file
    @brief Eigenのためのエイリアス，要素演算，行列演算群
    @author templateaholic10
    @date 11/21
*/
#ifndef EXEIGEN_HPP
#define EXEIGEN_HPP

#include <iostream>
#include <constexpr/cmath>
#include <excomplex>
#include <Eigen/Core>
#include <Eigen/SVD>
#include <Eigen/Eigenvalues>
#include <debug>

/*! @brief メタ関数
*/
namespace Eigen {
    /*! @namespace
        @brief サイズ調整に関する関数群．具体的にはEigen::Dynamic関連
    */
    namespace Size {
        namespace {
            constexpr int or_Dynamic(const int lhs, const int rhs, const int result)
            {
                if (lhs == Eigen::Dynamic || rhs == Eigen::Dynamic) {
                    return Eigen::Dynamic;
                } else {
                    return result;
                }
            }
        }

        constexpr int add(const int lhs, const int rhs)
        {
            return or_Dynamic(lhs, rhs, lhs + rhs);
        }

        constexpr int sub(const int lhs, const int rhs)
        {
            return or_Dynamic(lhs, rhs, lhs - rhs);
        }

        constexpr int multi(const int lhs, const int rhs)
        {
            return or_Dynamic(lhs, rhs, lhs * rhs);
        }

        constexpr int div(const int lhs, const int rhs)
        {
            return or_Dynamic(lhs, rhs, lhs / rhs);
        }
    }

    namespace meta {
        /*! @brief 行列を転置するメタ関数
        */
        template <typename T>
        struct transpose;

        template <typename Scalar, int Rows, int Cols>
        struct transpose <Eigen::Matrix <Scalar, Rows, Cols> >
        {
            using type = Eigen::Matrix <Scalar, Cols, Rows>;
        };

        /*! @breif 行列を定数だけ拡大するメタ関数
        */
        template <typename T, int Rowplus, int Colplus>
        struct expand;

        template <typename Scalar, int Rows, int Cols, int Rowplus, int Colplus>
        struct expand <Eigen::Matrix <Scalar, Rows, Cols>, Rowplus, Colplus> {
            using type = Eigen::Matrix <Scalar, Size::add(Rows, Rowplus), Size::add(Cols, Colplus)>;
        };

        /*! @brief 行列をスケール拡大するメタ関数
        */
        template <typename T, int Rowscale, int Colscale>
        struct scaleup;

        template <typename Scalar, int Rows, int Cols, int Rowscale, int Colscale>
        struct scaleup <Eigen::Matrix <Scalar, Rows, Cols>, Rowscale, Colscale> {
            using type = Eigen::Matrix <Scalar, Size::multi(Rows, Rowscale), Size::multi(Cols, Colscale)>;
        };

        /*! @brief 行列をスケール縮小するメタ関数
        */
        template <typename T, int Rowscale, int Colscale>
        struct scaledown;

        template <typename Scalar, int Rows, int Cols, int Rowscale, int Colscale>
        struct scaledown <Eigen::Matrix <Scalar, Rows, Cols>, Rowscale, Colscale> {
            using type = Eigen::Matrix <Scalar, Size::div(Rows, Rowscale), Size::div(Cols, Colscale)>;
        };

        /*! @brief 複素化
        */
        template <typename T>
        struct complexify;

        template <typename Scalar, int Rows, int Cols>
        struct complexify <Eigen::Matrix <Scalar, Rows, Cols>>
        {
            using type = Eigen::Matrix <typename std::complexify<Scalar>::type, Rows, Cols>;
        };

        /*! @brief 脱複素化
        */
        template <typename T>
        struct decomplexify;

        template <typename Scalar, int Rows, int Cols>
        struct decomplexify <Eigen::Matrix <Scalar, Rows, Cols>>
        {
            using type = Eigen::Matrix <typename std::decomplexify<Scalar>::type, Rows, Cols>;
        };
    }

    template <typename T>
    using Transpose_t = typename meta::transpose <T>::type;

    template <typename T, int Rowplus, int Colplus>
    using Expand_t = typename meta::expand <T, Rowplus, Colplus>::type;

    template <typename T, int Rowscale, int Colscale>
    using Scaleup_t = typename meta::scaleup <T, Rowscale, Colscale>::type;

    template <typename T, int Rowscale, int Colscale>
    using Scaledown_t = typename meta::scaledown <T, Rowscale, Colscale>::type;
}

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
    using dVector = Vector <double, _Rows>;

    template <int _Cols>
    using dRowVector = RowVector <double, _Cols>;

    /*! @alias
        @brief std::complex<double>型
    */
    template <int _Rows, int _Cols>
    using cMatrix = Matrix <std::complex <double>, _Rows, _Cols>;

    template <int _Rows>
    using cVector = Vector <std::complex <double>, _Rows>;

    template <int _Cols>
    using cRowVector = RowVector <std::complex <double>, _Cols>;
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
        return M.real();
    }

    // template <typename F, int _Rows, int _Cols>
    // Eigen::Matrix <F, _Rows, _Cols> real(const Eigen::Matrix <std::complex <F>, _Rows, _Cols> &M)
    // {
    //     const int                       rows = M.rows();
    //     const int                       cols = M.cols();
    //     Eigen::Matrix <F, _Rows, _Cols> retval;
    //     for (int i = 0; i < rows; i++) {
    //         for (int j = 0; j < cols; j++) {
    //             retval(i, j) = std::real(M(i, j));
    //         }
    //     }
    //
    //     return retval;
    // }

    template <typename _Scalar, int _Rows, int _Cols>
    Eigen::Matrix <_Scalar, _Rows, _Cols> imag(const Eigen::Matrix <_Scalar, _Rows, _Cols> &M)
    {
        return M.imag();
    }

    // template <typename F, int _Rows, int _Cols>
    // Eigen::Matrix <F, _Rows, _Cols> imag(const Eigen::Matrix <std::complex <F>, _Rows, _Cols> &M)
    // {
    //     const int                       rows = M.rows();
    //     const int                       cols = M.cols();
    //     Eigen::Matrix <F, _Rows, _Cols> retval;
    //     for (int i = 0; i < rows; i++) {
    //         for (int j = 0; j < cols; j++) {
    //             retval(i, j) = std::imag(M(i, j));
    //         }
    //     }
    //
    //     return retval;
    // }

    template <typename _Scalar, int _Rows, int _Cols>
    Eigen::Matrix <typename std::decomplexify <_Scalar>::type, _Rows, _Cols> abs(const Eigen::Matrix <_Scalar, _Rows, _Cols> &M)
    {
        const int                                                                rows = M.rows();
        const int                                                                cols = M.cols();
        Eigen::Matrix <typename std::decomplexify <_Scalar>::type, _Rows, _Cols> retval;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                retval(i, j) = std::abs(M(i, j));
            }
        }

        return retval;
    }

    template <typename _Scalar, int _Rows, int _Cols>
    Eigen::Matrix <typename std::decomplexify <_Scalar>::type, _Rows, _Cols> norm(const Eigen::Matrix <_Scalar, _Rows, _Cols> &M)
    {
        const int                                                                rows = M.rows();
        const int                                                                cols = M.cols();
        Eigen::Matrix <typename std::decomplexify <_Scalar>::type, _Rows, _Cols> retval;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                retval(i, j) = std::norm(M(i, j));
            }
        }

        return retval;
    }

    template <typename Derived, typename std::enable_if <std::is_complex <typename Derived::Scalar>::value>::type * = nullptr>
    Eigen::Matrix <typename std::decomplexify <typename Derived::Scalar>::type, Derived::RowsAtCompileTime, Derived::ColsAtCompileTime> arg(const Eigen::MatrixBase <Derived> &M)
    {
        using Scalar = typename Derived::Scalar;
        using Real   = typename std::decomplexify <Scalar>::type;

        return M.array().imag().binaryExpr(M.array().real(), std::ptr_fun <Real, Real, Real>(std::atan2)).matrix();
    }

    /*! @breif ArrayBase::argにはエラーがある．
        内部的にatan2で処理しているため値域は[-PI, PI]
    */
    template <typename Derived, typename std::enable_if <std::is_complex <typename Derived::Scalar>::value>::type * = nullptr>
    Eigen::Array <typename std::decomplexify <typename Derived::Scalar>::type, Derived::RowsAtCompileTime, Derived::ColsAtCompileTime> arg(const Eigen::ArrayBase <Derived> &A)
    {
        using Scalar = typename Derived::Scalar;
        using Real   = typename std::decomplexify <Scalar>::type;

        return A.imag().binaryExpr(A.real(), std::ptr_fun <Real, Real, Real>(std::atan2));
    }

    template <typename _Scalar, int _Rows, int _Cols>
    Eigen::Matrix <_Scalar, _Rows, _Cols> conj(const Eigen::Matrix <_Scalar, _Rows, _Cols> &M)
    {
        return M.conjugate();
    }
}

/*! @brief 行列演算群
*/
namespace Eigen {
    /*! @brief 複素ベクトルの実部と虚部を積み重ねる
    */
    template <typename Derived, int Option = Eigen::ColMajor, typename std::enable_if <std::is_complex<typename Derived::Scalar>::value>::type * = nullptr, typename std::enable_if <Option == Eigen::ColMajor>::type * = nullptr>
    Matrix<typename std::decomplexify<typename Derived::Scalar>::type, Size::multi(Derived::RowsAtCompileTime, 2), Derived::ColsAtCompileTime> Cartesian_form(const MatrixBase <Derived> &M)
    {
        using result_type = Matrix<typename std::decomplexify<typename Derived::Scalar>::type, Size::multi(Derived::RowsAtCompileTime, 2), Derived::ColsAtCompileTime>;
        const int rows = M.rows();
        const int cols = M.cols();
        result_type retval;
        retval.template block<Derived::RowsAtCompileTime, Derived::ColsAtCompileTime>(0, 0, rows, cols) = M.real();
        retval.template block<Derived::RowsAtCompileTime, Derived::ColsAtCompileTime>(rows, 0, rows, cols) = M.imag();
        return retval;
    }

    template <typename Derived, int Option, typename std::enable_if <std::is_complex<typename Derived::Scalar>::value>::type * = nullptr, typename std::enable_if <Option == Eigen::RowMajor>::type * = nullptr>
    Matrix<typename std::decomplexify<typename Derived::Scalar>::type, Derived::RowsAtCompileTime, Size::multi(Derived::ColsAtCompileTime, 2)> Cartesian_form(const MatrixBase <Derived> &M)
    {
        using result_type = Matrix<typename std::decomplexify<typename Derived::Scalar>::type, Derived::RowsAtCompileTime, Size::multi(Derived::ColsAtCompileTime, 2)>;
        const int rows = M.rows();
        const int cols = M.cols();
        result_type retval;
        retval.template block<Derived::RowsAtCompileTime, Derived::ColsAtCompileTime>(0, 0, rows, cols) = M.real();
        retval.template block<Derived::RowsAtCompileTime, Derived::ColsAtCompileTime>(0, cols, rows, cols) = M.imag();
        return retval;
    }

    namespace {
        constexpr int joined_size(const int size1, const int size2)
        {
            if (size1 == Eigen::Dynamic || size2 == Eigen::Dynamic) {
                return Eigen::Dynamic;
            } else {
                return size1 + size2;
            }
        }
    }

    /*! @brief 行列を縦に連結する関数
    */
    template <typename Derived1, typename Derived2,
              typename std::enable_if <std::is_same <typename Derived1::Scalar, typename Derived2::Scalar>::value>::type * = nullptr,
              typename std::enable_if <Derived1::ColsAtCompileTime == Derived2::ColsAtCompileTime>::type *                 = nullptr>
    Matrix <typename Derived1::Scalar, joined_size(Derived1::RowsAtCompileTime, Derived2::RowsAtCompileTime), Derived1::ColsAtCompileTime> vjoin(const MatrixBase <Derived1> &M1, const MatrixBase <Derived2> &M2)
    {
        using Scalar = typename Derived1::Scalar;
        constexpr int Rows1 = Derived1::RowsAtCompileTime;
        constexpr int Rows2 = Derived2::RowsAtCompileTime;
        constexpr int Rows  = joined_size(Rows1, Rows2);
        constexpr int Cols  = Derived1::ColsAtCompileTime;
        using result_type = Matrix <Scalar, Rows, Cols>;

        const int rows1 = M1.rows();
        const int rows2 = M2.rows();
        const int rows  = rows1 + rows2;
        const int cols1 = M1.cols();
        const int cols2 = M2.cols();
        assert(cols1 == cols2);
        const int   cols = cols1;
        result_type retval(rows, cols);
        retval << M1, M2;

        return retval;
    }

    /*! @brief 行列を横に連結する関数
    */
    template <typename Derived1, typename Derived2,
              typename std::enable_if <std::is_same <typename Derived1::Scalar, typename Derived2::Scalar>::value>::type * = nullptr,
              typename std::enable_if <Derived1::RowsAtCompileTime == Derived2::RowsAtCompileTime>::type *                 = nullptr>
    Matrix <typename Derived1::Scalar, Derived1::RowsAtCompileTime, joined_size(Derived1::ColsAtCompileTime, Derived2::ColsAtCompileTime)> hjoin(const MatrixBase <Derived1> &M1, const MatrixBase <Derived2> &M2)
    {
        using Scalar = typename Derived1::Scalar;
        constexpr int Rows  = Derived1::RowsAtCompileTime;
        constexpr int Cols1 = Derived1::ColsAtCompileTime;
        constexpr int Cols2 = Derived2::ColsAtCompileTime;
        constexpr int Cols  = joined_size(Cols1, Cols2);
        using result_type = Matrix <Scalar, Rows, Cols>;

        const int rows1 = M1.rows();
        const int rows2 = M2.rows();
        assert(rows1 == rows2);
        const int   rows  = rows1;
        const int   cols1 = M1.cols();
        const int   cols2 = M2.cols();
        const int   cols  = cols1 + cols2;
        result_type retval(rows, cols);
        retval << M1, M2;

        return retval;
    }

    /*! @brief 行列を2*2に連結する関数
    */
    template <typename Derived1, typename Derived2, typename Derived3, typename Derived4,
              typename std::enable_if <std::is_same <typename Derived1::Scalar, typename Derived2::Scalar>::value>::type * = nullptr,
              typename std::enable_if <std::is_same <typename Derived1::Scalar, typename Derived3::Scalar>::value>::type * = nullptr,
              typename std::enable_if <std::is_same <typename Derived1::Scalar, typename Derived4::Scalar>::value>::type * = nullptr,
              typename std::enable_if <Derived1::RowsAtCompileTime == Derived2::RowsAtCompileTime>::type *                 = nullptr,
              typename std::enable_if <Derived3::RowsAtCompileTime == Derived4::RowsAtCompileTime>::type *                 = nullptr,
              typename std::enable_if <Derived1::ColsAtCompileTime == Derived3::ColsAtCompileTime>::type *                 = nullptr,
              typename std::enable_if <Derived2::ColsAtCompileTime == Derived4::ColsAtCompileTime>::type *                 = nullptr>
    Matrix <typename Derived1::Scalar, joined_size(Derived1::RowsAtCompileTime, Derived3::RowsAtCompileTime), joined_size(Derived1::ColsAtCompileTime, Derived2::ColsAtCompileTime)> quadjoin(const MatrixBase <Derived1> &M1, const
                                                                                                                                                                                              MatrixBase <Derived2> &M2, const
                                                                                                                                                                                              MatrixBase <Derived3> &M3, const
                                                                                                                                                                                              MatrixBase <Derived4> &M4)
    {
        using Scalar = typename Derived1::Scalar;
        constexpr int Rows1 = Derived1::RowsAtCompileTime;
        constexpr int Rows2 = Derived2::RowsAtCompileTime;
        constexpr int Rows3 = Derived3::RowsAtCompileTime;
        constexpr int Rows4 = Derived4::RowsAtCompileTime;
        constexpr int Rows  = joined_size(Rows1, Rows3);
        constexpr int Cols1 = Derived1::ColsAtCompileTime;
        constexpr int Cols2 = Derived2::ColsAtCompileTime;
        constexpr int Cols3 = Derived3::ColsAtCompileTime;
        constexpr int Cols4 = Derived4::ColsAtCompileTime;
        constexpr int Cols  = joined_size(Cols1, Cols2);
        using result_type = Matrix <Scalar, Rows, Cols>;

        const int rows1 = M1.rows();
        const int rows2 = M2.rows();
        const int rows3 = M3.rows();
        const int rows4 = M4.rows();
        assert(rows1 == rows2);
        assert(rows3 == rows4);
        const int rows  = rows1 + rows3;
        const int cols1 = M1.cols();
        const int cols2 = M2.cols();
        const int cols3 = M3.cols();
        const int cols4 = M4.cols();
        assert(cols1 == cols3);
        assert(cols2 == cols4);
        const int   cols = cols1 + cols2;
        result_type retval(rows, cols);
        retval << M1, M2, M3, M4;

        return retval;
    }

    /*! @brief k番目の標準基底ベクトルを返す関数．
        Dynamicの場合は第二引数でサイズを指定する
    */
    template <typename T, int n, typename std::enable_if <std::is_integral <T>::value>::type * = nullptr>
    Vector <T, n> standard_base(const int k, const int size=n)
    {
        Vector <T, n> retval = Vector <T, n>::Zero(size);
        retval(k) = 1;

        return retval;
    }

    template <typename T, int n, typename std::enable_if <std::is_floating_point <T>::value>::type * = nullptr>
    Vector <T, n> standard_base(const int k, const int size=n)
    {
        Vector <T, n> retval = Vector <T, n>::Zero(size);
        retval(k) = 1.;

        return retval;
    }

    template <typename T, int n, typename std::enable_if <std::is_complex <T>::value>::type * = nullptr>
    Vector <T, n> standard_base(const int k, const int size=n)
    {
        Vector <T, n> retval = Vector <T, n>::Zero(size);
        retval(k).real(1.);

        return retval;
    }

    /*! @brief 行列のエルミート転置を返す関数
    */
    template <typename _Scalar, int _Rows, int _Cols>
    Eigen::Matrix <_Scalar, _Cols, _Rows> Hermite(const Eigen::Matrix <_Scalar, _Rows, _Cols> &M)
    {
        return M.adjoint();
    }

    /*! @brief 行列を対称化する関数
    */
    template <typename Derived>
    Derived symmentrize(const Eigen::MatrixBase <Derived> &M)
    {
        return (M + M.transpose()) / 2;
    }

    /*! @brief 行列を歪対称化する関数
    */
    template <typename Derived>
    Derived antisymmetrise(const Eigen::MatrixBase <Derived> &M)
    {
        return (M - M.transpose()) / 2;
    }

    /*! @brief 行列をエルミート化する関数
    */
    template <typename Derived>
    Derived Hermitize(const Eigen::MatrixBase <Derived> &M)
    {
        return (M + M.adjoint()) / 2;
    }

    /*! @brief 行列を歪エルミート化する関数
    */
    template <typename Derived>
    Derived antiHermitize(const Eigen::MatrixBase <Derived> &M)
    {
        return (M - M.adjoint()) / 2;
    }

    /*! @brief 行列のMoore-Penrose型一般逆行列を求める関数
        @param M 行列
    */
    template <typename Derived>
    Transpose_t <Derived> pinverse(const MatrixBase <Derived> &M)
    {
        constexpr int Rows = Derived::RowsAtCompileTime;
        constexpr int Cols = Derived::ColsAtCompileTime;
        constexpr int Min  = cpstd::min(Rows, Cols);
        const int     rows = M.rows();
        const int     cols = M.cols();
        const int     min  = std::min(rows, cols);
        const int     max  = std::max(rows, cols);

        Eigen::JacobiSVD <Derived> svd(M, ComputeFullU | ComputeFullV);
        typename Eigen::JacobiSVD <Derived>::SingularValuesType singular_values_inv = svd.singularValues();
        const double epsilon = 1.e-6;

        for (size_t i = 0; i < min; i++) {
            if (singular_values_inv(i) < epsilon) {
                singular_values_inv(i) = 0.;
            } else {
                singular_values_inv(i) = 1. / singular_values_inv(i);
            }
        }

        Transpose_t <Derived> invSigma(Transpose_t <Derived>::Zero(cols, rows));
        invSigma.template block <Min, Min>(0, 0, min, min) = singular_values_inv.template cast<typename Derived::Scalar>().asDiagonal();

        return svd.matrixV() * invSigma * svd.matrixU().adjoint();
    }

    /*! @brief 行列のk番目（0が最大，n-1が最小）の特異値を返す関数
        @param M 行列
        @param k 求める特異値の序列
    */
    template <typename _Scalar, int _Rows, int _Cols>
    typename std::decomplexify <_Scalar>::type singular_value(const Eigen::Matrix <_Scalar, _Rows, _Cols> &M, const int k)
    {
        Eigen::JacobiSVD <Eigen::Matrix <_Scalar, _Rows, _Cols> > svd(M);

        return svd.singularValues().eval()(k);
    }

    template <typename _Scalar, int _Rows, int _Cols>
    Eigen::Vector <typename std::decomplexify <_Scalar>::type, _Cols> singular_values(const Eigen::Matrix <_Scalar, _Rows, _Cols> &M)
    {
        Eigen::JacobiSVD <Eigen::Matrix <_Scalar, _Rows, _Cols> > svd(M);

        return svd.singularValues();
    }

    /*! @brief 行列のk番目の固有値を返す関数
        @param M 行列
        @param k 求める固有値の序列
    */
    template <typename _Scalar, int _Rows, int _Cols>
    typename std::complexify <_Scalar>::type eigenvalue(const Eigen::Matrix <_Scalar, _Rows, _Cols> &M, const int k)
    {
        // todo: 順序は適当
        Eigen::EigenSolver <Eigen::Matrix <_Scalar, _Rows, _Cols> > es(M);

        return es.eigenvalues().eval()(k);
    }

    template <typename _Scalar, int _Rows, int _Cols>
    Eigen::Vector <typename std::complexify <_Scalar>::type, _Cols> eigenvalues(const Eigen::Matrix <_Scalar, _Rows, _Cols> &M)
    {
        // todo: 順序は適当
        Eigen::EigenSolver <Eigen::Matrix <_Scalar, _Rows, _Cols> > es(M);

        return es.eigenvalues();
    }

    /*! @brief エルミート行列のk番目の固有値（0が最大，n-1が最小）を返す関数
        @param M 行列
        @param k 求める固有値の序列
    */
    template <typename _Scalar, int _Rows, int _Cols>
    typename std::decomplexify <_Scalar>::type selfadjoint_eigenvalue(const Eigen::Matrix <_Scalar, _Rows, _Cols> &M, const int k)
    {
        assert(M.rows() == M.cols());
        Eigen::SelfAdjointEigenSolver <Eigen::Matrix <_Scalar, _Rows, _Cols> > es(M);
        if (es.info() != Eigen::Success) {
            assert(false);

            return 0;
        }

        return es.eigenvalues().eval()(M.rows() - 1 - k);
    }

    template <typename _Scalar, int _Rows, int _Cols>
    Eigen::Vector <typename std::decomplexify <_Scalar>::type, _Cols> selfadjoint_eigenvalues(const Eigen::Matrix <_Scalar, _Rows, _Cols> &M)
    {
        assert(M.rows() == M.cols());
        Eigen::SelfAdjointEigenSolver <Eigen::Matrix <_Scalar, _Rows, _Cols> > es(M);
        if (es.info() != Eigen::Success) {
            assert(false);

            return 0;
        }

        return es.eigenvalues();
    }

    /*! @brief 行列の正定値性を判定する関数
        @param M エルミート行列
    */
    template <typename _Scalar, int _Rows, int _Cols>
    bool positive_definite(const Eigen::Matrix <_Scalar, _Rows, _Cols> &M)
    {
        assert(M.rows() == M.cols());
        Eigen::SelfAdjointEigenSolver <Eigen::Matrix <_Scalar, _Rows, _Cols> > es(M);
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
        Eigen::SelfAdjointEigenSolver <Eigen::Matrix <_Scalar, _Rows, _Cols> > es(M);
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
        Eigen::SelfAdjointEigenSolver <Eigen::Matrix <_Scalar, _Rows, _Cols> > es(M);
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
        Eigen::SelfAdjointEigenSolver <Eigen::Matrix <_Scalar, _Rows, _Cols> > es(M);
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
        Q_type       Q = Eigen::HouseholderQR <M_type>(M).householderQ();
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
