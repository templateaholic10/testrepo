#ifndef TEMPLATE_893
#define TEMPLATE_893

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
            void add(T1 a, T2 b, T3 c, T4 d);
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

// (1)のテスト
void test1()
{
    A<int>::B<float>::C<double> c;
    c.add(5, 4.2f, 10.5, 'c');
    return;
}

// (2)のテスト
void test2()
{
    Interface<1, int>::self_intro();
    Interface<2, double[]>::self_intro();
    Interface<3, Meta<0>>::self_intro();
    return;
}

#include "details/template_893.hpp"

#endif
