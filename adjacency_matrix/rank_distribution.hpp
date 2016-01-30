/*! @file
    @brief 隣接行列のランクの分布を計算する関数
    @author yanteyon10
    @date 11/18
*/
#ifndef RANK_DISTRIBUTION_HPP
#define RANK_DISTRIBUTION_HPP

#include <array>
#include "adjacency_matrix.hpp"

namespace graph {
    /*! @brief 隣接行列における辺のインデックスを進める内部関数
    破壊的なので絶対に外で使わない
    */
    void _index_forward(const int i, const int j, int &i_next, int &j_next)
    {
        if (i - j == 1) {
            i_next = i + 1;
            j_next = 0;
        } else {
            i_next = i;
            j_next = j + 1;
        }
    }

    /*! @brief 隣接行列のランクの分布を調べる関数，のDFS内部関数
    */
    template <int n>
    void _rank_distribution(const double lower_bound, const double upper_bound, const Adjacency_matrix <n> &M, const int i, const int j, std::array <int, n + 1> &retval)
    {
        // すべて未確定の状態，すべて確定の状態でともにコールされる
        // 密度が条件を満たしていなければ弾く
        // 残りがすべて0のときの密度
        const double lower = static_cast <double>(M.Esize()) / Adjacency_matrix <n>::max_Esize;
        if (upper_bound <= lower) {
            return;
        }
        // 残りがすべて1のときの密度
        const double upper = static_cast <double>(M.Esize() + ((n - i) * (n - 1) - j)) / Adjacency_matrix <n>::max_Esize;
        if (upper < lower_bound) {
            return;
        }
        // インデックス外のとき，全要素が確定
        if (i >= n) {
            // 自身のランクを調べる
            retval[M.rank()]++;

            return;
        }
        // indexだけ進めて再帰する（indexビットは0）
        int i_next;
        int j_next;
        _index_forward(i, j, i_next, j_next);
        _rank_distribution(lower_bound, upper_bound, M, i_next, j_next, retval);
        // コピーし，indexビットを1にして再帰する
        auto L = M;
        L.at(i, j) = 1;
        L.at(j, i) = 1;
        _rank_distribution(lower_bound, upper_bound, L, i_next, j_next, retval);
    }

    /*! @brief 隣接行列のランクの分布を調べる関数
        @param lower_bound [lower_bound, upper_bound)
        @param upper_bound [lower_bound, upper_bound)
    */
    template <int n>
    std::array <int, n + 1> rank_distribution(const double lower_bound=0., const double upper_bound=1.1)
    {
        std::array <int, n + 1> retval;
        retval.fill(0);
        Adjacency_matrix <n> M(0);
        _rank_distribution(lower_bound, upper_bound, M, 1, 0, retval);

        return retval;
    }
}

#endif
