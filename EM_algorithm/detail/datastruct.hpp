#ifndef DETAIL_DATA_STRUCT
#define DETAIL_DATA_STRUCT
#include "../data_struct.hpp"

namespace statistic {
    template <int dim, int num>
    Data_series <dim, num>::Data_series(std::istream &datain, const statistic_util::FORMAT &format)
    {
        char delim;
        std::string line;
        for (int t = 0; t < num; t++) {
            for (int i = 0; i < dim; i++) {
                datain >> _x[t](i);
                if (format == statistic_util::FORMAT::CSV_COMMA && i != dim - 1) {
                    datain >> delim;
                }
            }
            getline(datain, line);
        }
    }

    template <int dim, int num>
    template <class distribution>
    Data_series <dim, num>::Data_series(PD <dim, distribution> &pd)
    {
        for (int t = 0; t < num; t++) {
            _x[t] = pd.generate();
        }
    }

    template <int dim, int num>
    void Data_series <dim, num>::output(std::ostream &os, const statistic_util::FORMAT &format) const
    {
        const char delim = statistic_util::formatToDelim(format);
        for (int t = 0; t < num; t++) {
            for (int i = 0; i < dim; i++) {
                os << _x[t](i) << ((i != dim - 1) ? delim : '\n');
            }
        }
    }

    template <int dim, int num>
    const std::array<dvector<dim>, num>& Data_series <dim, num>::dataset() const
    {
        return _x;
    }

    void test_data_series_gaussian()
    {
        constexpr int dim = 2;  // 2次元
        constexpr int num = 1000;
        using PD = Probability_distribution <dim, GAUSSIAN>;
        using DS = Data_series <dim, num>;
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
        fout.open("test_ds_gaussian.param");
        pd.outparam(fout, format);
        fout.close();
        statistic_util::Range <dim> range({ -10., -10. }, { 10., 10. });
        fout.open("test_ds_gaussian.csv");
        pd.output <20, 20>(fout, range, format);
        fout.close();

        // データ
        DS ds(pd);

        // 出力
        fout.open("test_ds_gaussian.dataset");
        ds.output(fout, format);
        fout.close();
    }

    void test_data_series_gaussian_mixtures()
    {
        constexpr int dim         = 2; // 2次元
        constexpr int mixture_num = 2;  // 混合数
        constexpr int num = 1000;
        using PD = Probability_distribution <dim, GAUSSIAN_MIXTURES <mixture_num> >;
        using DS = Data_series <dim, num>;
        constexpr statistic_util::FORMAT format = statistic_util::FORMAT::CSV_COMMA;
        std::ofstream                    fout;

        // パラメータ群
        std::array <double, mixture_num> pi = { 0.5, 0.5 };

        std::array <dvector <dim>, mixture_num> mus;
        mus[0](0) = 0.; mus[0](1) = 0.;
        mus[1](0) = 10.; mus[1](1) = 0.;

        std::array <dmatrix <dim>, mixture_num> As;
        As[0](0, 0) = 1.; As[0](0, 1) = 0.;
        As[0](1, 0) = 0.; As[0](1, 1) = 1.;
        As[1](0, 0) = 1.; As[1](0, 1) = 0.;
        As[1](1, 0) = 0.; As[1](1, 1) = 1.;

        // 確率分布
        PD pd(pi, mus, As);

        // 出力
        fout.open("test_ds_gaussian_mixtures.param");
        pd.outparam(fout, format);
        fout.close();
        statistic_util::Range <dim> range({ -10., -10. }, { 10., 10. });
        fout.open("test_ds_gaussian_mixtures.csv");
        pd.output <20, 20>(fout, range, format);
        fout.close();

        // データ
        DS ds(pd);

        // 出力
        fout.open("test_ds_gaussian_mixtures.dataset");
        ds.output(fout, format);
        fout.close();
    }
}
#endif
