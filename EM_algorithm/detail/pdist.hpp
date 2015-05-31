#ifndef DETAIL_PDIST
#define DETAIL_PDIST

#include "../statistic_util.hpp"
#include "../pdist.hpp"

namespace statistic {
    /* --- 正規分布の実装 --- */

    template <int dim>
    Probability_distribution <dim, GAUSSIAN>::Probability_distribution(const dvector <dim> &mu, const dmatrix <dim> &A)
        : _mu(mu), _A(A), _sigma(boost::numeric::ublas::prod(A, boost::numeric::ublas::trans(A))), _sigmaInverse(matrix_util::invert(_sigma)), _sigmaDeterminant(matrix_util::determinant(_sigma)), _rnd(), _mt(_rnd()), _stdnorm(0., 1.)
    {
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
    dvector <dim> Probability_distribution <dim, GAUSSIAN>::generate() const
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
    template <int ... Meshes>
    void Probability_distribution <dim, GAUSSIAN>::output(std::ostream &os, const statistic_util::Range <dim> &range, const statistic_util::FORMAT format) const
    {
        // 指定するメッシュ数の数は次元と一致する．
        static_assert(dim == sizeof ... (Meshes), "mesh dimension error!");
        const char delim = formatToDelim(format);

        os << "# --- range ---" << std::endl;
        statistic_util::output(os, range);

        os << std::endl;
        constexpr auto meshes = util::make_array <int>(Meshes ...);
        os << "# --- meshes ---" << std::endl;
        for (int i = 0; i < dim; i++) {
            os << meshes[i] << ((i != dim - 1) ? '*' : '\n');
        }

        os << std::endl;
        using namespace std::placeholders;
        const auto nonmemberpdf = [&](const dvector <dim> &x) {
                                      double exponent = -0.5 * boost::numeric::ublas::inner_prod((x - _mu), boost::numeric::ublas::prod(_sigmaInverse, (x - _mu)));

                                      return statistic_util::normalize <dim>() / sqrt(_sigmaDeterminant) * exp(exponent);
                                  };
        statistic_util::expand_array <1, double, Meshes ...> M = statistic_util::Descretize <Meshes ...>::descretizeDV(nonmemberpdf, range);
        os << "# --- pdf ----" << std::endl;

        auto out = [&](double d) {
                       os << d << std::endl;
                   };
        util::Apply <double, dim, void>::apply(M, out);
    }

    // 書き出し
    template <>
    template <int ... Meshes>
    void Probability_distribution <2, GAUSSIAN>::output(std::ostream &os, const statistic_util::Range <2> &range, const statistic_util::FORMAT format) const
    {
        constexpr int dim = 2;
        // 指定するメッシュ数の数は次元と一致する．
        static_assert(dim == sizeof ... (Meshes), "mesh dimension error!");
        const char delim = formatToDelim(format);

        using namespace std::placeholders;
        const auto nonmemberpdf = [&](const dvector <dim> &x) {
                                      double exponent = -0.5 * boost::numeric::ublas::inner_prod((x - _mu), boost::numeric::ublas::prod(_sigmaInverse, (x - _mu)));

                                      return statistic_util::normalize <dim>() / sqrt(_sigmaDeterminant) * exp(exponent);
                                  };
        statistic_util::expand_array <1, double, Meshes ...> M = statistic_util::Descretize <Meshes ...>::descretizeDV(nonmemberpdf, range);
        os << "# --- pdf ----" << std::endl;
        if (format == statistic_util::FORMAT::CSV_COMMA_SQ) {
            statistic_util::outputSQ(os, M, range, delim);
        } else {
            os << "# --- range ---" << std::endl;
            statistic_util::output(os, range);

            os << std::endl;
            constexpr std::array <int, dim> meshes = util::make_array <int>(Meshes ...);
            os << "# --- meshes ---" << std::endl;
            for (int i = 0; i < dim; i++) {
                os << meshes[i] << ((i != dim - 1) ? '*' : '\n');
            }

            os << std::endl;
            auto out = [&](double d) {
                           os << d << std::endl;
                       };
            util::Apply <double, dim, void>::apply(M, out);
        }
    }

    // パラメータの書き出し
    template <int dim>
    void Probability_distribution <dim, GAUSSIAN>::outparam(std::ostream &os, const statistic_util::FORMAT format) const
    {
        const char delim = formatToDelim(format);

        os << "# --- average mu ---" << std::endl;
        statistic_util::output <dim>(os, _mu, delim);

        os << std::endl;
        os << "# --- variance sigma ---" << std::endl;
        statistic_util::output <dim>(os, _sigma, delim);
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
        constexpr statistic_util::FORMAT format = statistic_util::FORMAT::CSV_COMMA_SQ;
        std::ofstream                    fout;

        // パラメータ群
        dvector <dim> mu = matrix_util::make_bounded_vector <double, dim>(util::make_array <double>(0., 0.));

        dmatrix <dim> A = matrix_util::make_bounded_matrix <double, dim, dim>(util::make_common_array(
                                                                                  util::make_common_array(1., 0.),
                                                                                  util::make_common_array(0., 1.)
                                                                                  ));

        // 確率分布
        PD pd(mu, A);

        // 出力
        fout.open("testgaussian.param");
        pd.outparam(fout, format);
        fout.close();
        statistic_util::Range <dim> range({ -10., -10. }, { 10., 10. });
        fout.open("testgaussian.csv");
        pd.output <20, 20>(fout, range, format);
        fout.close();
    }
}

#endif
