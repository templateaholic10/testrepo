#ifndef TEMPLATE_893
#define TEMPLATE_893

#include <iostream>
#include <string>

// (1)ネストされたクラステンプレートのメンバである関数テンプレート
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

// (2)テンプレートパラメータにテンプレートを指定する
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
template <int param, int dim>  // 自由パラメータ
struct Interface<param, Meta<dim> >  // 束縛パラメータ
{
    static void self_intro();
};

// (1)のテスト1
void test1_1()
{
    std::cout << "test1_1 :" << std::endl;
    A<int>::B<float>::C<double> c;
    c.add1(5, 4.2f, 10.5, 'c');
    return;
}

// (1)のテスト2：エラーになるコード
// void test1_2()
// {
//     std::cout << "test1_2 :" << std::endl;
//     A<int>::B<float>::C<double> c;
//     c.add2(5, 4.2f, 10.5, 'c');
//     return;
// }

// (2)のテスト
void test2()
{
    std::cout << "test2 :" << std::endl;
    Interface<1, int>::self_intro();
    Interface<2, double[]>::self_intro();
    Interface<3, Meta<0>>::self_intro();
    return;
}

void printprintf(const std::string& str, int repeat)
{
    for (int i = 0; i < repeat; i++) {
        std::cout << str;
    }
}

#include "details/template_893.hpp"

#endif
