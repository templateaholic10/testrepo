#ifndef TEMP_META_PROGRAMMING
#define TEMP_META_PROGRAMMING

#include <iostream>
#include <string>
#include <typeinfo>
#include <boost/mpl/int.hpp>
#include <boost/mpl/modulus.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/string.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/mpl/lambda.hpp>
#include "../util.hpp"

namespace temp_meta_programming {
    namespace mpl = boost::mpl;
    using namespace mpl::placeholders;

    // メタ関数は「テンプレート引数でメタデータを引数にとり」「typeでメタデータを返す」関数である．
    // doubleとかを返したければstatic const valueとして返せばよい．

    // (1)twice2階メタ関数の実装
    // twice<F, X> ... メタ関数F，メタデータXのように使う．
    // メタ関数をテンプレートパラメータにとるので，Fをメタ関数クラスにラップするか，プレースホルダによって型に落とす．
    namespace ex1 {
        // プリミティブにメタ関数転送を利用する方法
        template <class F, class X>
        struct twice1
            : F::template apply <typename F::template apply <X>::type>
        {
        };

        // メンバメタ関数の起動にはtemplateキーワードが必要．
        // これを減らす方法
        template <class UnaryMeraFunctionClass, class Arg>
        struct apply1
            : UnaryMeraFunctionClass::template apply <Arg>
        {
        };

        // このようにFからメタ関数クラスを生成するとFがメタ関数クラスの場合はそのままに，プレースホルダ式の場合に拡張することができる．
        // std::add_pointer<_1>のようなプレースホルダ式をラムダ式という．mpl::lambdaはラムダ式からメタ関数クラスを生成する．
        template <class F, class X>
        struct twice2
            : apply1 <typename mpl::lambda <F>::type, typename apply1 <typename mpl::lambda <F>::type, X>::type>
        {
        };

        // mpl::applyを利用すれば以下のように書ける．
        // つまり，applyメタ関数は（内部的にmpl::lambdaを呼んでいるので）mpl::lambdaを介さずにプレースホルダ式を評価できる．
        template <class F, class X>
        struct twice3
            : mpl::apply <F, typename mpl::apply <F, X>::type>
        {
        };

        // mpl::apply<F,X>はメタ関数クラスFにラップされたメタ関数，またはプレースホルダ式FにXを渡して起動する．

        struct add_pointer_f
        {
            template <class T>
            struct apply : std::add_pointer <T>
            {
            };
        };

        void test()
        {
            static_assert(std::is_same <twice1 <add_pointer_f, int>::type, int **>::value, "twice1 doesn't work properly!");
            static_assert(std::is_same <twice2 <add_pointer_f, int>::type, int **>::value, "twice2 doesn't work properly!");
            static_assert(std::is_same <twice3 <add_pointer_f, int>::type, int **>::value, "twice3 doesn't work properly!");
        }
    }
}

#endif
