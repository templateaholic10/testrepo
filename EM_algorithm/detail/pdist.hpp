#ifndef DETAIL_PDIST
#define DETAIL_PDIST

#include "../pdist.hpp"

namespace statistic {
    /* --- 正規分布の実装 --- */

    template <int dim>
    Probability_distribution <dim, GAUSSIAN>::Probability_distribution(const dvector <dim> &mu, const dmatrix <dim> &A)
        : _mu(mu), _A(A), _sigma(boost::numeric::ublas::prod(A, boost::numeric::ublas::trans(A))), _sigmaDeterminant(matrix_util::determinant(_sigma)), _rnd(), _mt(_rnd()), _stdnorm(0., 1.)
    {
        matrix_util::invert(_sigma, _sigmaInverse);
        // 変換行列Aがフルランクであることの確認
    }

    // 確率密度関数
    template <int dim>
    double Probability_distribution <dim, GAUSSIAN>::pdf(const dvector <dim> &x) const
    {
        return pnorm(x, _mu, _sigmaInverse, _sigmaDeterminant);
    }

    // データ生成関数
    template <int dim>
    void Probability_distribution <dim, GAUSSIAN>::generate(dvector <dim> &data) const
    {
        dvector <dim> z;
        // 多変量標準正規分布を生成
        for (int i = 0; i < dim; i++) {
            z(i) = _stdnorm(_mt);
        }
        data = _A * (z - _mu);
    }

    // 書き出し
    template <int dim>
    template <FORMAT format>
    void Probability_distribution <dim, GAUSSIAN>::output(std::ostream &os, const Range <dim> &range, double mesh) const
    {
        char delim(formatToDelim(format));

        dvector <dim> x1(range.x1());
        dvector <dim> x2(range.x2());
        dvector <dim> x;

        os << "# --- average mu ---" << std::endl;
        for (int i = 0; i < dim; i++) {
            os << _mu(i) << ((i != dim - 1) ? delim : '\0');
        }
        os << std::endl;
        os << "# --- variance sigma ---" << std::endl;
        for (int i = 0; i < dim; i++) {
            for (int j = 0; j < dim; j++) {
                os << _sigma(i, j) << ((j != dim - 1) ? delim : '\0');
            }
            os << std::endl;
        }
        os << std::endl;
    }

    // パラメータの書き出し
    template <int dim>
    template <FORMAT format>
    void Probability_distribution <dim, GAUSSIAN>::outparam(std::ostream &os) const
    {
        char delim(formatToDelim(format));

        os << "# --- average mu ---" << std::endl;
        for (int i = 0; i < dim; i++) {
            os << _mu(i) << ((i != dim - 1) ? delim : '\n');
        }
        os << std::flush;
        os << "# --- variance sigma ---" << std::endl;
        for (int i = 0; i < dim; i++) {
            for (int j = 0; j < dim; j++) {
                os << _sigma(i, j) << ((j != dim - 1) ? delim : '\n');
            }
            os << std::flush;
        }
        os << std::endl;
    }

    template <class distribution>
    void test()
    {
    }

    template <>
    void test <GAUSSIAN>()
    {
        constexpr int dim = 2;  // 2次元
        using PD = Probability_distribution <dim, GAUSSIAN>;

        // パラメータ群
        dvector <dim> mu;
        mu(0) = 1.; mu(1) = 2.;

        dmatrix <dim> A;
        A(0, 0) = 1.; A(0, 1) = 1.;
        A(1, 0) = 0.; A(1, 1) = 2.;

        // 確率分布
        PD pd(mu, A);
    }
}

#endif
