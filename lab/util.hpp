#ifndef UTIL
#define UTIL

#include <iostream>
#include <string>
#include <array>
#include <iterator>
#include <typeinfo>
#include <type_traits>
#include <utility>
#include <cstddef>
#include <tuple>
#include <bitset>
#include <boost/optional.hpp>

namespace util {
    // ・epsilon
    constexpr double epsilon = 10e-6;

    // ・repeat関数
    // 文字列strをdelimで区切ってn回osに出力する
    void repeat(std::ostream &os, const std::string &str, int n)
    {
        for (int i = 0; i < n; i++) {
            os << str;
        }
    }

    void repeat(std::ostream &os, const std::string &str, int n, char delim)
    {
        for (int i = 0; i < n - 1; i++) {
            os << str << delim;
        }
        os << str;
    }

    // ・power関数
    template <typename T>
    constexpr T power(const T base, int exponent)
    {
        T result = base;
        for (int i = 1; i < exponent; i++) {
            result *= base;
        }

        return result;
    }

    // ・lg関数
    // ビット長を求める．
    constexpr int lg(const int n)
    {
        int _n  = n;
        int lgn = 0;
        while (_n) {
            lgn++;
            _n >>= 1;
        }

        return lgn;
    }

    // ・ifloor関数
    constexpr int ifloor(const double d)
    {
        if (d > 0) {
            // 正の場合
            return static_cast <int>(d);
        } else {
            // 負の場合
            return static_cast <int>(d + epsilon) - 1;
        }
    }

    // ・iceil関数
    constexpr int iceil(const double d)
    {
        if (d > 0) {
            // 正の場合
            return static_cast <int>(d - epsilon) + 1;
        } else {
            // 負の場合
            return static_cast <int>(d);
        }
    }

    // ・iround関数
    constexpr int iround(const double d)
    {
        if (d > 0) {
            // 正の場合
            return static_cast <int>(d + 0.5);
        } else {
            // 負の場合
            return static_cast <int>(d - 0.5);
        }
    }

    // ・reverse関数
    // イテレータがない場合．
    template <class T>
    constexpr T reverse(const T &container)
    {
        T result = container;
        for (size_t i = 0; i < container.size(); i++) {
            result[i] = container[container.size()-1-i];
        }

        return std::move(result);
    }

    // ・slice関数
    template <int i, int j>
    struct Slice
    {
        template <template <int> class T, int n>
        constexpr static T <j - i> slice(const T <n>& container)
        {
            T<j-i> result;
            for (size_t k = i; k < j; k++) {
                result[k-i] = container[k];
            }
            return std::move(result);
        }
    };

    // ・HSVtoRGB関数
    // hue \in \set{Z}, saturation = 0, ..., 255, value = 0, ..., 255.
    boost::optional <std::tuple <int, int, int> > HSVtoRGB(int hue, int saturation, int value)
    {
        if (saturation < 0 || saturation > 255) {
            return boost::none;
        }
        if (value < 0 || value > 255) {
            return boost::none;
        }

        int    hi, p, q, t;
        double f;

        hi = (hue / 60) % 6;
        f  = hue / 60. - static_cast <int>(hue / 60.);
        p  = static_cast <int>(round(value * (1. - (saturation / 255.))));
        q  = static_cast <int>(round(value * (1. - (saturation / 255.) * f)));
        t  = static_cast <int>(round(value * (1. - (saturation / 255.) * (1. - f))));

        int red, green, blue;
        switch (hi)
        {
            case 0:
                red   = value;
                green = t;
                blue  = p;
                break;
            case 1:
                red   = q;
                green = value;
                blue  = p;
                break;
            case 2:
                red   = p;
                green = value;
                blue  = t;
                break;
            case 3:
                red   = p;
                green = q;
                blue  = value;
                break;
            case 4:
                red   = t;
                green = p;
                blue  = value;
                break;
            case 5:
                red   = value;
                green = p;
                blue  = q;
                break;
            default:

                return boost::none;
                break;
        }

        return std::tuple <int, int, int>(red, green, blue);
    }

    // ・nresult_ofメタ関数
    // 関数fにT型の引数をn個渡した時の返り値の型を返すメタ関数

    // 内部的に用いる_nresult_of_sub関数
    template <typename Functor, typename T, int n, typename ... Args>
    struct _nresult_of_sub
    {
        using type = typename _nresult_of_sub <Functor, T, n - 1, T, Args ...>::type;
    };

