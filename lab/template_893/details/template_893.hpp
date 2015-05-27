#ifndef DETAILS_TEMPLATE_893
#define DETAILS_TEMPLATE_893

#include <iostream>
#include "../template_893.hpp"

// (1)ネストされたクラステンプレートのメンバである関数テンプレート
namespace ex1 {
    // テンプレートパラメータの指定は同じスコープに属するものを同行に書く
    template <class T1>
    template <class T2>
    template <class T3>
    template <class T4>
    void A <T1>::B <T2>::C <T3>::add1(T1 a, T2 b, T3 c, T4 d)
    {
        std::cout << a + b + c + d << std::endl;

        return;
    }

    // エラーになるコード
    // template <class T1>
    // template <class T2>
    // template <class T3>
    // template <class T4>
    // void A<T1>::B<T2>::C<T3>::add2(T1 a, T2 b, T3 c, T4 d)
    // {
    //     // 内部テンプレートクラス
    //     template<class T5>
    //     class E
    //     {
    //         template<class T6>
    //         class F
    //         {
    //             template<class T7>
    //             void add_all(T5 e, T6 f, T7 g)
    //             {
    //                 std::cout << a + b + c + d + e + f + g << std::endl;
    //             }
    //         }
    //     }
    //
    //     E<int>::F<float> tmp;
    //     tmp.add_all(1, 6.3f, 7.);
    //
    //     return;
    // }
}

// (2)テンプレートパラメータにテンプレートを指定する
namespace ex2 {
    // プライマリテンプレート
    template <int param, class T>
    void Interface <param, T>::self_intro()
    {
        std::cout << "I'm from primary template." << std::endl;
        std::cout << "param : " << param << std::endl;
        std::cout << "class : " << typeid(T).name() << std::endl;
    }

    // テンプレートの部分特殊化
    template <int param, int dim>
    void Interface <param, Meta <dim> >::self_intro()
    {
        std::cout << "I'm from partially specialized template." << std::endl;
        std::cout << "param : " << param << std::endl;
        std::cout << "class : " << "Meta" << " " << "with dim : " << dim << std::endl;
    }
}

#endif
