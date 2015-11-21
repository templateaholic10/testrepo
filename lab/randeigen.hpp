/*! @file
    @brief Eigenでランダム行列を扱うためのユーティリティ関数群
    @author yanteyon10
    @date 11/21
*/
#ifndef RANDEIGEN_HPP
#define RANDEIGEN_HPP

#include <util_rand>
#define EIGEN_NO_DEBUG
#include <Eigen/Dense>

namespace Eigen {
    /*! @brief 各要素が一様分布に従う確率変数によって決定される行列を返す関数
        @tparam rv_type 確率変数の型
        @tparam m 行サイズ
        @tparam n 列サイズ
        @tparam elem_type 要素の型．デフォルトはelem_typeと同じ
        @tparam mx m=Dynamicのときの行サイズ
        @tparam nx n=Dynamicのときの列サイズ
    */
    template <typename rv_type, int m, int n, typename elem_type = rv_type, int mx = Eigen::Dynamic, int nx = Eigen::Dynamic>
    Matrix <elem_type, m, n> rand_uniform(const rv_type &a, const rv_type &b, const std::random_device::result_type seed=std::random_device()())
    {
        const int                rows = (m == Eigen::Dynamic) ? mx : m;
        const int                cols = (n == Eigen::Dynamic) ? nx : n;
        Matrix <elem_type, m, n> retval(rows, cols);
        util::Uniform <rv_type>  uniform(a, b, seed);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                retval(i, j) = uniform();
            }
        }

        return retval;
    }

    /*! @brief 各要素が一様分布に従う確率変数によって決定される行列を返す関数
        @tparam rv_type 確率変数の型
        @tparam m 行サイズ
        @tparam n 列サイズ
        @tparam elem_type 要素の型．デフォルトはelem_typeと同じ
        @tparam mx m=Dynamicのときの行サイズ
        @tparam nx n=Dynamicのときの列サイズ
    */
    template <typename rv_type, int m, int n, typename elem_type = rv_type, int mx = Eigen::Dynamic, int nx = Eigen::Dynamic>
    Matrix <elem_type, m, n> rand_normal(const rv_type &mu, const rv_type &sigma, const std::random_device::result_type seed=std::random_device()())
    {
        const int                rows = (m == Eigen::Dynamic) ? mx : m;
        const int                cols = (n == Eigen::Dynamic) ? nx : n;
        Matrix <elem_type, m, n> retval(rows, cols);
        util::Gaussian <rv_type>  gaussian(mu, sigma, seed);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                retval(i, j) = gaussian();
            }
        }

        return retval;
    }
}

#endif
