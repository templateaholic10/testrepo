#ifndef DETAIL_PDIST
#define DETAIL_PDIST

#include "../statistic_util.hpp"
#include "../pdist.hpp"
#include "../matrix_util.hpp"

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
    dvector <dim> Probability_distribution <dim, GAUSSIAN>::generate()
    {
        dvector <dim> z;
        // 多変量標準正規分布を生成
        for (int i = 0; i < dim; i++) {
            z(i) = _stdnorm(_mt);
        }

        return std::move(_mu + boost::numeric::ublas::prod(_A, z));
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

    // 疎結合性を考えればこれはいらない．

    // 書き出し
    // template <>
    // template <int ... Meshes>
    // void Probability_distribution <2, GAUSSIAN>::output(std::ostream &os, const statistic_util::Range <2> &range, const statistic_util::FORMAT format) const
    // {
    //     constexpr int dim = 2;
    //     // 指定するメッシュ数の数は次元と一致する．
    //     static_assert(dim == sizeof ... (Meshes), "mesh dimension error!");
    //     const char delim = formatToDelim(format);
    //
    //     using namespace std::placeholders;
    //     const auto nonmemberpdf = [&](const dvector <dim> &x) {
    //                                   double exponent = -0.5 * boost::numeric::ublas::inner_prod((x - _mu), boost::numeric::ublas::prod(_sigmaInverse, (x - _mu)));
    //
    //                                   return statistic_util::normalize <dim>() / sqrt(_sigmaDeterminant) * exp(exponent);
    //                               };
    //     statistic_util::expand_array <1, double, Meshes ...> M = statistic_util::Descretize <Meshes ...>::descretizeDV(nonmemberpdf, range);
    //     os << "# --- pdf ----" << std::endl;
    //     if (format == statistic_util::FORMAT::CSV_COMMA_SQ) {
    //         statistic_util::outputSQ(os, M, range, delim);
    //     } else {
    //         os << "# --- range ---" << std::endl;
    //         statistic_util::output(os, range);
    //
    //         os << std::endl;
    //         constexpr std::array <int, dim> meshes = util::make_array <int>(Meshes ...);
    //         os << "# --- meshes ---" << std::endl;
    //         for (int i = 0; i < dim; i++) {
    //             os << meshes[i] << ((i != dim - 1) ? '*' : '\n');
    //         }
    //
    //         os << std::endl;
    //         auto out = [&](double d) {
    //                        os << d << std::endl;
    //                    };
    //         util::Apply <double, dim, void>::apply(M, out);
    //     }
    // }

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

    void testgaussian()
    {
        constexpr int dim = 2;  // 2次元
        using PD = Probability_distribution <dim, GAUSSIAN>;
        constexpr statistic_util::FORMAT format = statistic_util::FORMAT::CSV_COMMA;
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

    /* --- 混合正規分布の実装 --- */

    template <int dim, int mixture_num>
    Probability_distribution <dim, GAUSSIAN_MIXTURES <mixture_num> >::Probability_distribution(const std::array <double, mixture_num> &pi, const std::array <dvector <dim>, mixture_num> &mus, const std::array <dmatrix <dim>, mixture_num> &As)
        : _pi(pi), _mus(mus), _As(As), _rnd(), _mt(_rnd()), _stdnorm(0., 1.), _mixvoter(0, mixture_num - 1)
    {
        // todo :
        // std::array<dvector<2>, 2> vs;
        // vs[0] = matrix_util::make_bounded_vector(util::make_array<double>(1., 0.));
        // vs[1] = matrix_util::make_bounded_vector(util::make_array<double>(0., 1.));
        // auto twice = [](const dvector<2> _v) -> dvector<2>{return 2 * _v;};
        // auto doubled = util::Apply<dvector<2>, 1>::apply(vs, twice);
        // static_assert(std::is_same<decltype(doubled), std::array<dvector<2>, 2>>::value, "apply type error!");

        // std::array<dmatrix<2>, 2> ms;
        // ms[0](0,0) = 1.; ms[0](0,1) = 0.;
        // ms[0](1,0) = 0.; ms[0](1,1) = 1.;
        // ms[1](0,0) = 1.; ms[1](0,1) = 0.;
        // ms[1](1,0) = 0.; ms[1](1,1) = 1.;
        // auto twice = [](const dmatrix<2> _M) -> double{return _M(0,0)*_M(1,1) - _M(0,1)*_M(1,0);};
        // auto deters = util::Apply<dmatrix<2>, 1>::apply(ms, statistic_util::transToSigmaD2);
        // static_assert(std::is_same<decltype(deters), std::array<double, 2>>::value, "apply type error!");
        // auto lefttop = [](const dmatrix<dim> &_M)->double{return _M(0,0);};
        // auto lefttops = util::Apply<dmatrix<dim>, 1>::apply(As, lefttop);
        // static_assert(std::is_same<decltype(lefttops), std::array<double, mixture_num>>::value, "apply type error!");
        // auto inv = matrix_util::transToSigma(ms);
        // static_assert(std::is_same<decltype(inv), decltype(ms)>::value, "apply type error!");

        auto local_transToSigma = [](const dmatrix<dim>& _M) -> dmatrix<dim> {
            return std::move(boost::numeric::ublas::prod(_M, boost::numeric::ublas::trans(_M)));
        };
        auto local_invert = [](const dmatrix<dim>& _M) -> dmatrix<dim> {
            namespace ublas = boost::numeric::ublas;

            dmatrix<dim> lhs(_M);
            ublas::matrix <double>       rhs(ublas::identity_matrix <double>(_M.size1()));
            ublas::permutation_matrix <> pm(_M.size1());

            ublas::lu_factorize(lhs, pm);
            ublas::lu_substitute(lhs, pm, rhs);

            BOOST_UBLAS_CHECK(rhs.size1() == _M.size1(), ublas::internal_logic());
            BOOST_UBLAS_CHECK(rhs.size2() == _M.size2(), ublas::internal_logic());

            #if BOOST_UBLAS_TYPE_CHECK
                BOOST_UBLAS_CHECK(
                    ublas::detail::expression_type_check(
                        ublas::prod(_M, rhs),
                        ublas::identity_matrix <typename dmatrix<dim>::value_type>(_M.size1())
                        ),
                    ublas::internal_logic()
                    );
            #endif

            // >>>>>>>>>>
            /*
            mi.resize(rhs.size1(), rhs.size2(), false);
            mi.assign(rhs);
            // mi.assign_temporary(rhs);
            */

            // ----------
            return std::move(rhs);
            // <<<<<<<<<<
        };
        auto local_determinant = [](const dmatrix<dim>& _M) -> double {
            namespace ublas = boost::numeric::ublas;

            dmatrix<dim>       lu(_M);
            ublas::permutation_matrix <> pm(_M.size1());

            ublas::lu_factorize(lu, pm);

            double det(1.);

            using size_type = ublas::permutation_matrix <>::size_type;

            for (size_type i = 0; i < pm.size(); ++i) {
                det *= (i == pm(i)) ? +lu(i, i) : -lu(i, i);
            }

            return det;
        };

        _sigmas = util::Apply <dmatrix <dim>, 1>::apply(As, local_transToSigma);
        _sigmaInverses = util::Apply <dmatrix <dim>, 1>::apply(_sigmas, local_invert);
        _sigmaDeterminants = util::Apply <dmatrix <dim>, 1>::apply(_sigmas, local_determinant);

        double sum = 0.;
        for (auto p : pi) {
            sum += p;
        }
        assert(fabs(1. - sum) < statistic_util::epsilon);
        // 変換行列Aがフルランクであることの確認
    }

    // 確率密度関数
    template <int dim, int mixture_num>
    double Probability_distribution <dim, GAUSSIAN_MIXTURES <mixture_num> >::pdf(const dvector <dim> &x) const
    {
        double sum = 0.;
        for (int i = 0; i < mixture_num; i++) {
            sum += _pi[i] * pnorm(x, _mus[i], _sigmaInverses[i], _sigmaDeterminants[i]);
        }

        return sum;
    }

    // データ生成関数
    template <int dim, int mixture_num>
    dvector <dim> Probability_distribution <dim, GAUSSIAN_MIXTURES <mixture_num> >::generate()
    {
        using namespace boost::numeric;

        // まずどの分布から選択されるかを選択．
        int mixindex = _mixvoter(_mt);
        // std::cout << mixindex << std::endl;  // 確認

        dvector <dim> z;
        // 多変量標準正規分布を生成
        for (int i  = 0; i < dim; i++) {
            z(i) = _stdnorm(_mt);
        }

        return std::move(_mus[mixindex] + boost::numeric::ublas::prod(_As[mixindex], z));
    }

    // 書き出し
    template <int dim, int mixture_num>
    template <int ... Meshes>
    void Probability_distribution <dim, GAUSSIAN_MIXTURES <mixture_num> >::output(std::ostream &os, const statistic_util::Range <dim> &range, const statistic_util::FORMAT format) const
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
                                      using namespace boost::numeric;
                                      double sum = 0.;
                                      double exponent;
                                      for (int i = 0; i < mixture_num; i++) {
                                          exponent = -0.5 * ublas::inner_prod((x - _mus[i]), ublas::prod(_sigmaInverses[i], (x - _mus[i])));

                                          sum += _pi[i] * statistic_util::normalize <dim>() / sqrt(_sigmaDeterminants[i]) * exp(exponent);
                                      }

                                      return sum;
                                  };
        statistic_util::expand_array <1, double, Meshes ...> M = statistic_util::Descretize <Meshes ...>::descretizeDV(nonmemberpdf, range);
        os << "# --- pdf ----" << std::endl;

        auto out = [&](double d) {
                       os << d << std::endl;
                   };
        util::Apply <double, dim, void>::apply(M, out);
    }

    // 疎結合性を考えればこれはいらない．

    // 書き出し
    // template <int mixture_num>
    // template <int ... Meshes>
    // void Probability_distribution <2, GAUSSIAN_MIXTURES <mixture_num> >::output(std::ostream &os, const statistic_util::Range <2> &range, const statistic_util::FORMAT format) const
    // {
    //     constexpr int dim = 2;
    //     // 指定するメッシュ数の数は次元と一致する．
    //     static_assert(dim == sizeof ... (Meshes), "mesh dimension error!");
    //     const char delim = formatToDelim(format);
    //
    //     using namespace std::placeholders;
    //     const auto nonmemberpdf = [&](const dvector <dim> &x) {
    //                                   using namespace boost::numeric;
    //                                   double sum = 0.;
    //                                   double exponent;
    //                                   for (int i = 0; i < mixture_num; i++) {
    //                                       exponent = -0.5 * ublas::inner_prod((x - _mus[i]), ublas::prod(_sigmaInverses[i], (x - _mus[i])));
    //
    //                                       sum += _pi[i] * statistic_util::normalize <dim>() / sqrt(_sigmaDeterminants[i]) * exp(exponent);
    //                                   }
    //
    //                                   return sum;
    //                               };
    //     statistic_util::expand_array <1, double, Meshes ...> M = statistic_util::Descretize <Meshes ...>::descretizeDV(nonmemberpdf, range);
    //     os << "# --- pdf ----" << std::endl;
    //     if (format == statistic_util::FORMAT::CSV_COMMA_SQ) {
    //         statistic_util::outputSQ(os, M, range, delim);
    //     } else {
    //         os << "# --- range ---" << std::endl;
    //         statistic_util::output(os, range);
    //
    //         os << std::endl;
    //         constexpr std::array <int, dim> meshes = util::make_array <int>(Meshes ...);
    //         os << "# --- meshes ---" << std::endl;
    //         for (int i = 0; i < dim; i++) {
    //             os << meshes[i] << ((i != dim - 1) ? '*' : '\n');
    //         }
    //
    //         os << std::endl;
    //         auto out = [&](double d) {
    //                        os << d << std::endl;
    //                    };
    //         util::Apply <double, dim, void>::apply(M, out);
    //     }
    // }

    // パラメータの書き出し
    template <int dim, int mixture_num>
    void Probability_distribution <dim, GAUSSIAN_MIXTURES <mixture_num> >::outparam(std::ostream &os, const statistic_util::FORMAT format) const
    {
        const char delim = formatToDelim(format);

        os << "# --- mixture rate ---" << std::endl;
        for (int j = 0; j < mixture_num; j++) {
            os << _pi[j] << ((j != mixture_num - 1) ? delim : '\n');
        }

        os << std::endl;
        os << "# --- average mus ---" << std::endl;
        for (int j = 0; j < mixture_num; j++) {
            statistic_util::output <dim>(os, _mus[j], delim);
        }

        os << std::endl;
        os << "# --- variance sigmas ---" << std::endl;
        for (int j = 0; j < mixture_num; j++) {
            statistic_util::output <dim>(os, _sigmas[j], delim);
        }
    }

    void testgaussian_mixtures()
    {
        constexpr int dim         = 2; // 2次元
        constexpr int mixture_num = 2;  // 混合数
        using PD = Probability_distribution <dim, GAUSSIAN_MIXTURES <mixture_num> >;
        constexpr statistic_util::FORMAT format = statistic_util::FORMAT::CSV_COMMA;
        std::ofstream                    fout;

        // パラメータ群
        std::array <double, mixture_num> pi = { 0.5, 0.5 };

        std::array <dvector <dim>, mixture_num> mus;
        mus[0](0) = 0.; mus[0](1) = 0.;
        mus[1](0) = 10.; mus[1](1) = 0.;

        std::array <dmatrix <dim>, mixture_num> As;
        As[0](0,0) = 1.; As[0](0,1) = 0.;
        As[0](1,0) = 0.; As[0](1,1) = 1.;
        As[1](0,0) = 1.; As[1](0,1) = 0.;
        As[1](1,0) = 0.; As[1](1,1) = 1.;

        // 確率分布
        PD pd(pi, mus, As);

        // 出力
        fout.open("testgaussian_mixtures.param");
        pd.outparam(fout, format);
        fout.close();
        statistic_util::Range <dim> range({ -10., -10. }, { 10., 10. });
        fout.open("testgaussian_mixtures.csv");
        pd.output <20, 20>(fout, range, format);
        fout.close();
    }
}

#endif
