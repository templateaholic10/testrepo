#ifndef DETAIL_PDIST
#define DETAIL_PDIST

#include "../pdist.hpp"

namespace statistic {

    template <int dim, class distribution>
    void test();

    /* --- 正規分布の実装 --- */

    template <int dim>
    Probability_distribution <dim, GAUSSIAN>::Probability_distribution(const dvector <dim> &mu, const dmatrix <dim> &A)
        : _mu(mu), _A(A), _sigma(A * boost::numeric::ublas::trans(A)), _sigmaDeterminant(matrix_util::determinant(_sigma)), _mt(std::random_device rnd, rnd()), _stdnorm(0., 1.)
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
        dvector<dim> z();
        // 多変量標準正規分布を生成
        for (int i = 0; i < dim; i++) {
            z(i) = _stdnorm(_mt);
        }
        data = _A * (z - _mu);
        return;
    }

    // 書き出し
    template <int dim, FORMAT format>
    void Probability_distribution <dim, GAUSSIAN>::output <format>(std::ostream &os) const
    {
        char delim(formatToDelim(format));

        os << "# --- average mu ---" << std::endl;
        for (int i = 0; i < dim; i++) {
            os << _mu(i) << ((i != dim - 1) ? delim : '');
        }
        os << std::endl;
        os << "# --- variance sigma ---" << std::endl;
        for (int i = 0; i < dim; i++) {
            for (int j = 0; j < dim; j++) {
                os << _sigma(i, j) << ((j != dim - 1) ? delim : '');
            }
            os << std::endl;
        }
        os << std::endl;
        return;
    }

    template <>
    void test<GAUSSIAN>()
    {
        constexpr int dim(2);  // 2次元
        using PD = Probability_distribution<dim, GAUSSIAN>;

        // パラメータ群
        dvector <dim> mu({1., 1.});



        PD pd()


    }
}

#endif
