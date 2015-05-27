#ifndef PDIST
#define PDIST

#include <iostream>
#include <array>
#include <random>
#include "statistic_util.hpp"
#include "../util.hpp"

namespace statistic {
    template <int dim, class distribution> class Probability_distribution;

    // メタ関数
    // 未知の分布
    struct UNKNOWN
    {
        using type = UNKNOWN;
    };

    // 正規分布
    struct GAUSSIAN
    {
        using type = GAUSSIAN;
    };

    // 混合正規分布
    template <int mixture_num>
    struct GAUSSIAN_MIXTURES
    {
        using type = GAUSSIAN_MIXTURES <mixture_num>;
    };

    // プライマリテンプレート
    template <int dim, class distribution>
    class Probability_distribution
    {
    };

    template <class distribution>
    void test();

    // 正規分布の確率分布
    template <int dim>
    class Probability_distribution <dim, GAUSSIAN>
    {
    public:
        Probability_distribution() = delete;
        Probability_distribution(const dvector <dim> &mu, const dmatrix <dim> &A);
        ~Probability_distribution() = default;

        double pdf(const dvector <dim> &x) const;  // 確率密度関数
        void   generate(dvector <dim> &data) const; // 確率分布からデータを1つ生成する関数

        template <FORMAT format>
        void output(std::ostream &os, const Range& range, double mesh) const;  // ファイルに書き出す
        template <FORMAT format>
        void outparam(std::ostream &os) const;  // パラメータをファイルに書き出す

    private:
        dvector <dim> _mu;  // 平均ベクトル
        dmatrix <dim> _sigma;  // 分散共分散行列 _sigma := _A * _A^T
        // 以下冗長
        dmatrix <dim> _A;  // 変換行列
        dmatrix <dim> _sigmaInverse;  // 精度行列
        double        _sigmaDeterminant; // 分散共分散行列のディターミナント
        // 乱数生成器
        std::mt19937                _mt; // メルセンヌ・ツイスタ
        std::normal_distribution <> _stdnorm;  // 1次元標準正規分布に従う乱数生成器
    };

    template <>
    void test <GAUSSIAN>();

    // 混合正規分布の確率分布
    template <int dim, int mixture_num>
    class Probability_distribution <dim, GAUSSIAN_MIXTURES <mixture_num> >
    {
    public:
        Probability_distribution() = delete;
        Probability_distribution(const std::array <double, mixture_num> &pi, const std::array <dvector <dim>, mixture_num> &mu, const std::array <dmatrix <dim>, mixture_num> &sigma);
        ~Probability_distribution() = default;

        double pdf(const dvector <dim> &x) const;  // 確率密度関数
        void   generate(dvector <dim> &data) const; // 確率分布からデータを1つ生成する関数

        template <FORMAT format>
        void output(std::ostream &os, const Range& range, double mesh) const;  // ファイルに書き出す
        template <FORMAT format>
        void outparam(std::ostream &os) const;  // パラメータをファイルに書き出す

    private:
        std::array <double, mixture_num>        _pi; // 混合比
        std::array <dvector <dim>, mixture_num> _mu;  // 平均
        std::array <dmatrix <dim>, mixture_num> _sigma;  // 分散
    };

    template <>
    void test <GAUSSIAN_MIXTURES <2>>();

}

#include "detail/pdist.hpp"

#endif
