/*! @file
    @brief Eigen::matrix I/O
    @author yanteyon10
    @date 11/18
*/
#ifndef EIGEN_IO_HPP
#define EIGEN_IO_HPP

#include <Eigen/Core>
#include <iostream>
#include <cstdlib>

/*! @macro
    @brief デリミタ．インクルード前にEIGEN_IO_DELIMを定義することで変更が可能
*/
#ifndef EIGEN_IO_DELIM
#define EIGEN_IO_DELIM ','
#endif

/*! @brief Eigen::Matrix抽出子
*/
template <typename Elem, int m, int n>
std::ostream& operator<<(std::ostream& os, const Eigen::Matrix<Elem, m, n>& M)
{
    if (m == 0 || n == 0) {
        return os;
    }
    os << M(0, 0);
    for (int j = 1; j < n; j++) {
        os << EIGEN_IO_DELIM << M(0, j);
    }
    for (int i = 1; i < m; i++) {
        os << std::endl << M(i, 0);
        for (int j = 1; j < n; j++) {
            os << EIGEN_IO_DELIM << M(i, j);
        }
    }
    return os;
}

template <typename Elem, int m>
std::ostream& operator<<(std::ostream& os, const Eigen::Matrix<Elem, m, Eigen::Dynamic>& M)
{
    const int n = M.cols();
    if (m == 0 || n == 0) {
        return os;
    }
    os << M(0, 0);
    for (int j = 1; j < n; j++) {
        os << EIGEN_IO_DELIM << M(0, j);
    }
    for (int i = 1; i < m; i++) {
        os << std::endl << M(i, 0);
        for (int j = 1; j < n; j++) {
            os << EIGEN_IO_DELIM << M(i, j);
        }
    }
    return os;
}

template <typename Elem, int n>
std::ostream& operator<<(std::ostream& os, const Eigen::Matrix<Elem, Eigen::Dynamic, n>& M)
{
    const int m = M.rows();
    if (m == 0 || n == 0) {
        return os;
    }
    os << M(0, 0);
    for (int j = 1; j < n; j++) {
        os << EIGEN_IO_DELIM << M(0, j);
    }
    for (int i = 1; i < m; i++) {
        os << std::endl << M(i, 0);
        for (int j = 1; j < n; j++) {
            os << EIGEN_IO_DELIM << M(i, j);
        }
    }
    return os;
}

template <typename Elem>
std::ostream& operator<<(std::ostream& os, const Eigen::Matrix<Elem, Eigen::Dynamic, Eigen::Dynamic>& M)
{
    const int m = M.rows();
    const int n = M.cols();
    if (m == 0 || n == 0) {
        return os;
    }
    os << M(0, 0);
    for (int j = 1; j < n; j++) {
        os << EIGEN_IO_DELIM << M(0, j);
    }
    for (int i = 1; i < m; i++) {
        os << std::endl << M(i, 0);
        for (int j = 1; j < n; j++) {
            os << EIGEN_IO_DELIM << M(i, j);
        }
    }
    return os;
}

/*! @brief Eigen::Matrix挿入子
*/
template <typename Elem, int m, int n>
std::istream& operator>>(std::istream& is, Eigen::Matrix<Elem, m, n>& M)
{
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            is >> M(i, j);
            is.ignore(1, EIGEN_IO_DELIM);
        }
    }
    return is;
}

template <typename Elem, int m>
std::istream& operator>>(std::istream& is, Eigen::Matrix<Elem, m, Eigen::Dynamic>& M)
{
    const int n = M.cols();
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            is >> M(i, j);
            is.ignore(1, EIGEN_IO_DELIM);
        }
    }
    return is;
}

template <typename Elem, int n>
std::istream& operator>>(std::istream& is, Eigen::Matrix<Elem, Eigen::Dynamic, n>& M)
{
    const int m = M.rows();
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            is >> M(i, j);
            is.ignore(1, EIGEN_IO_DELIM);
        }
    }
    return is;
}

template <typename Elem>
std::istream& operator>>(std::istream& is, Eigen::Matrix<Elem, Eigen::Dynamic, Eigen::Dynamic>& M)
{
    const int m = M.rows();
    const int n = M.cols();
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            is >> M(i, j);
            is.ignore(1, EIGEN_IO_DELIM);
        }
    }
    return is;
}

#endif