    template <typename Functor, typename T, typename ... Args>
    struct _nresult_of_sub <Functor, T, 0, Args ...>
    {
        using type = typename std::result_of <Functor(Args ...)>::type;
    };

    template <typename Functor, typename T, int n>
    struct nresult_of
    {
        using type = typename _nresult_of_sub <Functor, T, n - 1, T>::type;
    };

    // nresult_of<F, T, 2>::type
    // = _nresult_of_sub<F, T, 1, T>::type
    // = _nresult_of_sub<F, T, 0, T, T>::type
    // = std::result_of<F(T, T)>::type

    // nresult_of<F, T, 1>::type
    // = _nresult_of_sub<F, T, 0, T>::type
    // = std::result_of<F(T)>::type

    // エイリアステンプレート
    template <typename Functor, typename T, int n>
    using nresult_of_t = typename nresult_of <Functor, T, n>::type;

    // std::arrayの拡張
    // --- ここから人様のコピペ

    // [1次元]

    // ・make_array関数
    // 関数テンプレートの型推論によってconstexprなarrayを要素数なしで生成する．
    template <typename T, typename ... Args>
    constexpr std::array <T, sizeof ... (Args)> make_array(Args&& ... args)
    {
        return std::array <T, sizeof ... (Args)> { static_cast <Args &&>(args) ... };
    }

    // コンパイル時に作成したstd::arrayについてはコンパイル時assert（static_assert）で要素数などをチェック可能．そのためのconstexprなsize関数を定義する．

    // ・array_size関数
    // std::arrayの要素数を取得する．
    template <typename T, std::size_t N>
    constexpr std::size_t array_size(const std::array <T, N>&)
    {
        return N;
    }

    // 固定長配列の要素数を取得する．
    template <typename T, std::size_t N>
    constexpr std::size_t array_size(T(&)[N])
    {
        return N;
    }

    // [多次元]

    // ・multi_arrayクラス
    // std::arrayの多次元版のラッパークラス
    //
    // ＜動機＞T x[a][b][c]なる多次元配列のstd::array版をmulti_array<T, a, b, c> xで記述したい．
    //     array<array<array<T, c>, b>, a>
    // のエイリアステンプレートを定義することによって実現する．

    // 可変長テンプレートパラメータを再帰で展開する．

    // 多次元版（再帰）
    template <typename T, std::size_t Size, std::size_t ... Sizes>
    struct multi_array_type
    {
        using type = std::array <typename multi_array_type <T, Sizes ...>::type, Size>;
    };

    // 1次元版（再帰の末端）
    template <typename T, std::size_t Size>
    struct multi_array_type <T, Size>
    {
        using type = std::array <T, Size>;
    };

    // エイリアステンプレート
    template <typename T, std::size_t Size, std::size_t ... Sizes>
    using multi_array = typename multi_array_type <T, Size, Sizes ...>::type;

    // ・make_common_array関数
    // 関数テンプレートの型推論によってconstexprなmulti_arrayを型名，要素数なしで生成する．

    // 引数は1次元．多次元array（multi_array）を生成するときは1次元arrayからなる1次元arrayを引数にとる．
    // 自身が挙動をあまり理解していない．

    // 戻り値の型を定めるメタ関数
    template <typename ... TArgs>
    struct common_array_type
    {
        using type = std::array <typename std::decay <typename std::common_type <TArgs ...>::type>::type, sizeof ... (TArgs)>;
    };

    // 関数本体
    template <typename ... TArgs>
    constexpr typename common_array_type <TArgs ...>::type make_common_array(TArgs&& ... args)
    {
        return typename common_array_type <TArgs ...>::type { std::forward <TArgs>(args) ... };
    }

    // --- ここまで人様のコピペ

    // ・convert_arrayメタ関数
    // multi_array<T1, ...>からmulti_array<T2, ...>を得る．
    // ただ，multi_arrayの次元には任意性があるので（arrayを要素に持つmulti_arrayかもしれない），m(>= n)次元のmulti_array型をn次元で切ってT2に置換した型を得ることにする．

    // プライマリテンプレート．ダミー
    template <class From, typename T2, int dim>
    struct convert_array
    {
        using type = From;
    };

    // 再帰版
    template <typename T2, int dim, class Fromsub, int n>
    struct convert_array <std::array <Fromsub, n>, T2, dim>
    {
        using type = std::array <typename convert_array <Fromsub, T2, dim - 1>::type, n>;
    };

    // 再帰の末端
    template <typename T2, typename T1, int n>
    struct convert_array <std::array <T1, n>, T2, 1>
    {
        using type = std::array <T2, n>;
    };

