/*! @file
    @brief 統計におけるユーティリティ関数群（特にEigenを使用するもの）
    @author templateaholic10
    @date 10/31
*/
#ifndef UTIL_STOCHASTIC
#define UTIL_STOCHASTIC

#include <exeigen>

namespace Eigen {
    /*! @brief 中心化行列
    */
    template <typename T, int n, int nx = Eigen::Dynamic>
    Matrix<T, n, n> Centering() {
        const int size = (n == Eigen::Dynamic) ? nx : n;
        return Matrix<T, n, n>::Identity(size, size) - (Matrix<T, n, n>::Ones(size, size) * Matrix<T, n, n>::Ones(size, size).transpose()) / size;
    }
}

namespace stochastic {
    /*! @alias
        @brief データフレーム
    */
    template <typename _Scalar, int _Rows, int _Cols>
    using Data_Frame = Eigen::Matrix <_Scalar, _Rows, _Cols>;

    /*! @brief データ行列の平均ベクトルを返す関数
        @param X データ行列
        @return 平均ベクトル
    */
    template <typename _Scalar, int _Rows, int _Cols>
    Eigen::Matrix<_Scalar, 1, _Cols> mean(const Eigen::Matrix <_Scalar, _Rows, _Cols>& X)
    {
        const int n = X.rows();
        return Eigen::Matrix<_Scalar, 1, _Rows>::Ones(n) * X / n;
    }

    /*! @brief データ行列を中心化する関数．ほとんどの場合pよりnが大きく，中心化行列Matrix<T, n, n>をスタックに確保できないため，中心化行列を使わない
        @param X データ行列
        @return 中心化したデータ行列
    */
    template <typename _Scalar, int _Rows, int _Cols>
    Eigen::Matrix <_Scalar, _Rows, _Cols> centering(const Eigen::Matrix <_Scalar, _Rows, _Cols>& X)
    {
        const int n = X.rows();
        return X - Eigen::Matrix <_Scalar, _Rows, 1>::Ones(n) * mean(X);
    }

    /*! @brief データ行列の共分散行列を返す関数
        @param X データ行列
        @return 共分散行列
    */
    template <typename _Scalar, int _Rows, int _Cols>
    Eigen::Matrix <_Scalar, _Cols, _Cols> covariance(const Eigen::Matrix <_Scalar, _Rows, _Cols>& X)
    {
        const int n = X.rows();
        const Eigen::Matrix <_Scalar, _Rows, _Cols> centered = centering(X);
        return (Eigen::Hermite(centered) * centered) / n;
    }

    /*! @brief データ行列の相互相関行列（信号処理），つまり原点周りの2次モーメント行列を返す関数
        @param X データ行列
        @return 相互相関行列
    */
    template <typename _Scalar, int _Rows, int _Cols>
    Eigen::Matrix <_Scalar, _Cols, _Cols> second_moment(const Eigen::Matrix <_Scalar, _Rows, _Cols>& X)
    {
        const int n = X.rows();
        return (Eigen::Hermite(X) * X) / n;
    }
}

#endif
