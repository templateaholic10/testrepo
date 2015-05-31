#ifndef DETAIL_PDIST
#define DETAIL_PDIST

#include "../statistic_util.hpp"
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
    dvector<dim> Probability_distribution <dim, GAUSSIAN>::generate() const
    {
        dvector <dim> z;
        // 多変量標準正規分布を生成
        for (int i = 0; i < dim; i++) {
            z(i) = _stdnorm(_mt);
        }
        return std::move(boost::numeric::ublas::prod(_A, (z - _mu)));
    }

    // 書き出し
    template <int dim>
    template <int... Meshes>
    void Probability_distribution <dim, GAUSSIAN>::output(std::ostream& os, const Range <dim>& range, const FORMAT format) const
    {
        // 指定するメッシュ数の数は次元と一致する．
        static_assert(dim == sizeof...(Meshes), "mesh dimension error!");
        char delim(formatToDelim(format));

        os << "# --- range ---" << std::endl;
        statistic::output(os, range);

        auto meshes = util::make_array<int>(Meshes...);
        os << "# --- meshes ---" << std::endl;
        os << meshes[0];
        for (int i = 1; i < dim; i++) {
            os << " * " << meshes[i];
        }

        auto M = Descretize<Meshes...>::descretize(pdf, range);
        os << "# --- pdf ----" << std::endl;
        auto out = [&](double d){os << d << std::endl;};
        util::Apply<double, dim, void>::apply(M, out);
    }

    // パラメータの書き出し
    template <int dim>
    void Probability_distribution <dim, GAUSSIAN>::outparam(std::ostream& os, const FORMAT format) const
    {
        const char delim = formatToDelim(format);

        os << "# --- average mu ---" << std::endl;
        output(os, _mu, delim);
        os << std::endl;
        os << "# --- variance sigma ---" << std::endl;
        output(os, _sigma, delim);
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
        FORMAT format = FORMAT::CSV_COMMA;
        std::ofstream fout;

        // パラメータ群
        dvector <dim> mu;
        mu(0) = 1.; mu(1) = 2.;

        dmatrix <dim> A;
        A(0, 0) = 1.; A(0, 1) = 1.;
        A(1, 0) = 0.; A(1, 1) = 2.;

        // 確率分布
        PD pd(mu, A);

        // 出力
        fout.open("testgaussian.param");
        pd.outparam(fout, format);
        fout.close();
        Range<dim> range({-10., 10.}, {-10., 10.});
        fout.open("testgaussian.pdf");
        pd.output<100, 100>(fout, range, format);
        fout.close();
    }
}

#endif