    // ・convert_array_f関数
    // 実際のmulti_arrayを受け取って型変換してデフォルトコンストラクトしたものを返す関数
    template <class From, typename T2, int dim, class Result = typename convert_array <From, T2, dim>::type>
    Result convert_array_f(From A)
    {
        return std::move(Result());
    }

    // ・apply関数
    // Rにおけるapply関数を実装する．

    // (1)基本的に副作用を認めないものとし，副作用を生じる場合はapply_side_effect関数を用いる．
    // (2)多次元版と1次元版を次元に関する部分特殊化で分岐する．
    // (3)左辺値版と右辺値版をオーバーロードする．
    // (4)voidな関数と値を返す関数はデフォルトテンプレートパラメータの部分特殊化で分岐する．
    // (5)multi_arrayの要素の型による多態は関数テンプレートの型推論が担保する．

    // 以上の設計から，クラステンプレートにメンバ関数テンプレートを持たせる実装を行う．

    // 返り値を持つ版
    // プライマリテンプレート
    template <typename T, int dim, typename Resultvoid = T>
    struct Apply
    {
        // const lvalue版
        template <class Fromsub, std::size_t n, typename Functor, typename FResult = typename std::result_of <Functor(T)>::type, class Result = typename convert_array <std::array <Fromsub, n>, FResult, dim>::type>
        constexpr static Result apply(const std::array <Fromsub, n> &x, const Functor &f)
        {
            Result applied_array;

            {
                auto from = x.begin();
                auto to   = applied_array.begin();
                for (; from != x.end(); ++from, ++to) {
                    *to = Apply <T, dim - 1>::apply(*from, f);
                }
            }

            return std::move(applied_array);
        }

        // lvalue版
        template <class Fromsub, std::size_t n, typename Functor, typename FResult = typename std::result_of <Functor(T)>::type, class Result = typename convert_array <std::array <Fromsub, n>, FResult, dim>::type>
        constexpr static Result apply_side_effect(std::array <Fromsub, n> &x, const Functor &f)
        {
            Result applied_array;

            {
                auto from = x.begin();
                auto to   = applied_array.begin();
                for (; from != x.end(); ++from, ++to) {
                    *to = Apply <T, dim - 1>::apply_side_effect(*from, f);
                }
            }

            return std::move(applied_array);
        }

        // const rvalue版
        template <class Fromsub, std::size_t n, typename Functor, typename FResult = typename std::result_of <Functor(T)>::type, class Result = typename convert_array <std::array <Fromsub, n>, FResult, dim>::type>
        constexpr static Result apply(const std::array <Fromsub, n> &&x, const Functor &f)
        {
            Result applied_array;

            {
                auto from = x.begin();
                auto to   = applied_array.begin();
                for (; from != x.end(); ++from, ++to) {
                    *to = Apply <T, dim - 1>::apply(*from, f);
                }
            }

            return std::move(applied_array);
        }

        // rvalue版
        template <class Fromsub, std::size_t n, typename Functor, typename FResult = typename std::result_of <Functor(T)>::type, class Result = typename convert_array <std::array <Fromsub, n>, FResult, dim>::type>
        constexpr static Result apply_side_effect(std::array <Fromsub, n> &&x, const Functor &f)
        {
            Result applied_array;

            {
                auto from = x.begin();
                auto to   = applied_array.begin();
                for (; from != x.end(); ++from, ++to) {
                    *to = Apply <T, dim - 1>::apply_side_effect(*from, f);
                }
            }

            return std::move(applied_array);
        }
    };

    // 1次元版
    template <typename T, typename Resultvoid>
    struct Apply <T, 1, Resultvoid>
    {
        // const lvalue版
        template <std::size_t n, typename Functor, typename FResult = typename std::result_of <Functor(T)>::type, class Result = typename convert_array <std::array <T, n>, FResult, 1>::type>
        constexpr static Result apply(const std::array <T, n> &x, const Functor &f)
        {
            // std::cout << "(const lvalue, nonvoid called)" << std::endl;
            Result applied_array;

            {
                auto from = x.begin();
                auto to   = applied_array.begin();
                for (; from != x.end(); ++from, ++to) {
                    *to = f(*from);
                }
            }

            return std::move(applied_array);
        }

        // lvalue版
        template <std::size_t n, typename Functor, typename FResult = typename std::result_of <Functor(T)>::type, class Result = typename convert_array <std::array <T, n>, FResult, 1>::type>
        constexpr static Result apply_side_effect(std::array <T, n> &x, const Functor &f)
        {
            // std::cout << "(lvalue, nonvoid called)" << std::endl;
            Result applied_array;

            {
                auto from = x.begin();
                auto to   = applied_array.begin();
                for (; from != x.end(); ++from, ++to) {
                    *to = f(*from);
                }
            }

            return std::move(applied_array);
        }

