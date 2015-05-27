#pragma once
#include <iostream>
#include <array>
#include "statistic_util.hpp"

namespace statistic {

    template <int dim, class distribution> class Probability_distribution;

    // メタ関数
    // 未知の分布
    struct UNKNOWN
    {
        static const UNKNOWN type;
    };

    // 正規分布
    struct GAUSSIAN
    {
        static const GAUSSIAN type;
    };

    // 混合正規分布
    template <int mixture_num>
    struct GAUSSIAN_MIXTURES
    {
        static const GAUSSIAN_MIXTURES<mixture_num> type;
    };

    // プライマリテンプレート
    template <int dim, class distribution>
    class Probability_distribution
    {
    };

    // 正規分布の確率分布
    template <int dim>
    class Probability_distribution<dim, GAUSSIAN>
    {
    public:
        Probability_distribution() = delete;
        Probability_distribution(const dvector<dim>& mu, const dmatrix<dim>& A);
        ~Probability_distribution() = default;

        double pdf(const dvector<dim>& x) const;  // 確率密度関数
        void generate(dvector<dim>& data) const;  // 確率分布からデータを1つ生成する関数
        template <FORMAT format>
        void output(std::ostream &os) const;  // ファイルに書き出す

    private:
        dvector<dim> _mu;  // 平均
        dmatrix<dim> _A;  // 変換行列
        dmatrix<dim> _sigma;  // 分散 _sigma := _A * _A^T
    };

    // 混合正規分布の確率分布
    template <int dim, int mixture_num>
    class Probability_distribution<dim, GAUSSIAN_MIXTURES<mixture_num> >
    {
    public:
        Probability_distribution() = delete;
        Probability_distribution(const std::array<double, mixture_num>& pi, const std::array<dvector<dim>, mixture_num>& mu, const std::array<dmatrix<dim>, mixture_num>& sigma);
        ~Probability_distribution() = default;

        double pdf(const dvector<dim>& x) const;  // 確率密度関数
        void generate(dvector<dim>& data) const;  // 確率分布からデータを1つ生成する関数
        template <FORMAT format>
        void output(std::ostream &os) const;  // ファイルに書き出す

    private:
        std::array<double, mixture_num> _pi;  // 混合比
        std::array<dvector<dim>, mixture_num> _mu;  // 平均
        std::array<dmatrix<dim>, mixture_num> _sigma;  // 分散
    };

}
