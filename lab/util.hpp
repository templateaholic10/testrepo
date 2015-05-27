#ifndef UTIL
#define UTIL

#include <iostream>
#include <string>

// 反復関数
// 文字列strをdelimで区切ってn回osに出力する
void repeat(std::ostream& os, const std::string& str, int n, char delim = '\0')
{
    for (int i = 0; i < n; i++) {
        os << str << ((i != n-1) ? delim : '\0');
    }
}

#endif