        // const rvalue﻿﻿版
        template <std::size_t n, typename Functor, typename FResult = typename std::result_of <Functor(T)>::type, class Result = typename convert_array <std::array <T, n>, FResult, 1>::type>
        constexpr static Result apply(const std::array <T, n> &&x, const Functor &f)
        {
            // std::cout << "(const rvalue, nonvoid called)" << std::endl;
            Result applied_array;

            {
                auto from = x.begin();
                auto to   = applied_array.begin();
                for (; from != x.end(); ++from, ++to) {
                    *to = f(*from);
                }
            }

            return std::move(applied_array);
        }

        // rvalue﻿﻿版
        template <std::size_t n, typename Functor, typename FResult = typename std::result_of <Functor(T)>::type, class Result = typename convert_array <std::array <T, n>, FResult, 1>::type>
        constexpr static Result apply_side_effect(std::array <T, n> &&x, const Functor &f)
        {
            // std::cout << "(rvalue, nonvoid called)" << std::endl;
            Result applied_array;

            {
                auto from = x.begin();
                auto to   = applied_array.begin();
                for (; from != x.end(); ++from, ++to) {
                    *to = f(*from);
                }
            }

            return std::move(applied_array);
        }
    };

    // void版
    // プライマリテンプレート
    template <typename T, int dim>
    struct Apply <T, dim, void>
    {
        // const lvalue版
        template <class Fromsub, std::size_t n, typename Functor>
        constexpr static void apply(const std::array <Fromsub, n> &x, const Functor &f)
        {
            {
                auto from = x.begin();
                for (; from != x.end(); ++from) {
                    Apply <T, dim - 1, void>::apply(*from, f);
                }
            }
        }

        // lvalue版
        template <class Fromsub, std::size_t n, typename Functor>
        constexpr static void apply_side_effect(std::array <Fromsub, n> &x, const Functor &f)
        {
            {
                auto from = x.begin();
                for (; from != x.end(); ++from) {
                    Apply <T, dim - 1, void>::apply_side_effect(*from, f);
                }
            }
        }

        // const rvalue版
        template <class Fromsub, std::size_t n, typename Functor>
        constexpr static void apply(const std::array <Fromsub, n> &&x, const Functor &f)
        {
            {
                auto from = x.begin();
                for (; from != x.end(); ++from) {
                    Apply <T, dim - 1, void>::apply(*from, f);
                }
            }
        }

        // rvalue版
        template <class Fromsub, std::size_t n, typename Functor>
        constexpr static void apply_side_effect(std::array <Fromsub, n> &&x, const Functor &f)
        {
            {
                auto from = x.begin();
                for (; from != x.end(); ++from) {
                    Apply <T, dim - 1, void>::apply_side_effect(*from, f);
                }
            }
        }
    };

    // 1次元版
    template <typename T>
    struct Apply <T, 1, void>
    {
        // const lvalue版
        template <std::size_t n, typename Functor>
        constexpr static void apply(const std::array <T, n> &x, const Functor &f)
        {
            // std::cout << "(const lvalue, void called)" << std::endl;
            {
                auto from = x.begin();
                for (; from != x.end(); ++from) {
                    f(*from);
                }
            }
        }

        // lvalue版
        template <std::size_t n, typename Functor>
        constexpr static void apply_side_effect(std::array <T, n> &x, const Functor &f)
        {
            // std::cout << "(lvalue, void called)" << std::endl;
            {
                auto from = x.begin();
                for (; from != x.end(); ++from) {
                    f(*from);
                }
            }
        }

        // const rvalue﻿﻿版
        template <std::size_t n, typename Functor>
        constexpr static void apply(const std::array <T, n> &&x, const Functor &f)
        {
            // std::cout << "(const rvalue, void called)" << std::endl;
            {
                auto from = x.begin();
                for (; from != x.end(); ++from) {
                    f(*from);
                }
            }
        }

        // rvalue﻿﻿版
        template <std::size_t n, typename Functor>
        constexpr static void apply_side_effect(std::array <T, n> &&x, const Functor &f)
        {
            // std::cout << "(rvalue, void called)" << std::endl;
            {
                auto from = x.begin();
                for (; from != x.end(); ++from) {
                    f(*from);
                }
            }
        }
    };
}

#endif
