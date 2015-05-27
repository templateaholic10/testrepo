#include <iostream>
#include <stdio.h>
#include <functional>
#include "util.hpp"

// 関数ポインタと関数オブジェクト（ファンクタ）
//
// Cでは
// ・関数ポインタを使う
// ・ポインタを通じて動的多態を実現する
//
// C++では
// ・関数オブジェクト（ファンクタ）を使う
// ・関数テンプレートを通じて静的多態を実現する
//
// 関数オブジェクト（ファンクタ）とは
// ・operator()が定義されたクラス
// ・ラムダ式
// であり，通常の関数と同じくfunctionalのstd::functionテンプレートで受けることができる

// (1)関数ポインタの例

namespace ex1 {
    // 関数
    int add(int a, int b)
    {
        return a + b;
    }

    int sub(int a, int b)
    {
        return a - b;
    }

    // 動的多態を実現するインタフェース
    int calc(int (*func)(int, int), int a, int b)
    {
        return func(a, b);
    }
}

namespace functor {
    void test1()
    {
        std::cout << "test1 : function pointer" << std::endl;
        int x(10);
        int y(5);
        std::cout << "calc(add, " << x << ", " << y << ") = " << ex1::calc(ex1::add, x, y) << std::endl;
        std::cout << "calc(sub, " << x << ", " << y << ") = " << ex1::calc(ex1::sub, x, y) << std::endl;
    }
}

// (2)関数オブジェクトの例

namespace ex2 {
    // 関数オブジェクト
    class Add {
    public:
        int operator()(int a, int b)
        {
            return a + b;
        }
    };

    class Sub {
    public:
        int operator()(int a, int b)
        {
            return a - b;
        }
    };

    // 静的多態を実現する関数テンプレート
    template <class functor>
    int calc(functor f, int a, int b)
    {
        return f(a, b);
    }
}

namespace functor {
    void test2()
    {
        std::cout << "test2 : functor" << std::endl;
        int      x(10);
        int      y(5);
        ex2::Add add;
        ex2::Sub sub;
        std::cout << "calc(add, " << x << ", " << y << ") = " << ex2::calc(add, x, y) << std::endl;
        std::cout << "calc(sub, " << x << ", " << y << ") = " << ex2::calc(sub, x, y) << std::endl;
    }
}

// (3)std::functionの例

namespace functor {
    void test3()
    {
        std::cout << "test3 : std::function" << std::endl;
        int      x(10);
        int      y(5);
        ex2::Add add;
        ex2::Sub sub;

        // 普通の関数
        std::function <int(int, int)> f_func = ex1::add;
        std::cout << "(standard function) add(" << x << ", " << y << ") = " << f_func(x, y) << std::endl;

        // 関数オブジェクト
        std::function <int(int, int)> f_func_obj = ex2::Add();
        std::cout << "(functor) Add add; add(" << x << ", " << y << ") = " << f_func_obj(x, y) << std::endl;

        // ラムダ式
        std::function <int(int, int)> f_lambda = [ = ](int a, int b) {
                                                     return a + b;
                                                 };
        std::cout << "(lambda) [=](int a, int b){ return a+b; }(" << x << ", " << y << ") = " << f_lambda(x, y) << std::endl;

        // メンバ関数
        std::cout << "(member function) Calc::add(" << x << ", " << y << ") = " << "coming soon..." << std::endl;
    }
}

namespace functor {
    void test()
    {
        auto horizonline = []() {
                               repeat(std::cout, "-", 20);
                               std::cout << std::endl;
                           };

        horizonline();
        horizonline();
        std::cout << "functor.hpp" << std::endl;
        horizonline();
        horizonline();
        test1();
        horizonline();
        test2();
        horizonline();
        test3();
        horizonline();
    }
}
