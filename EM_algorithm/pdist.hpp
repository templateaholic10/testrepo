﻿#ifndef PDIST
#define PDIST

#include <iostream>
#include <fstream>
#include <array>
#include <random>
#include <functional>
#include "statistic_util.hpp"
#include "../lab/util.hpp"

namespace statistic {
    template <int dim>
    using dvector = statistic_util::dvector <dim>;

    template <int dim>
    using dmatrix = statistic_util::dmatrix <dim>;

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

    // 正規分布の確率分布
    template <int dim>
    class Probability_distribution <dim, GAUSSIAN>
    {
    public:
        Probability_distribution() = delete;
        Probability_distribution(const dvector <dim> &mu, const dmatrix <dim> &A);
        ~Probability_distribution() = default;

        double        pdf(const dvector <dim> &x) const; // 確率密度関数

        dvector <dim> generate(); // 確率分布からデータを1つ生成する関数

        template <int ... Meshes>
        void output(std::ostream &os, const statistic_util::Range <dim> &range, const statistic_util::FORMAT format) const;  // ファイルに書き出す
        void outparam(std::ostream &os, const statistic_util::FORMAT format) const;  // パラメータをファイルに書き出す

    private:
        const dvector <dim> _mu;  // 平均ベクトル
        const dmatrix <dim> _sigma;  // 分散共分散行列 _sigma := _A * _A^T
        // 以下冗長
        const dmatrix <dim> _A;  // 変換行列
        dmatrix <dim> _sigmaInverse;  // 精度行列
        const double        _sigmaDeterminant; // 分散共分散行列のディターミナント
        // 乱数生成器
        std::random_device                _rnd; // 非決定的乱数生成器
        std::mt19937                _mt; // メルセンヌ・ツイスタ
        std::normal_distribution <> _stdnorm;  // 1次元標準正規分布に従う乱数生成器
    };

    // 混合正規分布の確率分布
    template <int dim, int mixture_num>
    class Probability_distribution <dim, GAUSSIAN_MIXTURES <mixture_num> >
    {
    public:
        Probability_distribution() = delete;
        Probability_distribution(const std::array <double, mixture_num> &pi, const std::array <dvector <dim>, mixture_num> &mus, const std::array <dmatrix <dim>, mixture_num> &As);
        ~Probability_distribution() = default;

        double        pdf(const dvector <dim> &x) const; // 確率密度関数

        dvector <dim> generate(); // 確率分布からデータを1つ生成する関数

        template <int ... Meshes>
        void output(std::ostream &os, const statistic_util::Range <dim> &range, const statistic_util::FORMAT format) const;        // ファイルに書き出す
        void outparam(std::ostream &os, const statistic_util::FORMAT format) const;        // パラメータをファイルに書き出す

    private:
        const std::array <double, mixture_num>        _pi; // 混合比
        const std::array <dvector <dim>, mixture_num> _mus;  // 平均ベクトル
        const std::array <dmatrix <dim>, mixture_num> _As;  // 変換行列
        std::array <dmatrix <dim>, mixture_num>       _sigmas; // 分散共分散行列 _sigma := _A * _A^T
        // 以下冗長
        std::array <dmatrix <dim>, mixture_num> _sigmaInverses;  // 精度行列
        std::array <double, mixture_num>        _sigmaDeterminants; // 分散共分散行列のディターミナント
        // 乱数生成器
        std::random_device                     _rnd; // 非決定的乱数生成器
        std::mt19937                     _mt; // メルセンヌ・ツイスタ
        std::normal_distribution <>      _stdnorm; // 1次元標準正規分布に従う乱数生成器
        std::uniform_real_distribution <> _mixvoter;  // 一様分布に従う乱数生成器
    };

    void testgaussian();
    // void testgaussian_mixtures();
}

#include "detail/pdist.hpp"

#endif
