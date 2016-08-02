/*! @file
    @brief Eigen::SparseMatrix I/O
    @author templateaholic10
    @date 8/2
*/
#ifndef EIGENSPARSE_IO_HPP
#define EIGENSPARSE_IO_HPP

#include <Eigen/Sparse>
#include <vector>
#include <iostream>
#include <cstdlib>

/*! @enum
    @brief 疎行列の表示方法
*/
namespace Eigen {
    enum SparseFormat
    {
        RAW, // 密行列と同じ
        COO, // 座標リスト(coordinate list)
    };
}

namespace Eigen {
    namespace {
        template <typename T>
        using SMatrix = Eigen::SparseMatrix <T>;
    }
}

/*! @macro
    @brief デリミタ．インクルード前にEIGEN_IO_DELIMを定義することで変更が可能
*/
#ifndef EIGEN_IO_DELIM
    #define EIGEN_IO_DELIM ','
#endif

// 演算子をすべての名前空間から探索するため，グローバルにおく

/*! @brief Eigen::Matrixの高度抽出関数．きちんと初期化してから使うように
    @param os 出力ストリーム
    @param M 行列
    @param format 疎行列の表現形式
    @param delim デリミタ
*/
template <typename Elem>
std::ostream &out(std::ostream &os, const Eigen::SMatrix <Elem> &M, const Eigen::SparseFormat format=Eigen::COO, const char delim=EIGEN_IO_DELIM)
{
    const int _m = M.rows();
    const int _n = M.cols();
    if (_m == 0 || _n == 0) {
        return os;
    }
    if (format == Eigen::COO) {
        // 座標リスト形式
        bool first = true;
        for (int j = 0; j < M.outerSize(); ++j) {
            for (typename Eigen::SMatrix <Elem>::InnerIterator it(M, j); it; ++it)
            {
                if (first) {
                    os << it.row() << delim << it.col() << delim << it.value();
                    first = false;
                } else {
                    os << std::endl << it.row() << delim << it.col() << delim << it.value();
                }
            }
        }
    }

    return os;
}

/*! @brief Eigen::Matrixの高度挿入関数．きちんと初期化してから使うように
    @param is 入力ストリーム
    @param M 行列
    @param format 疎行列の表現形式
    @param delim デリミタ
*/
template <typename Elem>
std::istream &in(std::istream &is, Eigen::SMatrix <Elem> &M, const Eigen::SparseFormat format=Eigen::COO, const char delim=EIGEN_IO_DELIM)
{
    using Tri_type = Eigen::Triplet<Elem>;
    const int _m = M.rows();
    const int _n = M.cols();
    if (_m == 0 || _n == 0) {
        return is;
    }
    if (format == Eigen::COO) {
        // 座標リスト形式
        std::vector<Tri_type> trivec;
        trivec.reserve(std::max(2*std::min(_m, _n), 1000));

        int row, col;
        Elem val;

        while (is) {
            is >> row;
            is.ignore(1, delim);
            is >> col;
            is.ignore(1, delim);
            is >> val;
            is.ignore(1, delim);
            is.ignore(0, '\n');
            trivec.push_back(Tri_type(row, col, val));
        }

        M.setFromTriplets(trivec.begin(), trivec.end());
    }

    return is;
}

/*! @brief Eigen::SparseMatrix抽出子．きちんと初期化してから使うように
*/
// template <typename Elem>
// std::ostream&operator<<(std::ostream &os, const Eigen::SMatrix <Elem> &M)
// {
//     return out(os, M);
// }

/*! @brief Eigen::Matrix挿入子．きちんと初期化してから使うように
*/
// template <typename Elem, int m, int n>
// std::istream&operator>>(std::istream &is, Eigen::Matrix <Elem, m, n> &M)
// {
//     return in(is, M);
// }

#endif
