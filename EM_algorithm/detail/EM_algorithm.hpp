#ifndef DETAIL_EM_ALGORITHM
#define DETAIL_EM_ALGORITHM

#include <fstream>
#include "../EM_algorithm.hpp"

namespace EM {
    template <int dim, int mixture_num>
    template <int num>
    std::tuple <typename EM_estimator <dim, statistic::GAUSSIAN_MIXTURES <mixture_num> >::Record, double> EM_estimator <dim, statistic::GAUSSIAN_MIXTURES <mixture_num> >::update(const typename EM_estimator <dim,
                                                                                                                                                                                                               statistic::GAUSSIAN_MIXTURES <
                                                                                                                                                                                                                   mixture_num> >::Record&
                                                                                                                                                                                  record, const statistic::Data_series <dim, num> &data_series)
    {
        auto pi     = std::get <0>(record);
        auto mus    = std::get <1>(record);
        auto sigmas = std::get <2>(record);

        // 中間生成物p[t][dist]
        util::multi_array <double, num, mixture_num> p;
        dvector <dim>                                mu;
        dmatrix <dim>                                sigmaInverse;
        double                                       sigmaDeterminant;
        for (int dist = 0; dist < mixture_num; dist++) {
            mu               = mus[dist];
            sigmaInverse     = matrix_util::invert(sigmas[dist]);
            sigmaDeterminant = matrix_util::determinant(sigmas[dist]);
            for (int t = 0; t < num; t++) {
                p[t][dist] = statistic_util::pnorm <dim>(data_series.dataset()[t], mu, sigmaInverse, sigmaDeterminant);
            }
        }

        // 中間生成物gamma[t][dist]
        util::multi_array <double, num, mixture_num> gamma;
        for (int t = 0; t < num; t++) {
            double denominator = 0.;
            for (int dist = 0; dist < mixture_num; dist++) {
                denominator += pi[dist] * p[t][dist];
            }
            for (int dist = 0; dist < mixture_num; dist++) {
                gamma[t][dist] = pi[dist] * p[t][dist] / denominator;
            }
        }

        // 中間生成物gammaのデータに関する和
        std::array <double, mixture_num> gammaSum;
        for (int dist = 0; dist < mixture_num; dist++) {
            double sum = 0.;
            for (int t = 0; t < num; t++) {
                sum += gamma[t][dist];
            }
            gammaSum[dist] = sum;
        }

        using namespace boost::numeric;

        // 混合率_pi
        std::array <double, mixture_num> new_pi;
        for (int dist = 0; dist < mixture_num; dist++) {
            new_pi[dist] = gammaSum[dist] / num;
        }

        // 平均_mu
        std::array <dvector <dim>, mixture_num> new_mus;
        dvector <dim>                           sumvector;
        for (int dist = 0; dist < mixture_num; dist++) {
            sumvector = dvector <dim>();
            for (int t = 0; t < num; t++) {
                sumvector += gamma[t][dist] * data_series.dataset()[t];
            }
            new_mus[dist] = sumvector / gammaSum[dist];
        }

        // 分散_sigma
        std::array <dmatrix <dim>, mixture_num> new_sigmas;
        dmatrix <dim>                           summatrix;
        for (int dist = 0; dist < mixture_num; dist++) {
            summatrix = dmatrix <dim>();
            for (int t = 0; t < num; t++) {
                summatrix += gamma[t][dist] * ublas::outer_prod(data_series.dataset()[t], data_series.dataset()[t]);
            }
            new_sigmas[dist] = summatrix / gammaSum[dist] - ublas::outer_prod(new_mus[dist], new_mus[dist]);
        }

        // 対数尤度_logL
        // データごとの対数尤度の和を求める．
        double logL;
        double sum_of_logL = 0.;
        for (int t = 0; t < num; t++) {
            // データごとの，分布ごとの尤度の和を求める．
            double sum_of_L_par_data = 0.;
            for (int dist = 0; dist < mixture_num; dist++) {
                sum_of_L_par_data += pi[dist] * p[t][dist];
            }
            sum_of_logL += log(sum_of_L_par_data);
        }

        logL = sum_of_logL;

        return std::make_tuple(std::make_tuple(new_pi, new_mus, new_sigmas), logL);
    }

