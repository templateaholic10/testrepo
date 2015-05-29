#ifndef UTIL
#define UTIL

#include <iostream>
#include <string>
#include <array>
#include <iterator>
#include <type_traits>
#include <utility>

namespace util {
    // ・repeat関数
    // 文字列strをdelimで区切ってn回osに出力する
    void repeat(std::ostream &os, const std::string &str, int n, char delim='\0')
    {
        for (int i = 0; i < n; i++) {
            os << str << ((i != n - 1) ? delim : '\0');
        }
    }

    // std::arrayの拡張
    // ルーチェさんの実装を丸パクリしただけです．

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

    /*
        // ・result_ofメタ関数
        // 関数型から戻り値の型を抽出する．

        // 単純に戻り値の型を得るだけならばautoで受けてdecltypeすればよい．result_ofメタ関数による結果をテンプレート宣言に利用できる点が便利．

        // 型推論を利用したいので関数テンプレートをメタ関数でラップする．
        struct result_of_type_impl
        {
            // Resultが…

            // (1)通常の関数ポインタのとき
            template <class Result, class ... Args>
            static Result Func(Result (*)(Args ...));

            // (2)非constなメンバ関数ポインタのとき
            template <class T, class Result, class ... Args>
            static Result Func(Result (T::*)(Args ...));

            // (3)constなメンバ関数ポインタのとき
            template <class T, class Result, class ... Args>
            static Result Func(Result (T::*)(Args ...) const);

            // (4)関数オブジェクトのとき（ラムダ式含む）
            template <class T, class FuncType = decltype(&T::operator())>
            static decltype(Func(std::declval <FuncType>()))Func(T *);
            // decltype関数は引数の型を「宣言から」得る．つまり引数は評価されない．
            // decltype関数の引数に「ある型Tの変数」を使いたいとき，declval<T>()と書くことができる．例えば，T()などとしてしまうとTがdefault constructiveであることが要求されるが，decltypeの中身は評価されないのでdefault constructiveかはどうでもいい．
        };

        // ラッパーメタ関数
        // 例えば T = double (*)(double) （doubleをdoubleに変換する関数ポインタ）
        template <class T>
        struct result_of
        {
            using type = decltype(result_of_type_impl::Func(std::declval <std::add_pointer<typename std::remove_pointer<T>::type> >()));
        };
    */
    // ・apply関数
    // Rにおけるapply関数を実装する

    // 次元について再帰を行う．
    // 第一テンプレートパラメータによって3つの関数テンプレートをオーバーロード．

    // B = apply(A, f);
    // Aはconst lvalue reference，fはvoid以外を返す副作用を持たない関数．

    // 1次元版（プライマリテンプレート，array以外のarrayである場合）
    template <typename T, std::size_t Size, typename Functor, typename Result = typename std::result_of<Functor(T)>::type>
    struct Apply_sub
    {
        // メタ関数内でオーバーロード
        static std::array <Result, Size> apply_sub(const std::array <T, Size> &x, Functor f);

        static std::array <Result, Size> apply_sub(std::array <T, Size> &&x, Functor f);
    };

    template <typename T, std::size_t Size, typename Functor>
    struct Apply_sub_void
    {
        static void apply_sub_void(std::array <T, Size> &x, Functor f);
    };
/*
    // 部分特殊化できるのはクラステンプレートのみ
    // 多次元版（arrayのarrayである場合の特殊化）
    template <std::size_t Size, typename Functor, typename Tsub, std::size_t Sizesub>
    struct Apply_sub <std::array <Tsub, Sizesub>, Size, Functor>
    {
        // 返り値は怪しい．
        static auto apply_sub(const std::array <std::array<Tsub, Sizesub>, Size> &x, Functor f);
    };
*/
    template <typename T, std::size_t Size, typename Functor>
    auto apply(const std::array <T, Size> &x, Functor f)
    {
        return std::move(Apply_sub <T, Size, Functor>::apply_sub(x, f));
    }

    template <typename T, std::size_t Size, typename Functor>
    void apply_void(std::array <T, Size> &x, Functor f)
    {
        Apply_sub_void <T, Size, Functor>::apply_sub_void(x, f);
    }

    // 1次元版（プライマリテンプレート，array以外のarrayである場合）
    template <typename T, std::size_t Size, typename Functor, typename Result>
    std::array <Result, Size> Apply_sub <T, Size, Functor, Result>::apply_sub(const std::array <T, Size> &x, Functor f)
    {
        std::cout << "(const lvalue, 1 dim called)" << std::endl;

        // 内部でmulti_arrayを作る．
        std::array <Result, Size> applied_array;

        {
            auto from = x.begin();
            auto to   = applied_array.begin();
            for (; from != x.end(); ++from, ++to) {
                *to = f(*from);
            }
        }

        return std::move(applied_array);
    }

    template <typename T, std::size_t Size, typename Functor, typename Result>
    std::array <Result, Size> Apply_sub <T, Size, Functor, Result>::apply_sub(std::array <T, Size> &&x, Functor f)
    {
        std::cout << "(rvalue, 1 dim called)" << std::endl;

        // 内部でmulti_arrayを作る．
        std::array <Result, Size> applied_array;

        {
            auto from = x.begin();
            auto to   = applied_array.begin();
            for (; from != x.end(); ++from, ++to) {
                *to = f(*from);
            }
        }

        return std::move(applied_array);
    }

    template <typename T, std::size_t Size, typename Functor>
    void Apply_sub_void<T, Size, Functor>::apply_sub_void(std::array <T, Size> &x, Functor f)
    {
        std::cout << "(lvalue, 1 dim called)" << std::endl;

        {
            auto from = x.begin();
            for (; from != x.end(); ++from) {
                f(*from);
            }
        }

        return;
    }

