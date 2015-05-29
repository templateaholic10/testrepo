#ifndef STATISTIC_UTIL
#define STATISTIC_UTIL

#include <iostream>
#include <vector>
#include "matrix_util.hpp"

namespace statistic {
    // ファイルフォーマット
    enum class FORMAT { CSV_SPACE, CSV_COMMA };

    // ファイルフォーマットからデリミタを得る
    constexpr char formatToDelim(FORMAT format)
    {
        char ch;
        switch (format) {
            case CSV_SPACE:
                ch = ' ';
                break;
            case CSV_COMMA:
                ch = ',';
                break;
            default:
                assert(false);
                ch = ' ';
                break;
        }

        return ch;
    }

    // エイリアステンプレート
    template <int dim>
    using dvector = boost::numeric::ublas::bounded_vector <double, dim>;

    template <int dim>
    using dmatrix = boost::numeric::ublas::bounded_matrix <double, dim, dim>;

    // 多次元正規分布のpdf
    template <int dim>
    double pnorm(const dvector <dim> &x, const dvector <dim> &mu, const dmatrix <dim> &sigmaInverse, double sigmaDeterminant);

    // （内部的に使用）多次元正規分布の正規化子
    template <int dim>
    constexpr double normalize();

    // 混合正規分布の対数尤度
    // 混合率とデータごと，分布ごとの確率pを与える
    template <int num, int mixture_num>
    double logL(const std::array <double, mixture_num> &pi, const std::array <std::array <double, mixture_num>, num> &p);

    // 矩形領域
    // _x1が左上，_x2が右下
    template <int dim>
    class Range
    {
    public:
        Range() = delete;
        Range(const dvector<dim>& x1, const dvector<dim>& x2)
        {
            for (int i = 0; i < dim; i++) {
                BOOST_UBLAS_CHECK(x1(i) <= x2(i));
            }
            _x1 = x1; _x2 = x2;
        };
        Range(const std::vector<double>& x1, const std::vector<double>& x2)
        {
            BOOST_UBLAS_CHECK(x1.size() == dim && x2.size() == dim);
            for (int i = 0; i < dim; i++) {
                BOOST_UBLAS_CHECK(x1[i] <= x2[i];
            }
            _x1 = x1; _x2 = x2;
        };
        ~Range() = default;

        // ゲッター
        dvector<dim> x1() const
        {
            return _x1;
        };
        dvector<dim> x2() const
        {
            return _x2;
        };
    private:
        dvector<dim> _x1, _x2;
    };
}

#include "detail/statistic_util.hpp"

#endif
