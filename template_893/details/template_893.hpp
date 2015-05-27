#ifndef DETAILS_TEMPLATE_893
#define DETAILS_TEMPLATE_893

#include <iostream>
#include "../template_893.hpp"

// (1)ネストされたクラステンプレートのメンバである関数テンプレート
// テンプレートパラメータの指定は同じスコープに属するものを同行に書く
template <class T1>
template <class T2>
template <class T3>
template <class T4>
void A<T1>::B<T2>::C<T3>::add(T1 a, T2 b, T3 c, T4 d)
{
    std::cout << a + b + c + d << std::endl;
    return;
}

#endif
