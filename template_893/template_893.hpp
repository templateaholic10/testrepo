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

// (1)のテスト
void test1()
{
    A<int>::B<float>::C<double> c;
    c.add(5, 4.2f, 10.5, 'c');  // 118.7
    return;
}

#include "details/template_893.hpp"

#endif
