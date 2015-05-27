#ifndef UTIL
#define UTIL

#include <iostream>
#include <string>

namespace util {

    // 反復関数
    // 文字列strをdelimで区切ってn回osに出力する
    void repeat(std::ostream& os, const std::string& str, int n, char delim = '\0')
    {
        for (int i = 0; i < n; i++) {
            os << str << ((i != n-1) ? delim : '\0');
        }
    }

    // 多次元配列
    // ルーチェさんの実装を丸パクリしただけです
    //
    // T x[A][B][C]なる多次元配列のstd::array版をmulti_array<T, A, B, C>

/*
    template <typename T, std::size_t Size, std::size_t ...Sizes>
    struct multi_array_type
    {
        using type = std::array<typename multi_array_type<T, Sizes...>::type, Size>;
    }
*/
}

#endif
