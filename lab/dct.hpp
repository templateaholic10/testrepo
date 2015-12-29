/*! @file
    @brief DCT（離散コサイン変換）．DCT-1とDCT-2がある
    @author yanteyon10
    @date 12/29
*/
#ifndef DCT_HPP
#define DCT_HPP

#include <fft>

namespace Eigen {
    /*! @brief DCT-1用の偶拡張メタ関数．abcdeからabcdedcbを作る
    */
    template <typename T, int Option = Eigen::ColMajor>
    struct Evenize1;

    /*! @brief 縦ベクトル
    */
    template <typename Scalar, int Rows, int Cols>
    struct Evenize1 <Eigen::Matrix<Scalar, Rows, Cols>, Eigen::ColMajor> {
        using type = typename Eigen::Expand<typename Eigen::Scaleup<Matrix<Scalar, Rows, Cols>, 2, 1>::type, -2, 0>::type;
    };

    /*! @brief 横ベクトル
    */
    template <typename Scalar, int Rows, int Cols>
    struct Evenize1 <Eigen::Matrix<Scalar, Rows, Cols>, Eigen::RowMajor> {
        using type = typename Eigen::Expand<typename Eigen::Scaleup<Matrix<Scalar, Rows, Cols>, 1, 2>::type, 0, -2>::type;
    };

    /*! @brief DCT-1用の偶拡張関数．abcdeからabcdedcbを作る
    */
    template <int Option, typename Scalar, int Rows, int Cols, typename std::enable_if<Option == Eigen::ColMajor>::type* = nullptr>
    typename Evenize1<Eigen::Matrix<Scalar, Rows, Cols>, Option>::type evenize1(const Eigen::Matrix<Scalar, Rows, Cols> &M)
    {
        const int rows = M.rows();
        const int cols = M.cols();
        using param_type = Eigen::Matrix<Scalar, Rows, Cols>;
        using result_type = typename Evenize1<param_type, Option>::type;
        result_type retval;
        retval.template block<Rows, Cols>(0, 0, rows, cols) = M;
        retval.template block<Eigen::Size::sub(Rows, 2), Cols>(rows, 0, rows-2, cols) = M.colwise().reverse().template block<Eigen::Size::sub(Rows, 2), Cols>(1, 0, rows-2, cols);
        return retval;
    }

    template <int Option, typename Scalar, int Rows, int Cols, typename std::enable_if<Option == Eigen::RowMajor>::type* = nullptr>
    typename Evenize1<Eigen::Matrix<Scalar, Rows, Cols>, Option>::type evenize1(const Eigen::Matrix<Scalar, Rows, Cols> &M)
    {
        const int rows = M.rows();
        const int cols = M.cols();
        using param_type = Eigen::Matrix<Scalar, Rows, Cols>;
        using result_type = typename Evenize1<param_type, Option>::type;
        result_type retval;
        retval.template block<Rows, Cols>(0, 0, rows, cols) = M;
        retval.template block<Rows, Eigen::Size::sub(Cols, 2)>(0, cols, rows, cols-2) = M.rowwise().reverse().template block<Rows, Eigen::Size::sub(Cols, 2)>(0, 1, rows, cols-2);
        return retval;
    }

    template <typename Scalar, int Rows, int Cols>
    typename Evenize1<Eigen::Matrix<Scalar, Rows, Cols>, Eigen::ColMajor>::type evenize1(const Eigen::Matrix<Scalar, Rows, Cols> &M)
    {
        return evenize1<Eigen::ColMajor>(M);
    }

    /*! @brief DCT-2用の偶拡張メタ関数．abcdeからabcdedcbを作る
    */
    template <typename T, int Option = Eigen::ColMajor>
    struct Evenize2;

    /*! @brief 縦ベクトル
    */
    template <typename Scalar, int Rows, int Cols>
    struct Evenize2 <Eigen::Matrix<Scalar, Rows, Cols>, Eigen::ColMajor> {
        using type = typename Eigen::Scaleup<Matrix<Scalar, Rows, Cols>, 4, 1>::type;
    };

    /*! @brief 横ベクトル
    */
    template <typename Scalar, int Rows, int Cols>
    struct Evenize2 <Eigen::Matrix<Scalar, Rows, Cols>, Eigen::RowMajor> {
        using type = typename Eigen::Scaleup<Matrix<Scalar, Rows, Cols>, 1, 4>::type;
    };

    /*! @brief DCT-2用の偶拡張関数．abcdeから0a0b0c0d0e0e0d0c0b0aを作る
    */
    template <int Option, typename Scalar, int Rows, int Cols, typename std::enable_if<Option == Eigen::ColMajor>::type* = nullptr>
    typename Evenize2<Eigen::Matrix<Scalar, Rows, Cols>, Option>::type evenize2(const Eigen::Matrix<Scalar, Rows, Cols> &M)
    {
        const int rows = M.rows();
        const int cols = M.cols();
        using param_type = Eigen::Matrix<Scalar, Rows, Cols>;
        using result_type = typename Evenize2<param_type, Option>::type;
        result_type retval;
        for (int i = 0; i < rows; i++) {
            retval.row(2*i).fill(0);
            retval.row(2*i+1) = M.row(i);
        }
        for (int i = rows; i < 2*rows; i++) {
            retval.row(2*i).fill(0);
            retval.row(2*i+1) = M.row(2*rows-1-i);
        }
        return retval;
    }

    template <int Option, typename Scalar, int Rows, int Cols, typename std::enable_if<Option == Eigen::RowMajor>::type* = nullptr>
    typename Evenize2<Eigen::Matrix<Scalar, Rows, Cols>, Option>::type evenize2(const Eigen::Matrix<Scalar, Rows, Cols> &M)
    {
        const int rows = M.rows();
        const int cols = M.cols();
        using param_type = Eigen::Matrix<Scalar, Rows, Cols>;
        using result_type = typename Evenize2<param_type, Option>::type;
        result_type retval;
        for (int j = 0; j < cols; j++) {
            retval.col(2*j).fill(0);
            retval.col(2*j+1) = M.col(j);
        }
        for (int j = cols; j < 2*cols; j++) {
            retval.col(2*j).fill(0);
            retval.col(2*j+1) = M.col(2*cols-1-j);
        }
        return retval;
    }

    template <typename Scalar, int Rows, int Cols>
    typename Evenize2<Eigen::Matrix<Scalar, Rows, Cols>, Eigen::ColMajor>::type evenize2(const Eigen::Matrix<Scalar, Rows, Cols> &M)
    {
        return evenize2<Eigen::ColMajor>(M);
    }
}

#endif
