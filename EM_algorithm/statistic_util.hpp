#ifndef STATISTIC_UTIL
#define STATISTIC_UTIL

#include <iostream>
#include <vector>
#include "../lab/util.hpp"
#include "matrix_util.hpp"

namespace statistic_util {
    constexpr double epsilon = 1.e-6;

    // ファイルフォーマット
    enum class FORMAT { CSV_SPACE, CSV_COMMA, CSV_COMMA_SQ };

    // ファイルフォーマットからデリミタを得る
    constexpr char formatToDelim(const FORMAT format)
    {
        char ch('\0');
        switch (format) {
            case FORMAT::CSV_SPACE:
                ch = ' ';
                break;
            case FORMAT::CSV_COMMA:
            case FORMAT::CSV_COMMA_SQ:
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
    using dvector = boost::numeric::ublas::bounded_vector <double, static_cast <std::size_t>(dim)>;

    template <int dim>
    using dmatrix = boost::numeric::ublas::bounded_matrix <double, static_cast <std::size_t>(dim), static_cast <std::size_t>(dim)>;

    // それぞれの出力
    template <int dim>
    void output(std::ostream &os, const dvector <dim> &v, const char delim)
    {
        for (int i = 0; i < dim; i++) {
            os << v(i) << ((i != dim - 1) ? delim : '\n');
        }
    }

    template <int dim>
    void output(std::ostream &os, const dmatrix <dim> &A, const char delim)
    {
        for (int i = 0; i < dim; i++) {
            for (int j = 0; j < dim; j++) {
                os << A(i, j) << ((j != dim - 1) ? delim : '\n');
            }
        }
    }

    // 座標変換行列から分散行列を得る
    // dmatrix<2> transToSigmaD2(const dmatrix<2>& m)
    // {
    //     BOOST_UBLAS_CHECK(m.size1() == m.size2(), boost::numeric::ublas::external_logic());
    //
    //     return std::move(boost::numeric::ublas::prod(m, boost::numeric::ublas::trans(m)));
    // }

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
    private:
        std::array <double, dim> _x1, _x2;
    public:
        Range() = delete;
        // これで右辺値も参照できる．
        Range(const std::array <double, dim> &x1, const std::array <double, dim> &x2)
            : _x1(x1), _x2(x2)
        {
            for (int i = 0; i < dim; i++) {
                if (_x1[i] > _x2[i]) {
                    std::swap(_x1[i], _x2[i]);
                }
            }
        }

        /*
                // これで右辺値も参照できる．
                Range(const std::vector <double> &x1, const std::vector <double> &x2)
                {
                    static_assert((x1.size() == dim && x2.size() == dim), "size error!");
                    _x1 = x1; _x2 = x2;
                    for (int i = 0; i < dim; i++) {
                        if (_x1[i] > _x2[i]) {
                            std::swap(_x1, _x2);
                        }
                    }
                }
        */
        ~Range() = default;

        // ゲッター
        const std::array <double, dim> &x1() const
        {
            return _x1;
        }

        const std::array <double, dim> &x2() const
        {
            return _x2;
        }

        // スライシング
        Range <1> first() const
        {
            return Range <1>(std::array <double, 1>(_x1[0]), std::array <double, 1>(_x2[0]));
        }

        Range <dim - 1> rest() const
        {
            static_assert(dim > 1, "range dimension under flow!");
            // めっちゃ冗長．
            std::array <double, dim - 1> restx1;
            std::array <double, dim - 1> restx2;
            for (int i = 1; i < dim; i++) {
                restx1[i - 1] = _x1[i];
                restx2[i - 1] = _x2[i];
            }

            return std::move(Range <dim - 1>(restx1, restx2));
        }
    };

    template <int dim>
    void output(std::ostream &os, const Range <dim> &range)
    {
        os << "[" << range.x1()[0] << ", " << range.x2()[0] << "]";
        for (int i = 1; i < dim; i++) {
            os << " * " << "[" << range.x1()[i] << ", " << range.x2()[i] << "]";
        }
        os << std::endl;
    }

    // 0次元rangeの特殊化を禁止
    // template <>
    // Range<0>
    // {};

    // ・outputSQ関数
    // 2次元multi_arrayを矩形に整形して表示する．
    template <std::size_t n1, std::size_t n2>
    void outputSQ(std::ostream &os, const util::multi_array <double, n1, n2> &M, const char delim)
    {
        for (int i = 0; i < n1; i++) {
            for (int j = 0; j < n2; j++) {
                os << M(i, j) << ((j != n2 - 1) ? delim : '\n');
            }
        }
    }

    template <typename T, std::size_t nx, std::size_t ny, int meshnumx = nx - 1, int meshnumy = ny - 1>
    void outputSQ(std::ostream &os, const std::array <std::array <T, ny>, nx> &M, const Range <2> &range, const char delim)
    {
        double meshx = (range.x2()[0] - range.x1()[0]) / meshnumx;
        double meshy = (range.x2()[1] - range.x1()[1]) / meshnumy;

        // y軸目盛り
        double yfirst = range.x1()[1];
        for (int j = 0; j <= meshnumy; j++, yfirst += meshy) {
            os << "," << yfirst;
        }
        os << std::endl;

        // x軸目盛りおよび本体
        double xfirst = range.x1()[0];
        for (int i = 0; i <= meshnumx; i++, xfirst += meshx) {
            os << xfirst;
            for (int j = 0; j <= meshnumy; j++) {
                os << "," << M[i][j];
            }
            os << std::endl;
        }
    }

    // ・descretize関数
    // メッシュを切って関数を離散化する．
    //     Descretize<100, 100>::descretize(f, range)
    // のような呼び出しを期待する．
    // ここで，100，100はメッシュの数であり，各次元の要素数より1だけ少ないことに注意．

    // 関数fへの要請は以下の通り．
    // (1)実数体R^dim上で定義された関数．つまり，dim個のdoubleを変数にとる．
    // (2)返り値がvoid型ではない．

    // メンバ関数だとまずい？

    // descretize関数→f(double, double, ..., double)
    // descretizeA関数→f(std::array<double, dim>)
    // descretizeDV関数→f(dvector<dim>)

    template <int offset, typename T, int Size, int ... Sizes>
    struct _expand_array
    {
        using type = std::array <typename _expand_array <offset, T, Sizes ...>::type, Size + offset>;
    };

    template <int offset, typename T, int Size>
    struct _expand_array <offset, T, Size>
    {
        using type = std::array <T, Size + offset>;
    };

    // エイリアステンプレート
    template <int offset, typename T, int ... Sizes>
    using expand_array = typename _expand_array <offset, T, Sizes ...>::type;

    // dimでそもそもの次元を保持．
    // 多次元版
    template <int dim, int Sizefirst, int ... Sizerest>
    struct Descretize_sub
    {
        template <typename Functor, typename FResult = typename util::nresult_of <Functor, double, dim>::type, class Result = expand_array <1, FResult, Sizefirst, Sizerest ...>, typename ... Doubles>
        constexpr static Result _descretize_sub(const Functor &f, const Range <1 + sizeof ... (Sizerest)> &range, Doubles ... doubles)
        {
            Result fd;
            double mesh = (range.x2()[0] - range.x1()[0]) / Sizefirst;

            double xfirst = range.x1()[0];
            for (int i = 0; i <= Sizefirst; i++, xfirst += mesh) {
                fd[i] = Descretize_sub <dim, Sizerest ...>::_descretize_sub(f, range.rest(), doubles ..., xfirst);
            }

            return std::move(fd);
        }

        template <typename Functor, typename FResult = typename std::result_of <Functor(std::array <double, dim> )>::type, class Result = expand_array <1, FResult, Sizefirst, Sizerest ...>, typename ... Doubles>
        constexpr static Result _descretizeA_sub(const Functor &f, const Range <1 + sizeof ... (Sizerest)> &range, Doubles ... doubles)
        {
            Result fd;
            double mesh = (range.x2()[0] - range.x1()[0]) / Sizefirst;

            double xfirst = range.x1()[0];
            for (int i = 0; i <= Sizefirst; i++, xfirst += mesh) {
                fd[i] = Descretize_sub <dim, Sizerest ...>::_descretizeA_sub(f, range.rest(), doubles ..., xfirst);
            }

            return std::move(fd);
        }

        template <typename Functor, typename FResult = typename std::result_of <Functor(dvector <dim> )>::type, class Result = expand_array <1, FResult, Sizefirst, Sizerest ...>, typename ... Doubles>
        static Result _descretizeDV_sub(const Functor &f, const Range <1 + sizeof ... (Sizerest)> &range, Doubles ... doubles)
        {
            Result fd;
            double mesh = (range.x2()[0] - range.x1()[0]) / Sizefirst;

            double xfirst = range.x1()[0];
            for (int i = 0; i <= Sizefirst; i++, xfirst += mesh) {
                fd[i] = Descretize_sub <dim, Sizerest ...>::_descretizeDV_sub(f, range.rest(), doubles ..., xfirst);
            }

            return std::move(fd);
        }
    };

    // 1次元版
    template <int dim, int Sizefirst>
    struct Descretize_sub <dim, Sizefirst>
    {
        template <typename Functor, typename FResult = typename util::nresult_of <Functor, double, dim>::type, class Result = expand_array <1, FResult, Sizefirst>, typename ... Doubles>
        constexpr static Result _descretize_sub(const Functor &f, const Range <1> &range, Doubles ... doubles)
        {
            Result fd;
            double mesh = (range.x2()[0] - range.x1()[0]) / Sizefirst;

            double xfirst = range.x1()[0];
            for (int i = 0; i <= Sizefirst; i++, xfirst += mesh) {
                fd[i] = f(doubles ..., xfirst);
            }

            return std::move(fd);
        }

        template <typename Functor, typename FResult = typename std::result_of <Functor(std::array <double, dim> )>::type, class Result = expand_array <1, FResult, Sizefirst>, typename ... Doubles>
        constexpr static Result _descretizeA_sub(const Functor &f, const Range <1> &range, Doubles ... doubles)
        {
            Result fd;
            double mesh = (range.x2()[0] - range.x1()[0]) / Sizefirst;

            double xfirst = range.x1()[0];
            for (int i = 0; i <= Sizefirst; i++, xfirst += mesh) {
                fd[i] = f(util::make_array <double>(doubles ..., xfirst));
            }

            return std::move(fd);
        }

        template <typename Functor, typename FResult = typename std::result_of <Functor(dvector <dim> )>::type, class Result = expand_array <1, FResult, Sizefirst>, typename ... Doubles>
        static Result _descretizeDV_sub(const Functor &f, const Range <1> &range, Doubles ... doubles)
        {
            Result fd;
            double mesh = (range.x2()[0] - range.x1()[0]) / Sizefirst;

            double xfirst = range.x1()[0];
            for (int i = 0; i <= Sizefirst; i++, xfirst += mesh) {
                fd[i] = f(matrix_util::make_bounded_vector <double, dim>(util::make_array <double>(doubles ..., xfirst)));
            }

            return std::move(fd);
        }
    };

    // 多次元版
    template <int Sizefirst, int ... Sizerest>
    struct Descretize
    {
        template <typename Functor, typename FResult = typename util::nresult_of <Functor, double, 1 + sizeof ... (Sizerest)>::type, class Result = expand_array <1, FResult, Sizefirst, Sizerest ...> >
        constexpr static Result descretize(const Functor &f, const Range <1 + sizeof ... (Sizerest)> &range)
        {
            Result fd;
            double mesh = (range.x2()[0] - range.x1()[0]) / Sizefirst;

            double xfirst = range.x1()[0];
            for (int i = 0; i <= Sizefirst; i++, xfirst += mesh) {
                fd[i] = Descretize_sub <1 + sizeof ... (Sizerest), Sizerest ...>::_descretize_sub(f, range.rest(), xfirst);
            }

            return std::move(fd);
        }

        template <typename Functor, typename FResult = typename std::result_of <Functor(std::array <double, 1 + sizeof ... (Sizerest)> )>::type, class Result = expand_array <1, FResult, Sizefirst, Sizerest ...> >
        constexpr static Result descretizeA(const Functor &f, const Range <1 + sizeof ... (Sizerest)> &range)
        {
            Result fd;
            double mesh = (range.x2()[0] - range.x1()[0]) / Sizefirst;

            double xfirst = range.x1()[0];
            for (int i = 0; i <= Sizefirst; i++, xfirst += mesh) {
                fd[i] = Descretize_sub <1 + sizeof ... (Sizerest), Sizerest ...>::_descretizeA_sub(f, range.rest(), xfirst);
            }

            return std::move(fd);
        }

        template <typename Functor, typename FResult = typename std::result_of <Functor(dvector <1 + sizeof ... (Sizerest)> )>::type, class Result = expand_array <1, FResult, Sizefirst, Sizerest ...> >
        static Result descretizeDV(const Functor &f, const Range <1 + sizeof ... (Sizerest)> &range)
        {
            Result fd;
            double mesh = (range.x2()[0] - range.x1()[0]) / Sizefirst;

            double xfirst = range.x1()[0];
            for (int i = 0; i <= Sizefirst; i++, xfirst += mesh) {
                fd[i] = Descretize_sub <1 + sizeof ... (Sizerest), Sizerest ...>::_descretizeDV_sub(f, range.rest(), xfirst);
            }

            return std::move(fd);
        }
    };

    // 1次元版
    template <int Sizefirst>
    struct Descretize <Sizefirst>
    {
        template <typename Functor, typename FResult = typename util::nresult_of <Functor, double, 1>::type, class Result = expand_array <1, FResult, Sizefirst> >
        constexpr static Result descretize(const Functor &f, const Range <1> &range)
        {
            Result fd;
            double mesh = (range.x2()[0] - range.x1()[0]) / Sizefirst;

            double xfirst = range.x1()[0];
            for (int i = 0; i <= Sizefirst; i++, xfirst += mesh) {
                fd[i] = f(xfirst);
            }

            return std::move(fd);
        }

        template <typename Functor, typename FResult = typename std::result_of <Functor(std::array <double, 1> )>::type, class Result = expand_array <1, FResult, Sizefirst> >
        constexpr static Result descretizeA(const Functor &f, const Range <1> &range)
        {
            Result fd;
            double mesh = (range.x2()[0] - range.x1()[0]) / Sizefirst;

            double xfirst = range.x1()[0];
            for (int i = 0; i <= Sizefirst; i++, xfirst += mesh) {
                fd[i] = f(util::make_array <double>(xfirst));
            }

            return std::move(fd);
        }

        template <typename Functor, typename FResult = typename std::result_of <Functor(dvector <1> )>::type, class Result = expand_array <1, FResult, Sizefirst> >
        static Result descretizeDV(const Functor &f, const Range <1> &range)
        {
            Result fd;
            double mesh = (range.x2()[0] - range.x1()[0]) / Sizefirst;

            double xfirst = range.x1()[0];
            for (int i = 0; i <= Sizefirst; i++, xfirst += mesh) {
                fd[i] = f(matrix_util::make_bounded_vector <double, 1>(util::make_array <double>(xfirst)));
            }

            return std::move(fd);
        }
    };
}

#include "detail/statistic_util.hpp"

#endif
