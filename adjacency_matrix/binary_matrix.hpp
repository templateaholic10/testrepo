/*! @file
    @brief 0-1行列クラス
    @author yanteyon10
    @date 11/18
*/
#ifndef BINARY_MATRIX_HPP
#define BINARY_MATRIX_HPP

#include <iostream>
#include <cassert>
#include <bitset>
#include <Eigen/Core>
#include <Eigen/LU>

namespace Eigen {
    // フレンド関数のためのプロトタイプ宣言
    template <int n>
    class Binary_matrix;

    /*! @class 0-1行列を表すクラス
        @tparam n サイズ
    */
    template <int n>
    class Binary_matrix {
    public:
        using value_type = bool;
        static constexpr int size = n;
    private:
        std::bitset <n *n> bits;
    public:
        static char delim;
    public:
        /*! @brief コンストラクタ
        */
        Binary_matrix(int num=0)
            : bits(num)
        {
        }

        /*! @brief 初期化関数
        */
        void init(int num=0)
        {
            bits(num);
        }

        /*! @brief ゲッター
        */
        bool operator()(int i, int j) const
        {
            assert(0 <= i && i < n);
            assert(0 <= j && j < n);

            return bits[i * n + j];
        }

        /*! @brief 境界チェックをしないゲッター
        */
        bool at(int i, int j) const
        {
            return bits[i * n + j];
        }

        /*! @brief セッター
        */
        typename std::bitset <n *n>::reference operator()(int i, int j)
        {
            assert(0 <= i && i < n);
            assert(0 <= j && j < n);

            return bits[i * n + j];
        }

        /*! @brief 境界チェックをしないセッター
        */
        typename std::bitset <n *n>::reference at(int i, int j)
        {
            return bits[i * n + j];
        }

        /*! @brief 1の数を返す関数
        */
        int count() const
        {
            return bits.count();
        }

        /*! @brief 階数を返す関数
        */
        int rank() const;

        template <int _n>
        friend std::ostream&operator<<(std::ostream &os, const Binary_matrix <_n> &M);

        template <int _n>
        friend std::istream&operator>>(std::istream &is, Binary_matrix <_n> &M);
    };

    template <int n>
    char Binary_matrix <n>::delim = ',';

    template <int n>
    std::ostream&operator<<(std::ostream &os, const Eigen::Binary_matrix <n> &M)
    {
        const char delim = Eigen::Binary_matrix <n>::delim;
        os << M.bits[0];
        for (int j = 1; j < n; j++) {
            os << delim << M.bits[j];
        }
        for (int i = 1; i < n; i++) {
            os << std::endl << M.bits[i * n];
            for (int j = 1; j < n; j++) {
                os << delim << M.bits[i * n + j];
            }
        }

        return os;
    }

    template <int n>
    std::istream&operator>>(std::istream &is, Eigen::Binary_matrix <n> &M)
    {
        const char delim = Eigen::Binary_matrix <n>::delim;
        int        buffer;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
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

    /*! @brief Binary_matrixからEigen::Matrixへの変換関数
    */
    template <int n, typename T = int>
    Eigen::Matrix <T, n, n> matrix(const Binary_matrix <n> &M)
    {
        Eigen::Matrix <T, n, n> retval(Eigen::Matrix <T, n, n>::Zero());
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (M(i, j)) {
                    retval(i, j) = 1;
                }
            }
        }

        return retval;
    }

    template <int n>
    int Binary_matrix <n>::rank() const
    {
        Eigen::Matrix <double, n, n>                     M = Eigen::matrix <n, double>(*this);
        Eigen::FullPivLU <Eigen::Matrix <double, n, n> > lu(M);

        return lu.rank();
    }
}

#endif
