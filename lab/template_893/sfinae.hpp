#ifndef SFINAE
#define SFINAE

#include <iostream>
#include <array>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/list.hpp>
#include <boost/mpl/at.hpp>
#include "../util.hpp"

namespace sfinae {
    // クラステンプレートの場合
    // 部分特殊化とSFINAEを組み合わせる．
    // SFINAEに失敗すると特殊化の候補から除外する．
    // 複数通りに特殊化可能なとき最も深い特殊化が選択される．

    // char, int, longなどの整数型しか特殊化できないクラステンプレートIntegral_array．
    template <class T, size_t size, class Ignored = void>
    struct Integral_array;

    template <class T, size_t size>
    struct Integral_array<T, size, typename std::enable_if<std::is_integral<T>::value>::type>
    {
        std::array<T, size> a;
    };

    // もしcharだけであれば単純に部分特殊化すればよい．
    template <class T, size_t size>
    struct Char_array;

    template <size_t size>
    struct Char_array<char, size>
    {
        std::array<char, size> a;
    };

    // メタ関数にも応用が広い．
    // 例えばSTLコンテナの判定が（おおまかには）iteratorを持っているかどうかでできることから，STL判定器が作れる．
    template <class T, class Ignored = void>
    struct is_STL
    {
        static constexpr bool value = false;
    };

    // パラメータを評価するためのメタ関数．
    template <class>
    struct ignored
    {
        using type = void;
    };

    template <class T>
    struct is_STL<T, typename ignored<typename T::iterator>::type>
    {
        static constexpr bool value = true;
    };

    // ランダムアクセス可能か判定するメタ関数．
    template <class T, class Ignored = void>
    struct is_RandomAccess
    {
        static constexpr bool value = false;
    };

    // ランダムアクセス可能か判定するメタ関数．
    template <class T>
    struct is_RandomAccess<T, typename ignored<typename boost::mpl::at_c<T, 0>::type>::type>
    {
        static constexpr bool value = true;
    };

    // 関数テンプレートの場合
    // オーバーロードとSFINAEを組み合わせる．
    // SFINAEに失敗するとオーバーロードの候補から除外する．
    // 複数通りに特殊化可能なとき，ダメ．

    // 整数型かどうかを教えてくれる関数．
    template <class T, typename std::enable_if<std::is_integral<T>::value>::type* = nullptr>
    void annouce_is_integer(T num)
    {
        std::cout << num << " is integer." << std::endl;
    }

    template <class T, typename std::enable_if<!std::is_integral<T>::value>::type* = nullptr>
    void annouce_is_integer(T num)
    {
        std::cout << num << " is not integer." << std::endl;
    }

    void test_sfinae()
    {
        std::cout << util::Repeat("-", 20) << std::endl;
        std::cout << "TEST SFINAE" << std::endl;
        // これはOK
        Integral_array<unsigned int, 3> a;
        // これはコンパイルエラー（implicit instantiation of undefined template）
        // Integral_array<double, 5> d;
        std::cout << "std::array<int, 3> is STL? : " << is_STL<std::array<int, 3>>::value << std::endl;
        std::cout << "int is STL? : " << is_STL<int>::value << std::endl;

        using v = boost::mpl::vector<char, short, int, long>;
        using v_type = boost::mpl::at_c<v, 0>::type;
        std::cout << "vector is random access? : " << is_RandomAccess<v>::value << std::endl;
        using lst = boost::mpl::list<char, short, int, long>;
        using lst_type = boost::mpl::at_c<lst, 0>::type;
        std::cout << "list is random access? : " << is_RandomAccess<lst>::value << std::endl;

        long l = 5;
        std::string str = "hoge";
        std::cout << std::is_integral<int>::value << std::endl;
        annouce_is_integer(l);
        annouce_is_integer(str);
        std::cout << util::Repeat("-", 20) << std::endl;
    }
}

#endif
