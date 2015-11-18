/*! @file
    @brief std::array I/O
    @author yanteyon10
    @date 11/18
*/
#ifndef ARRAY_IO_HPP
#define ARRAY_IO_HPP

#include <array>
#include <iostream>
#include <cstdlib>

/*! @macro
    @brief デリミタ．インクルード前にARRAY_IO_DELIMを定義することで変更が可能
*/
#ifndef ARRAY_IO_DELIM
#define ARRAY_IO_DELIM ','
#endif

/*! @brief std::array抽出子
*/
template <typename Elem, size_t n>
std::ostream& operator<<(std::ostream& os, const std::array<Elem, n>& seq)
{
    if (n == 0) {
        return os;
    }
    os << *(seq.begin());
    for (auto it = seq.begin()+1; it != seq.end(); it++) {
        os << ARRAY_IO_DELIM << *it;
    }
    return os;
}

/*! @brief std::array挿入子
*/
template <typename Elem, size_t n>
std::istream& operator>>(std::istream& is, std::array<Elem, n>& seq)
{
    for (auto elem : seq) {
        is >> elem;
        is.ignore(1, ARRAY_IO_DELIM);
    }
    return is;
}

#endif
