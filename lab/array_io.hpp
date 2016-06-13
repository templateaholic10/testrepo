/*! @file
    @brief std::array I/O
    @author templateaholic10
    @date 11/18
*/
#ifndef ARRAY_IO_HPP
#define ARRAY_IO_HPP

#include <array>
#include <iostream>
#include <cstdlib>

// 演算子をすべての名前空間から探索するため，グローバルにおく

/*! @macro
    @brief デリミタ．インクルード前にARRAY_IO_DELIMを定義することで変更が可能
*/
#ifndef ARRAY_IO_DELIM
    #define ARRAY_IO_DELIM ','
#endif

/*! @brief std::arrayの高度抽出関数
    @param os 出力ストリーム
    @param seq std::array
    @param delim デリミタ
*/
template <typename Elem, size_t n>
std::ostream &out(std::ostream &os, const std::array <Elem, n> &seq, const char delim=ARRAY_IO_DELIM)
{
    if (n == 0) {
        return os;
    }
    os << *(seq.begin());
    for (auto it = seq.begin() + 1; it != seq.end(); it++) {
        os << delim << *it;
    }

    return os;
}

/*! @brief Eigen::Matrixの高度挿入関数
    @param is 入力ストリーム
    @param seq std::array
    @param delim デリミタ
*/
template <typename Elem, size_t n>
std::istream &in(std::istream &is, std::array <Elem, n> &seq, const char delim=ARRAY_IO_DELIM)
{
    if (n == 0) {
        return is;
    }
    is >> *(seq.begin());
    for (auto it = seq.begin() + 1; it != seq.end(); it++) {
        is.ignore(1, delim);
        is >> *it;
    }

    return is;
}

/*! @brief std::array抽出子
*/
template <typename Elem, size_t n>
std::ostream&operator<<(std::ostream &os, const std::array <Elem, n> &seq)
{
    return out(os, seq);
}

/*! @brief std::array挿入子
*/
template <typename Elem, size_t n>
std::istream&operator>>(std::istream &is, std::array <Elem, n> &seq)
{
    return in(is, seq);
}

#endif
