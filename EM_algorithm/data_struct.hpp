#pragma once
#include <iostream>
#include "statistic_util.hpp"

namespace statistic {

    template <int dim, int num> struct Data_series;

    template <int dim, int num>
    struct Data_series
    {
        Data_series() = delete;
        Data_series(std::istream &datain);  // ファイルから読み込む場合
        Data_series(const Generator &generator);  // 生成器から作る場合

        template <FORMAT format>
        void output(std::ostream &os) const;  // ファイルに書き出す

        void test();

        int _dim;
        int _num;
        std::array<dvector<dim>, num> _x;  // 次元はdim，要素数はnum
    };

}
