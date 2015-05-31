#pragma once
#include <iostream>
#include <vector>
#include <array>
#include <boost/numeric/ublas/matrix.hpp>         // (1) 普通の行列用のヘッダ
#include <boost/numeric/ublas/triangular.hpp>     // (2) 三角行列用のヘッダ
#include <boost/numeric/ublas/symmetric.hpp>      // (3) 対称行列用のヘッダ
#include <boost/numeric/ublas/hermitian.hpp>      // (4) エルミート行列用のヘッダ
#include <boost/numeric/ublas/matrix_sparse.hpp>  // (5) 疎行列用のヘッダ
#include <boost/numeric/ublas/vector.hpp>         // (6) ベクトル用のヘッダ
#include <boost/numeric/ublas/vector_sparse.hpp>  // (7) 疎ベクトル用のヘッダ
#include <boost/numeric/ublas/lu.hpp>
#include "statistic_util.hpp"

namespace EM {

    template <int dim, int num, int mixture_num> class EM_estimator;

    template <int dim, int num, int mixture_num>
    class EM_estimator
    {
    public:
        EM_estimator() = delete;
        EM_estimator(std::istream &datain, std::istream &initialin);
        ~EM_estimator() = default;

        void estimate();
        void output(std::ostream &estimateout, std::ostream &convergeout) const;

    private:
        void step();
        bool haltCheck() const;

    private:
        int _mixture_num;
        int _step;
        static constexpr double _epsilon = 0.01;

        Data_series<dim, num> _data;
        std::vector<std::array<double, mixture_num>> _pi;  // _pi[step][dist]：ステップ数，分布番号
        std::vector<std::array<dvector<dim>, mixture_num>> _mu;  // _mu[step][dist](i)：ステップ数，分布番号，次元
        std::vector<std::array<dmatrix<dim>, mixture_num>> _sigma;  // _sigma[step][dist](i, j)：ステップ数，分布番号，次元
        std::vector<double> _logL;  // _E_logL[step]：対数尤度の期待値
    };

}
