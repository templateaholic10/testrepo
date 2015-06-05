#ifndef EM_ALGORITHM
#define EM_ALGORITHM

#include <iostream>
#include <deque>
#include <array>
#include <boost/numeric/ublas/lu.hpp>
#include "statistic_util.hpp"
#include "data_struct.hpp"

#undef BOOST_UBLAS_TYPE_CHECK
#define BOOST_UBLAS_TYPE_CHECK 0

namespace EM {
    // エイリアステンプレート
    template <int dim>
    using dvector = statistic_util::dvector <dim>;

    template <int dim>
    using dmatrix = statistic_util::dmatrix <dim>;

    // EM_estimatorはインスタンスを持たない推定器．

    // プライマリテンプレート
    template <int dim, class distribution>
    struct EM_estimator
    {
    };

    // 混合正規分布のEMアルゴリズム
    template <int dim, int mixture_num>
    struct EM_estimator <dim, statistic::GAUSSIAN_MIXTURES <mixture_num> >
    {
        // レコードは {混合比，平均，分散}
        using Record = std::tuple <std::array <double, mixture_num>, std::array <dvector <dim>, mixture_num>, std::array <dmatrix <dim>, mixture_num> >;

        using ExRecord = std::tuple <std::array <double, mixture_num>, std::array <dvector <dim>, mixture_num>, std::array <dmatrix <dim>, mixture_num>, double >;

        // パラメータを更新し，対数尤度を返す関数
        template <int num>
        static boost::optional<ExRecord> update(const Record &record, const statistic::Data_series <dim, num> &data_series);

        // 対数尤度を返す関数
        template <int num>
        static boost::optional<double> logL(const Record &record, const statistic::Data_series <dim, num> &data_series);

        // レコードを読み込む関数
        static Record input(std::istream &is, const char delim);

        // レコードを出力する関数
        static void output(std::ostream &os, const Record &record, const char delim);

        static void output(std::ostream &os, const statistic_util::NA &na, const char delim);

        static void output(std::ostream &os, const statistic_util::Header &header, const char delim);
    };

    constexpr int DIM         = 2;
    constexpr int MIXTURE_NUM = 4;
    constexpr int NUM         = 100;

    void test_EM_gaussian_mixtures();

    void _test_EM_initial_value(std::ostream &os, const statistic::Data_series <DIM, NUM> &ds, const EM_estimator <DIM, statistic::GAUSSIAN_MIXTURES <MIXTURE_NUM> >::Record &record);

    void test_EM_initial_value(const std::string& numbering);

    void test_EM_initial_pdf();
}

#include "detail/EM_algorithm.hpp"

#endif
