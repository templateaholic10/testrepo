﻿/*! @file
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

namespace Eigen {
    /*! @brief Eigen::Matrixの高度抽出関数
        @param os 出力ストリーム
        @param M 行列
        @param stand 行列を立体で表現するか，1列で表現するか
        @param delim デリミタ
    */
    template <typename Elem, int m, int n>
    std::ostream& out(std::ostream& os, const Eigen::Matrix<Elem, m, n>& M, const bool stand = true, const char delim = EIGEN_IO_DELIM)
    {
        if (m == 0 || n == 0) {
            return os;
        }
        os << M(0, 0);
        for (int j = 1; j < n; j++) {
            os << delim << M(0, j);
        }
        for (int i = 1; i < m; i++) {
            if (stand) {
                os << std::endl << M(i, 0);
            } else {
                os << delim << M(i, 0);
            }
            for (int j = 1; j < n; j++) {
                os << delim << M(i, j);
            }
        }
        return os;
    }

    template <typename Elem, int m>
    std::ostream& out(std::ostream& os, const Eigen::Matrix<Elem, m, Eigen::Dynamic>& M, const bool stand = true, const char delim = EIGEN_IO_DELIM)
    {
        const int n = M.cols();
        if (m == 0 || n == 0) {
            return os;
        }
        os << M(0, 0);
        for (int j = 1; j < n; j++) {
            os << delim << M(0, j);
        }
        for (int i = 1; i < m; i++) {
            if (stand) {
                os << std::endl << M(i, 0);
            } else {
                os << delim << M(i, 0);
            }
            for (int j = 1; j < n; j++) {
                os << delim << M(i, j);
            }
        }
        return os;
    }

    template <typename Elem, int n>
    std::ostream& out(std::ostream& os, const Eigen::Matrix<Elem, Eigen::Dynamic, n>& M, const bool stand = true, const char delim = EIGEN_IO_DELIM)
    {
        const int m = M.rows();
        if (m == 0 || n == 0) {
            return os;
        }
        os << M(0, 0);
        for (int j = 1; j < n; j++) {
            os << delim << M(0, j);
        }
        for (int i = 1; i < m; i++) {
            if (stand) {
                os << std::endl << M(i, 0);
            } else {
                os << delim << M(i, 0);
            }
            for (int j = 1; j < n; j++) {
                os << delim << M(i, j);
            }
        }
        return os;
    }

    template <typename Elem>
    std::ostream& out(std::ostream& os, const Eigen::Matrix<Elem, Eigen::Dynamic, Eigen::Dynamic>& M, const bool stand = true, const char delim = EIGEN_IO_DELIM)
    {
        const int m = M.rows();
        const int n = M.cols();
        if (m == 0 || n == 0) {
            return os;
        }
        os << M(0, 0);
        for (int j = 1; j < n; j++) {
            os << delim << M(0, j);
        }
        for (int i = 1; i < m; i++) {
            if (stand) {
                os << std::endl << M(i, 0);
            } else {
                os << delim << M(i, 0);
            }
            for (int j = 1; j < n; j++) {
                os << delim << M(i, j);
            }
        }
        return os;
    }

    /*! @brief Eigen::Matrixの高度挿入関数
        @param is 入力ストリーム
        @param M 行列
        @param delim デリミタ
    */
    template <typename Elem, int m, int n>
    std::istream& in(std::istream& is, Eigen::Matrix<Elem, m, n>& M, const char delim = EIGEN_IO_DELIM)
    {
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                is >> M(i, j);
                is.ignore(1, delim);
            }
        }
        return is;
    }

    template <typename Elem, int m>
    std::istream& in(std::istream& is, Eigen::Matrix<Elem, m, Eigen::Dynamic>& M, const char delim = EIGEN_IO_DELIM)
    {
        const int n = M.cols();
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                is >> M(i, j);
                is.ignore(1, delim);
            }
        }
        return is;
    }

    template <typename Elem, int n>
    std::istream& in(std::istream& is, Eigen::Matrix<Elem, Eigen::Dynamic, n>& M, const char delim = EIGEN_IO_DELIM)
    {
        const int m = M.rows();
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                is >> M(i, j);
                is.ignore(1, delim);
            }
        }
        return is;
    }

    template <typename Elem>
    std::istream& in(std::istream& is, Eigen::Matrix<Elem, Eigen::Dynamic, Eigen::Dynamic>& M, const char delim = EIGEN_IO_DELIM)
    {
        const int m = M.rows();
        const int n = M.cols();
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                is >> M(i, j);
                is.ignore(1, delim);
            }
        }
        return is;
    }
}

#endif