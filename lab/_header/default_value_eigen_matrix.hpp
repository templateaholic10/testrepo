/*! @file
    @brief Eigen::Matrixのデフォルト値を定義
    @auther yanteyon10
    @date 11/10
*/

#ifndef DEFAULT_VALUE_EIGEN_MATRIX_HPP
#define DEFAULT_VALUE_EIGEN_MATRIX_HPP

#include "../default_value.hpp"
#include <Eigen/Core>

namespace util {
    template <typename T, int m, int n>
    struct Default_value<Eigen::Matrix<T, m, n>, void>
    {
        static Eigen::Matrix<T, m, n> eval()
        {
            return Eigen::Matrix<T, m, n>::Zero();
        }
    };
}

#endif
