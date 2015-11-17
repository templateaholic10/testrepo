/*! @file
    @brief 隣接行列クラス
    @author yanteyon10
    @date 10/28
*/
#ifndef ADJACENCY_MATRIX_HPP
#define ADJACENCY_MATRIX_HPP

#include <iostream>
#include <cassert>
#include <bitset>
#include <algorithm>
#include <util>
#define EIGEN_NO_DEBUG
#include <util_eigen>

namespace graph {
    // フレンド関数のためのプロトタイプ宣言
    template <size_t n>
    class Binary_matrix;

    /*! @class 0-1行列を表すクラス
        @tparam n サイズ
    */
    template <size_t n>
    class Binary_matrix {
    private:
        std::bitset<n*n> bits;
    public:
        static char delim;
    public:
        Binary_matrix(unsigned int num = 0)
        : bits(num)
        {
        }
        bool operator()(unsigned int i, unsigned int j) const
        {
            assert(0 <= i && i < n);
            assert(0 <= j && j < n);
            return bits[i*n+j];
        }
        typename std::bitset<n*n>::reference operator()(unsigned int i, unsigned int j)
        {
            assert(0 <= i && i < n);
            assert(0 <= j && j < n);
            return bits[i*n+j];
        }
        template <size_t _n>
        friend std::ostream& operator<<(std::ostream& os, const Binary_matrix<_n>& M);
        template <size_t _n>
        friend std::istream& operator>>(std::istream& is, Binary_matrix<_n>& M);
    };

    template <size_t n>
    char Binary_matrix<n>::delim = ',';

    template <size_t n>
    std::ostream& operator<<(std::ostream& os, const Binary_matrix<n>& M)
    {
        const char delim = Binary_matrix<n>::delim;
        os << M.bits[0];
        for (size_t j = 1; j < n; j++) {
            os << delim << M.bits[j];
        }
        for (size_t i = 1; i < n; i++) {
            os << std::endl << M.bits[i*n];
            for (size_t j = 1; j < n; j++) {
                os << delim << M.bits[i*n+j];
            }
        }
        return os;
    }

    template <size_t n>
    std::istream& operator>>(std::istream& is, Binary_matrix<n>& M)
    {
        const char delim = Binary_matrix<n>::delim;
        unsigned int buffer;
        for (size_t i = 0; i < n; i++) {
            for (size_t j = 0; j < n; j++) {
                is >> buffer;
                if (buffer == 0) {
                    M(i, j) = 0;
                } else {
                    M(i, j) = 1;
                }
                is.ignore(1, delim);
            }
        }
        return is;
    }

    /*! @class 自己閉路と並行辺を持たない無向グラフの接続行列を表すクラス
        辺の有無を01で表現してstd::bitsetに格納する
        @tparam n グラフの頂点数
    */
    template <size_t n>
    class Adjacency_matrix : public Binary_matrix<n>{
    public:
        /*! @brief 隣接行列かどうか（0対角，対称か）自己診断する関数
        */
        bool self_check() const
        {
            // 対角成分
            for (size_t k = 0; k < n; k++) {
                if (Binary_matrix<n>::operator()(k, k) != 0) {
                    return false;
                }
            }
            // 非対角成分
            for (size_t i = 1; i < n; i++) {
                for (size_t j = 0; j < i; j++) {
                    if (Binary_matrix<n>::operator()(i, j) != Binary_matrix<n>::operator()(j, i)) {
                        return false;
                    }
                }
            }
            return true;
        }
        /*! @brief 自身の上三角成分を利用して自身を対称化する関数
        */
        void Ubuild()
        {
            // 対角成分
            for (size_t k = 0; k < n; k++) {
                Binary_matrix<n>::operator()(k, k) = 0;
            }
            // 非対角成分
            for (size_t i = 1; i < n; i++) {
                for (size_t j = 0; j < i; j++) {
                    Binary_matrix<n>::operator()(i, j) = Binary_matrix<n>::operator()(j, i);
                }
            }
        }
        /*! @brief 自身の下三角成分を利用して自身を対称化する関数
        */
        void Lbuild()
        {
            // 対角成分
            for (size_t k = 0; k < n; k++) {
                Binary_matrix<n>::operator()(k, k) = 0;
            }
            // 非対角成分
            for (size_t i = 1; i < n; i++) {
                for (size_t j = 0; j < i; j++) {
                    Binary_matrix<n>::operator()(j, i) = Binary_matrix<n>::operator()(i, j);
                }
            }
        }
    };
}

#endif
