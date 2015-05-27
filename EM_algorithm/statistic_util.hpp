#pragma once
#include <iostream>
#include "matrix_util.hpp"

namespace statistic {

    // ファイルフォーマット
    enum class FORMAT {CSV_SPACE, CSV_COMMA};

    // エイリアステンプレート
    template <int dim>
    using dvector = boost::numeric::ublas::bounded_vector<double, dim>;

    template <int dim>
    using dmatrix = boost::numeric::ublas::bounded_matrix<double, dim, dim>;

    // 多次元正規分布のpdf
    template <int dim>
    double pnorm(const dvector<dim>& x, const dvector<dim>& mu, const dmatrix<dim>& sigmaInverse, double sigmaDeterminant);

    // （内部的に使用）多次元正規分布の正規化子
    template <int dim>
    constexpr double normalize();

    // 混合正規分布の対数尤度
    // 混合率とデータごと，分布ごとの確率pを与える
    template <int num, int mixture_num>
    double logL(const std::array<double, mixture_num>& pi, const std::array<std::array<double, mixture_num>, num>& p);

}