    /*
    // 部分特殊化できるのはクラステンプレートのみ
    // 多次元版（arrayのarrayである場合の特殊化）
    template <std::size_t Size, typename Functor, typename Tsub, std::size_t Sizesub>
    auto Apply_sub <std::array <Tsub, Sizesub>, Size, Functor>::apply_sub(const std::array <Tsub, Size> &x, Functor f)
    {
        std::cout << "(const lvalue, n dim called)" << std::endl;

        // 内部でmulti_arrayを作る．
        std::array <std::array <Tsub, Sizesub>, Size> applied_array;

        {
            auto from = x.begin();
            auto to   = applied_array.begin();
            for (; from != x.end(); ++from, ++to) {
                *to = apply(*from, f);
            }
        }

        return std::move(applied_array);
    }
*/
    /*
        // 1次元版（プライマリテンプレート，array以外のarrayである場合）
        template <typename T, std::size_t Size, typename Functor>
        auto apply(const std::array <T, Size> &x, Functor f)
        {
            std::cout << "(const lvalue, 1 dim called)" << std::endl;

            using Result = decltype(std::declval <Functor>()(std::declval <T>()));

            // 内部でmulti_arrayを作る．
            std::array <Result, Size> applied_array;

            {
                auto from = x.begin();
                auto to   = applied_array.begin();
                for (; from != x.end(); ++from, ++to) {
                    *to = f(*from);
                }
            }

            return std::move(applied_array);
        }

        // 多次元版（arrayのarrayである場合の特殊化）
        template <std::size_t Size, typename Functor, typename Tsub, std::size_t Sizesub>
        auto apply <std::array <Tsub, Sizesub>, Size, Functor>(const std::array <Tsub, Size> &x, Functor f)
        {
            std::cout << "(const lvalue, n dim called)" << std::endl;

            // 内部でmulti_arrayを作る．
            std::array <std::array <Tsub, Sizesub>, Size> applied_array;

            {
                auto from = x.begin();
                auto to   = applied_array.begin();
                for (; from != x.end(); ++from, ++to) {
                    *to = apply(*from, f);
                }
            }

            return std::move(applied_array);
        }
    */
    /*
        // 再帰版
        template <typename T, std::size_t Size, std::size_t ... Sizes, typename Functor, typename Result = decltype(std::declval <Functor>()(std::declval <T>()))>
        multi_array <Result, Size, Sizes ...> apply(const multi_array <T, Size, Sizes ...> &x, Functor f)
        {
            std::cout << "(const lvalue, n dim called)" << std::endl;

            // 内部でmulti_arrayを作る．
            multi_array <Result, Size, Sizes ...> applied_array;

            {
                auto from = x.begin();
                auto to   = applied_array.begin();
                for (; from != x.end(); ++from, ++to) {
                    *to = apply(*from, f);
                }
            }

            return std::move(applied_array);
        }
    */
    /*
        // apply(A, fvoid);
        // Aはlvalue reference，fはvoidを返す副作用を持つ関数．

        // 1次元版
        template <typename T, std::size_t Size, typename Functor, typename Result = decltype(std::declval <Functor>()(std::declval <T>()))>
        void apply(std::array <T, Size> &x, Functor f)
        {
            static_assert(std::is_same <Result, void>::value, "apply not void error!");

            std::cout << "(lvalue, 1 dim called)" << std::endl;

            {
                auto from = x.begin();
                for (; from != x.end(); ++from) {
                    f(*from);
                }
            }
        }
    */
    /*
        // 再帰版
        template <typename T, std::size_t Size, std::size_t ... Sizes, typename Functor, typename Result = decltype(std::declval <Functor>()(std::declval <T>()))>
        void apply(multi_array <T, Size, Sizes ...> &x, Functor f)
        {
            static_assert(std::is_same <Result, void>::value, "apply not void error!");

            std::cout << "(lvalue, n dim called)" << std::endl;

            {
                auto from = x.begin();
                for (; from != x.end(); ++from) {
                    apply(*from, f);
                }
            }
        }
    */
    // B = apply(multi_array(), f);
    // Aはrvalue reference，fはvoid以外を返す副作用を持たない関数．

    // 1次元版
    template <typename T, std::size_t Size, typename Functor, typename Result = decltype(std::declval <Functor>()(std::declval <T>()))>
    std::array <Result, Size> apply(std::array <T, Size> &&x, Functor f)
    {
        std::cout << "(rvalue, 1 dim called)" << std::endl;

        // 内部でmulti_arrayを作る．
        std::array <Result, Size> applied_array;

        {
            auto from = x.begin();
            auto to   = applied_array.begin();
            for (; from != x.end(); ++from, ++to) {
                *to = f(*from);
            }
        }

        return std::move(applied_array);
    }

    /*
        // 再帰版
        template <typename T, std::size_t Size, std::size_t ... Sizes, typename Functor, typename Result = decltype(std::declval <Functor>()(std::declval <T>()))>
        multi_array <Result, Size, Sizes ...> apply(multi_array <T, Size, Sizes ...> &&x, Functor f)
        {
            std::cout << "(rvalue, n dim called)" << std::endl;

            // 内部でmulti_arrayを作る．
            multi_array <Result, Size, Sizes ...> applied_array;

            {
                auto from = x.begin();
                auto to   = applied_array.begin();
                for (; from != x.end(); ++from, ++to) {
                    *to = apply(*from, f);
                }
            }

            return std::move(applied_array);
        }

        // apply(multi_array(), fvoid);
        // Aはrvalue reference，fはvoidを返す副作用を持つ関数．
        // ありえない．右辺値を変更する意味がない．
        */
}

#endif
