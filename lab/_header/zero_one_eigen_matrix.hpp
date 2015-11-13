/*! @file
    @brief Eigen::Matrixのデフォルト値を定義
    @auther yanteyon10
    @date 11/10
*/

#ifndef ZERO_ONE_EIGEN_MATRIX_HPP
#define ZERO_ONE_EIGEN_MATRIX_HPP

#include "../zero_one.hpp"
#include <Eigen/Core>

namespace util {
    template <typename T, int m, int n>
    struct Zero<Eigen::Matrix<T, m, n>, void>
    {
        static Eigen::Matrix<T, m, n> eval()
        {
            return Eigen::Matrix<T, m, n>::Zero();
        }
    };
    template <typename T, int m, int n>
    struct One<Eigen::Matrix<T, m, n>, void>
    {
        static Eigen::Matrix<T, m, n> eval()
        {
            return Eigen::Matrix<T, m, n>::Identity();
        }
    };
}

#endif
