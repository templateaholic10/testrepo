#ifndef TEMPLATE_893
#define TEMPLATE_893

#include <iostream>
#include <string>
#include <typeinfo>
#include "../util.hpp"

// (1)ネストされたクラステンプレートのメンバである関数テンプレート
namespace ex1 {
    template <class T1>
    struct A
    {
        template <class T2>
        struct B
        {
            template <class T3>
            struct C
            {
                template <class T4>
                void add1(T1 a, T2 b, T3 c, T4 d);

                // エラーになるコード（内部テンプレート）
                // template <class T4>
                // void add2(T1 a, T2 b, T3 c, T4 d);
            };
        };
    };
}

// (2)テンプレートパラメータにテンプレートを指定する
namespace ex2 {
    // メタ関数
    template <int dim>
    struct Meta
    {
    };

    // インタフェース
    template <int param, class T>
    struct Interface
    {
        static void self_intro();
    };

    // インタフェースの特殊化
    // paramとdimは同じ段のスコープに属するので同行に書く
    template <int param, int dim> // 自由パラメータ
    struct Interface <param, Meta <dim> > // 束縛パラメータ
    {
        static void self_intro();
    };
}

// (3)可変長テンプレート引数
namespace ex3 {
    // 静的多態を実現でき，非常に便利
    // テンプレートでない可変長引数関数も関数テンプレートで定義するほうが望ましい
    //
    // ... Args : pack演算子は可変長パラメータを圧縮する
    // Args...  : unpack演算子は可変長パラメータを展開する
    //
    // Argsの各要素に直接アクセスすることはできない
    //
    // ＜方針＞
    // pack・unpack演算子を用いて可変長パラメータを前から順に，再帰的に展開する

    // (3-1) テンプレートパラメータに対応する可変長引数を持つ場合
    // テンプレート実引数に基づいて型推論が可能

    // 引数が1つのとき
    template <class T>
    void conveyer(const T &t)
    {
        std::cout << "(1 param called)" << std::endl;
        std::cout << t << std::endl;
    }

    // 引数が複数のとき
    template <class First, class ... Rest>
    void conveyer(const First &first, const Rest& ... rest)
    {
        std::cout << "(n params called)" << std::endl;
        conveyer(first);  // 先頭要素を処理
        conveyer(rest ...);  // 残りをunpackして渡す
    }

    // 引数の数によって呼び分けられる
    // 引数が2つのときconveyer(rest...)の引数が1つになりそちらが呼ばれる

    // (3-2) テンプレートパラメータに対応する可変長引数を持たない場合（誤り）
    // テンプレート実引数に基づく型推論が不可能なので，明示的に（実）特殊化する必要がある
    // エラーになるコード

    // 引数が1つのとき
    template <class T>
    void self_intro_error()
    {
        std::cout << "(1 param called)" << std::endl;
        std::cout << typeid(T).name() << std::endl;
    }

    // 引数が複数のとき
    template <class First, class ... Rest>
    void self_intro_error()
    {
        std::cout << "(n params called)" << std::endl;
        self_intro_error <First>();     // 先頭要素を処理
        self_intro_error <Rest ...>();    // 残りをunpackして渡す
    }

    // 引数が2つのときRestがただ1つになるが，可変長引数である2番目が呼ばれる．このときRestが空になり，対応するself_introがないので再帰の末尾でエラーになる

    // (3-3) テンプレートパラメータに対応する可変長引数を持たない場合（正解）
    // テンプレート実引数に基づく型推論が不可能なので，明示的に（実）特殊化する必要がある

    // 引数が1つのとき
    template <class T>
    void self_intro()
    {
        std::cout << "(1 param called)" << std::endl;
        std::cout << typeid(T).name() << std::endl;
    }

    // 引数が複数のとき
    template <class First, class Second, class ... Rest>
    void self_intro()
    {
        std::cout << "(n params called)" << std::endl;
        self_intro <First>();             // 先頭要素を処理
        self_intro <Second, Rest ...>();            // 残りをunpackして渡す
    }

    // 引数が2つのときRestがただ1つになるが，可変長引数である2番目が呼ばれる．このときRestが空になり，対応するself_introがないので再帰の末尾でエラーになる
}

// (1)のテスト
namespace ex1 {
    // (1)のテスト1
    void test1_1()
    {
        std::cout << "test1_1 :" << std::endl;
        A <int>::B <float>::C <double> c;
        std::cout << "c.add1(5, 4.2f, 10.5, 'c') prints " << std::endl;
        c.add1(5, 4.2f, 10.5, 'c');
    }

    // (1)のテスト2：エラーになるコード
    // void test1_2()
    // {
    //     std::cout << "test1_2 :" << std::endl;
    //     A<int>::B<float>::C<double> c;
    //     c.add2(5, 4.2f, 10.5, 'c');
    //     return;
    // }

    void test1()
    {
        test1_1();
        // std::cout << std::endl;
        // test1_2();
    }
}

// (2)のテスト
namespace ex2 {
    void test2()
    {
        std::cout << "test2 :" << std::endl;
        std::cout << "Interface <1, int>::self_intro() prints " << std::endl;
        Interface <1, int>::self_intro();
        std::cout << "Interface <2, double[]>::self_intro() prints " << std::endl;
        Interface <2, double[]>::self_intro();
        std::cout << "Interface <3, Meta <0> >::self_intro() prints " << std::endl;
        Interface <3, Meta <0> >::self_intro();
    }
}

// (3)のテスト
namespace ex3 {
    void test3()
    {
        std::cout << "test3-1 :" << std::endl;
        std::cout << "conveyer(0, 1.f, 3., 'c', \"str\") prints " << std::endl;
        conveyer(0, 1.f, 3., 'c', "str");
        // std::cout << std::endl;
        // std::cout << "test3-2 :" << std::endl;
        // self_intro_error(0, 1.f, 3., 'c', "str");
        std::cout << std::endl;
        std::cout << "test3-3 :" << std::endl;
        std::cout << "self_intro <int, char *, const std::string&>() prints " << std::endl;
        self_intro <int, char *, const std::string&>();
    }
}

#include "detail/template_893.hpp"

#endif
