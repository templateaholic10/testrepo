/*! @file
    @brief 隣接行列クラス
    @author yanteyon10
    @date 11/18
*/
#ifndef ADJACENCY_MATRIX_HPP
#define ADJACENCY_MATRIX_HPP

#include "binary_matrix.hpp"

namespace graph {
    // フレンド関数のためのプロトタイプ宣言
    template <int n>
    class Adjacency_matrix;

    /*! @class 自己閉路と並行辺を持たない無向グラフの接続行列を表すクラス
        辺の有無を01で表現してstd::bitsetに格納する
        @tparam n グラフの頂点数
    */
    template <int n>
    class Adjacency_matrix : public Eigen::Binary_matrix <n> {
    public:
        static constexpr int Vsize     = n;
        static constexpr int max_Esize = n * (n - 1) / 2;
    private:
        /*! @brief std::bitsetに着想を得た，セッターレベルでの隣接化を可能にする内部クラス
        */
        class reference {
        private:
            Adjacency_matrix <n> &body;
            const int             i;
            const int             j;
        public:
            reference(Adjacency_matrix <n> &body_, const int i_, const int j_)
                : body(body_), i(i_), j(j_)
            {
            }

            Adjacency_matrix <n>&operator=(int value)
            {
                Eigen::Binary_matrix <n> &base_body = static_cast <Eigen::Binary_matrix <n>&>(body);
                if (i == j) {
                    assert(value == 0);
                    base_body(i, i) = 0;
                } else {
                    base_body(i, j) = value;
                    base_body(j, i) = value;
                }

                return body;
            }
        };

    public:
        /*! @brief コンストラクタ
        */
        Adjacency_matrix(int num=0)
            : Eigen::Binary_matrix <n>(num)
        {
            assert(self_check());
        }

        /*! @brief 初期化関数．コンストラクタと異なり自己診断を行わない
        */
        void init(int num=0)
        {
            Eigen::Binary_matrix <n>::init(num);
        }

        /*! @brief 隣接行列かどうか（0対角，対称か）自己診断する関数
        */
        bool self_check() const
        {
            // 対角成分
            for (int k = 0; k < n; k++) {
                if (Eigen::Binary_matrix <n>::operator()(k, k) != 0) {
                    return false;
                }
            }
            // 非対角成分
            for (int i = 1; i < n; i++) {
                for (int j = 0; j < i; j++) {
                    if (Eigen::Binary_matrix <n>::operator()(i, j) != Eigen::Binary_matrix <n>::operator()(j, i)) {
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
            for (int k = 0; k < n; k++) {
                Eigen::Binary_matrix <n>::operator()(k, k) = 0;
            }
            // 非対角成分
            for (int i = 1; i < n; i++) {
                for (int j = 0; j < i; j++) {
                    Eigen::Binary_matrix <n>::operator()(i, j) = Eigen::Binary_matrix <n>::operator()(j, i);
                }
            }
        }

        /*! @brief 自身の下三角成分を利用して自身を対称化する関数
        */
        void Lbuild()
        {
            // 対角成分
            for (int k = 0; k < n; k++) {
                Eigen::Binary_matrix <n>::operator()(k, k) = 0;
            }
            // 非対角成分
            for (int i = 1; i < n; i++) {
                for (int j = 0; j < i; j++) {
                    Eigen::Binary_matrix <n>::operator()(j, i) = Eigen::Binary_matrix <n>::operator()(i, j);
                }
            }
        }

        /*! @brief 枝数を返す関数
        */
        int Esize() const
        {
            assert(self_check());

            return Eigen::Binary_matrix <n>::count() / 2;
        }

        /*! @brief セッター．速度を優先したいときは基底クラスのat関数（境界チェックなし，隣接化なし）を使用する
        */
        typename Adjacency_matrix <n>::reference operator()(int i, int j)
        {
            assert(0 <= i && i < n);
            assert(0 <= j && j < n);

            return Adjacency_matrix <n>::reference((*this), i, j);
        }

        template <int _n>
        friend std::istream&operator>>(std::istream &is, Adjacency_matrix <_n> &M);
    };

    template <int n>
    std::istream&operator>>(std::istream &is, graph::Adjacency_matrix <n> &M)
    {
        Eigen::Binary_matrix <n> &base_M = static_cast <Eigen::Binary_matrix <n>&>(M);
        std::istream             &retval = Eigen::operator>>(is, base_M);
        assert(M.self_check());

        return retval;
    }
}

#endif