    template <int dim, int mixture_num>
    template <int num>
    double EM_estimator <dim, statistic::GAUSSIAN_MIXTURES <mixture_num> >::logL(const typename EM_estimator <dim,
                                                                                                              statistic::GAUSSIAN_MIXTURES <
                                                                                                                  mixture_num> >::Record&
                                                                                 record, const statistic::Data_series <dim, num> &data_series)
    {
        auto pi     = std::get <0>(record);
        auto mus    = std::get <1>(record);
        auto sigmas = std::get <2>(record);

        // 中間生成物p[t][dist]
        util::multi_array <double, num, mixture_num> p;
        dvector <dim>                                mu;
        dmatrix <dim>                                sigmaInverse;
        double                                       sigmaDeterminant;
        for (int dist = 0; dist < mixture_num; dist++) {
            mu               = mus[dist];
            sigmaInverse     = matrix_util::invert(sigmas[dist]);
            sigmaDeterminant = matrix_util::determinant(sigmas[dist]);
            for (int t = 0; t < num; t++) {
                p[t][dist] = statistic_util::pnorm <dim>(data_series.dataset()[t], mu, sigmaInverse, sigmaDeterminant);
            }
        }

        // 対数尤度_logL
        // データごとの対数尤度の和を求める．
        double logL;
        double sum_of_logL = 0.;
        for (int t = 0; t < num; t++) {
            // データごとの，分布ごとの尤度の和を求める．
            double sum_of_L_par_data = 0.;
            for (int dist = 0; dist < mixture_num; dist++) {
                sum_of_L_par_data += pi[dist] * p[t][dist];
            }
            sum_of_logL += log(sum_of_L_par_data);
        }

        logL = sum_of_logL;

        return logL;
    }

    template <int dim, int mixture_num>
    void EM_estimator <dim, statistic::GAUSSIAN_MIXTURES <mixture_num> >::output(std::ostream &os, const Record &record)
    {
        auto tmppi = std::get <0>(record);
        for (int dist = 0; dist < mixture_num; dist++) {
            os << tmppi[dist] << ((dist != mixture_num - 1) ? ',' : '\0');
        }

        os << ',';

        auto tmpmus = std::get <1>(record);
        for (int dist = 0; dist < mixture_num; dist++) {
            for (int j = 0; j < dim; j++) {
                os << tmpmus[dist](j) << ((j != dim - 1) ? ',' : '\0');
            }
            os << ((dist != mixture_num - 1) ? ',' : '\0');
        }

        os << ',';

        auto tmpsigmas = std::get <2>(record);
        for (int dist = 0; dist < mixture_num; dist++) {
            for (int i = 0; i < dim; i++) {
                for (int j = 0; j < dim; j++) {
                    os << tmpsigmas[dist](i, j) << ((j != dim - 1) ? ',' : '\0');
                }
                os << ((i != dim - 1) ? ',' : '\0');
            }
            os << ((dist != mixture_num - 1) ? ',' : '\0');
        }

        os << std::endl;
    }

