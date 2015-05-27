#include "pdist.hpp"

namespace statistic {

    template <int dim>
    Probability_distribution<dim, GAUSSIAN>::Probability_distribution(const dvector<dim>& mu, const dmatrix<dim>& A)
    : _mu(mu), _A(A), _sigma(A * boost::numeric::ublas::trans(A))
    {
        // 変換行列Aがフルランクであることの確認
    }

    // 確率密度関数
    template <int dim>
    double Probability_distribution<dim, GAUSSIAN>::pdf(const dvector<dim>& x) const
    {
        return double(0.);
    }

    // データ生成関数
    template <int dim>
    void Probability_distribution<dim, GAUSSIAN>::generate(dvector<dim>& data) const
    {
        return;
    }

    // 書き出し
    template <int dim, FORMAT format>
    void Probability_distribution<dim, GAUSSIAN>::output<format>(std::ostream &os)
    {
        return double(0.);
    }

}
