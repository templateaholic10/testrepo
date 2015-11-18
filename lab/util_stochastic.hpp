/*! @file
    @brief 統計におけるユーティリティ関数群（特にEigenを使用するもの）
    @author yanteyon10
    @date 10/31
*/
#ifndef UTIL_STOCHASTIC
#define UTIL_STOCHASTIC

#include <util>
#include <util_eigen>

namespace Eigen {
    template <int n>
    dMatrix<n, n> Centering() {
        return dMatrix<n, n>::Identity() - (dVector<n>::Ones() * dVector<n>::Ones().transpose()) / n;
    }
}

namespace stochastic {
    /*! @brief データフレーム型
    */
    template <typename T, int n, int p>
    using Data_Frame = Eigen::Matrix <T, n, p>;

    /*! @brief データ行列の平均ベクトルを返す関数
        @param X データ行列
        @return 平均ベクトル
    */
    template <typename T, int n, int p>
    Eigen::RowVector<T, p> mean(const Eigen::Matrix<T, n, p>& X)
    {
        return Eigen::dRowVector<n>::Constant(1./n) * X;
    }

    /*! @brief データ行列を中心化する関数．ほとんどの場合pよりnが大きく，中心化行列Matrix<T, n, n>をスタックに確保できないため，中心化行列を使わない
        @param X データ行列
        @return 中心化したデータ行列
    */
    template <typename T, int n, int p>
    Eigen::Matrix<T, n, p> centering(const Eigen::Matrix<T, n, p>& X)
    {
        return X - Eigen::Vector<T, n>::Ones() * mean(X);
    }

    /*! @brief データ行列の共分散行列を返す関数
        @param X データ行列
        @return 共分散行列
    */
    template <typename T, int n, int p>
    Eigen::Matrix<T, p, p> covariance(const Eigen::Matrix<T, n, p>& X)
    {
        Eigen::Matrix<T, n, p> centered = centering(X);
        return (Eigen::Hermite(centered) * centered) / n;
    }

    /*! @brief データ行列の相互相関行列（信号処理），つまり原点周りの2次モーメント行列を返す関数
        @param X データ行列
        @return 相互相関行列
    */
    template <typename T, int n, int p>
    Eigen::Matrix<T, p, p> second_moment(const Eigen::Matrix<T, n, p>& X)
    {
        return (Eigen::Hermite(X) * X) / n;
    }

    /*! @brief データ行列操作関数のテスト関数
    */
    void test_DM_funcs()
    {
        std::cout << util::Repeat("-", 20) << std::endl;
        std::cout << "TEST FUNCTION: mean, covariance, centering" << std::endl;
        constexpr int p = 3;
        constexpr int n = 4;
        Eigen::dMatrix<n, p> X;
        X << 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12;
        std::cout << "data matrix X: " << std::endl;
        std::cout << X << std::endl;
        std::cout << "mean vector: " << std::endl;
        std::cout << mean(X) << std::endl;
        std::cout << "covariance matrix: " << std::endl;
        std::cout << covariance(X) << std::endl;
        std::cout << "centered matrix: " << std::endl;
        std::cout << centering(X) << std::endl;
        std::cout << util::Repeat("-", 20) << std::endl;
    }
}

#endif