    void test_EM_gaussian_mixtures()
    {
        constexpr int dim         = 2; // 2次元
        constexpr int mixture_num = 4;  // 混合数
        constexpr int num         = 1000;
        using PD = statistic::Probability_distribution <dim, statistic::GAUSSIAN_MIXTURES <mixture_num> >;
        using DS = statistic::Data_series <dim, num>;
        constexpr statistic_util::FORMAT format = statistic_util::FORMAT::CSV_COMMA;
        std::ofstream                    fout;

        // // 真のパラメータ群
        // std::array <double, mixture_num> pi = { 0.1, 0.2, 0.3, 0.4 };
        //
        // std::array <dvector <dim>, mixture_num> mus;
        // mus[0](0) = 0.; mus[0](1) = 0.;
        // mus[1](0) = 0.; mus[1](1) = 10.;
        // mus[2](0) = 10.; mus[2](1) = 0.;
        // mus[3](0) = 10.; mus[3](1) = 10.;
        //
        // std::array <dmatrix <dim>, mixture_num> As;
        // As[0](0, 0) = 1.; As[0](0, 1) = 0.;
        // As[0](1, 0) = 0.; As[0](1, 1) = 1.;
        // As[1](0, 0) = 1.; As[1](0, 1) = 0.;
        // As[1](1, 0) = 0.; As[1](1, 1) = 1.;
        // As[2](0, 0) = 1.; As[2](0, 1) = 0.;
        // As[2](1, 0) = 0.; As[2](1, 1) = 1.;
        // As[3](0, 0) = 1.; As[3](0, 1) = 0.;
        // As[3](1, 0) = 0.; As[3](1, 1) = 1.;
        //
        // // 確率分布
        // PD pd(pi, mus, As);
        //
        // // 出力
        // fout.open("test_EM.trueparam");
        // pd.outparam(fout, format);
        // fout.close();
        // statistic_util::Range <dim> range({ -10., -10. }, { 10., 10. });
        // fout.open("test_EM_gaussian_mixtures.csv");
        // pd.output <20, 20>(fout, range, format);
        // fout.close();

        // データの生成
        // DS ds(pd);

        // 出力
        // fout.open("test_EM.dataset");
        // ds.output(fout, format);
        // fout.close();

        // データの読込
        std::ifstream fin("data/square.dataset");
        DS ds(fin, format);
        fin.close();

        // EMアルゴリズム
        using EM     = EM_estimator <dim, statistic::GAUSSIAN_MIXTURES <mixture_num> >;
        using Record = EM_estimator <dim, statistic::GAUSSIAN_MIXTURES <mixture_num> >::Record;
        static_assert(std::is_same <Record, std::tuple <std::array <double, mixture_num>, std::array <dvector <dim>, mixture_num>, std::array <dmatrix <dim>, mixture_num> > >::value, "Record type error!");

        std::deque <Record> paramtable;
        std::deque <double> logLtable;

        // 初期化
        auto pi1 = util::make_array<double>(0.25, 0.25, 0.25, 0.25);
        std::array <dvector <dim>, mixture_num> mus1;
        mus1[0](0) = 0.; mus1[0](1) = 0.;
        mus1[1](0) = 0.; mus1[1](1) = 0.;
        mus1[2](0) = 0.; mus1[2](1) = 0.;
        mus1[3](0) = 0.; mus1[3](1) = 0.;
        std::array <dmatrix <dim>, mixture_num> sigmas1;
        sigmas1[0](0, 0) = 1.; sigmas1[0](0, 1) = 0.;
        sigmas1[0](1, 0) = 0.; sigmas1[0](1, 1) = 1.;
        sigmas1[1](0, 0) = 1.; sigmas1[1](0, 1) = 0.;
        sigmas1[1](1, 0) = 0.; sigmas1[1](1, 1) = 1.;
        sigmas1[2](0, 0) = 1.; sigmas1[2](0, 1) = 0.;
        sigmas1[2](1, 0) = 0.; sigmas1[2](1, 1) = 1.;
        sigmas1[3](0, 0) = 1.; sigmas1[3](0, 1) = 0.;
        sigmas1[3](1, 0) = 0.; sigmas1[3](1, 1) = 1.;

        auto record = std::make_tuple(pi1, mus1, sigmas1);
        paramtable.push_back(record);
        logLtable.push_back(EM::logL(record, ds));

        // EM
        Record oldrecord, newrecord;
        double oldlogL, newlogL;
        int    counter = 0;
        const int big_num = 1e6;
        double max, tmp;
        newrecord = paramtable.back();
        newlogL   = logLtable.back();
        do {
            oldrecord = newrecord;
            oldlogL   = newlogL;
            auto up = EM::update(paramtable.back(), ds);
            paramtable.push_back(std::get <0>(up));
            logLtable.push_back(std::get <1>(up));
            newrecord = paramtable.back();
            newlogL   = logLtable.back();

            // 停止判定
            max = 0.;
            for (size_t dist = 0; dist < mixture_num; dist++) {
                if ((tmp = fabs(std::get <0>(newrecord)[dist] - std::get <0>(oldrecord)[dist])) > max) {
                    max = tmp;
                }
                if ((tmp = matrix_util::d_inf(std::get <1>(newrecord)[dist], std::get <1>(oldrecord)[dist])) > max) {
                    max = tmp;
                }
                if ((tmp = matrix_util::d_inf(std::get <2>(newrecord)[dist], std::get <2>(oldrecord)[dist])) > max) {
                    max = tmp;
                }
            }

            counter++;
            if (counter > big_num) {
                std::cout << "too many loop." << std::endl;
                break;
            }
        } while (max > statistic_util::epsilon);

        // 出力
        const char delim = formatToDelim(format);
        fout.open("test_EM.params");
        while (!paramtable.empty()) {
            EM::output(fout, paramtable.front());
            paramtable.pop_front();
        }
        fout.close();

        fout.open("test_EM.logL");
        while (!logLtable.empty()) {
            fout << logLtable.front() << std::endl;
            logLtable.pop_front();
        }
        fout.close();
    }
}

#endif
