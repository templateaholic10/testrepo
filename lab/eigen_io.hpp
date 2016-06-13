/*! @file
    @brief Eigen::matrix I/O
    @author templateaholic10
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

// 演算子をすべての名前空間から探索するため，グローバルにおく

/*! @brief Eigen::Matrixの高度抽出関数．Eigen::Dynamic行列はきちんと初期化してから使うように
    @param os 出力ストリーム
    @param M 行列
    @param stand 行列を立体で表現するか，1列で表現するか
    @param delim デリミタ
*/
template <typename Elem, int m, int n>
std::ostream &out(std::ostream &os, const Eigen::Matrix <Elem, m, n> &M, const bool stand=true, const char delim=EIGEN_IO_DELIM)
{
    const int _m = M.rows();
    const int _n = M.cols();
    if (_m == 0 || _n == 0) {
        return os;
    }
    os << M(0, 0);
    for (int j = 1; j < _n; j++) {
        os << delim << M(0, j);
    }
    for (int i = 1; i < _m; i++) {
        if (stand) {
            os << std::endl << M(i, 0);
        } else {
            os << delim << M(i, 0);
        }
        for (int j = 1; j < _n; j++) {
            os << delim << M(i, j);
        }
    }

    return os;
}

/*! @brief Eigen::Matrixの高度挿入関数．Eigen::Dynamic行列はきちんと初期化してから使うように
    @param is 入力ストリーム
    @param M 行列
    @param delim デリミタ
*/
template <typename Elem, int m, int n>
std::istream &in(std::istream &is, Eigen::Matrix <Elem, m, n> &M, const char delim=EIGEN_IO_DELIM)
{
    const int _m = M.rows();
    const int _n = M.cols();
    if (_m == 0 || _n == 0) {
        return is;
    }
    is >> M(0, 0);
    for (int j = 1; j < _n; j++) {
        is.ignore(1, delim);
        is >> M(0, j);
    }
    for (int i = 1; i < _m; i++) {
        for (int j = 0; j < _n; j++) {
            is.ignore(1, delim);
            is >> M(i, j);
        }
    }

    return is;
}

/*! @brief Eigen::Matrix抽出子．Eigen::Dynamic行列はきちんと初期化してから使うように
*/
template <typename Elem, int m, int n>
std::ostream&operator<<(std::ostream &os, const Eigen::Matrix <Elem, m, n> &M)
{
    return out(os, M);
}

/*! @brief Eigen::Matrix挿入子．Eigen::Dynamic行列はきちんと初期化してから使うように
*/
template <typename Elem, int m, int n>
std::istream&operator>>(std::istream &is, Eigen::Matrix <Elem, m, n> &M)
{
    return in(is, M);
}

#endif
