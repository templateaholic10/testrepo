#ifndef DETAIL_PDIST
#define DETAIL_PDIST

#include "../pdist.hpp"

namespace statistic {
    template <int dim, class distribution>
    void test();

    /* --- 正規分布の実装 --- */

    template <int dim>
    Probability_distribution <dim, GAUSSIAN>::Probability_distribution(const dvector <dim> &mu, const dmatrix <dim> &A)
        : _mu(mu), _A(A), _sigma(A * boost::numeric::ublas::trans(A)), _sigmaDeterminant(matrix_util::determinant(_sigma)), _mt(std::random_device rnd, rnd()), _stdnorm(0., 1.)
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
    void Probability_distribution <dim, GAUSSIAN>::generate(dvector <dim> &data) const
    {
        dvector <dim> z();
        // 多変量標準正規分布を生成
        for (int i = 0; i < dim; i++) {
            z(i) = _stdnorm(_mt);
        }
        data = _A * (z - _mu);
    }

    // 書き出し
    template <int dim, FORMAT format>
    void Probability_distribution <dim, GAUSSIAN>::output <format>(std::ostream &os, const Range &range, double mesh) const
    {
        char delim(formatToDelim(format));

        dvector <dim> x1(range.x1());
        dvector <dim> x2(range.x2());
        dvector <dim> x;

        // 各次元についてインクリメントしつつ出力
        // 4次元の場合
        for (x(0) = x1(0); x(0) < x2(0); x(0) += mesh) {
            for (x(1) = x1(1); x(1) < x2(1); x(1) += mesh) {
                for (x(2) = x1(2); x(2) < x2(2); x(2) += mesh) {
                    for (x(3) = x1(3); x(3) < x2(3); x(3) += mesh) {
                        os << pdf(x) << ((i4 != dim - 1) ? delim : '\n');
                    }
                }
            }
        }
        for (int i = 0; i < dim; i++) {
            while (x(i) <= x2(i)) {
                os << pdf(x);
                x(i) += mesh;
            }
        }




        os << "# --- average mu ---" << std::endl;
        for (int i = 0; i < dim; i++) {
            os << _mu(i) << ((i != dim - 1) ? delim : '');
        }
        os << std::endl;
        os << "# --- variance sigma ---" << std::endl;
        for (int i = 0; i < dim; i++) {
            for (int j = 0; j < dim; j++) {
                os << _sigma(i, j) << ((j != dim - 1) ? delim : '');
            }
            os << std::endl;
        }
        os << std::endl;
    }

    // パラメータの書き出し
    template <int dim, FORMAT format>
    void Probability_distribution <dim, GAUSSIAN>::outparam <format>(std::ostream &os) const
    {
        char delim(formatToDelim(format));

        os << "# --- average mu ---" << std::endl;
        for (int i = 0; i < dim; i++) {
            os << _mu(i) << ((i != dim - 1) ? delim : '\n');
        }
        os << std::flush;
        os << "# --- variance sigma ---" << std::endl;
        for (int i = 0; i < dim; i++) {
            for (int j = 0; j < dim; j++) {
                os << _sigma(i, j) << ((j != dim - 1) ? delim : '\n');
            }
            os << std::flush;
        }
        os << std::endl;
    }

    template <>
    void test <GAUSSIAN>()
    {
        constexpr int dim(2);  // 2次元
        using PD = Probability_distribution <dim, GAUSSIAN>;

        // パラメータ群
        dvector <dim> mu;
        mu(0) = 1.; mu(1) = 2.;

        dmatrix <dim> A;
        A(0, 0) = 1.; A(0, 1) = 1.;
        A(1, 0) = 0.; A(1, 1) = 2.;

        // 確率分布
        PD pd(mu, A);


        PD pd()
    }
}

#endif
