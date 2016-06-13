/*! @file
    @brief std::vector I/O
    @author templateaholic10
    @date 11/18
*/
#ifndef VECTOR_IO_HPP
#define VECTOR_IO_HPP

#include <vector>
#include <iostream>
#include <cstdlib>

/*! @macro
    @brief デリミタ．インクルード前にVECTOR_IO_DELIMを定義することで変更が可能
*/
#ifndef VECTOR_IO_DELIM
#define VECTOR_IO_DELIM ','
#endif

/*! @brief std::vector抽出子
*/
template <typename Elem>
std::ostream& operator<<(std::ostream& os, const std::vector<Elem>& seq)
{
    if (seq.empty()) {
        return os;
    }
    os << *(seq.begin());
    for (auto it = seq.begin()+1; it != seq.end(); it++) {
        os << VECTOR_IO_DELIM << *it;
    }
    return os;
}

/*! @brief std::vector挿入子
*/
template <typename Elem>
std::istream& operator>>(std::istream& is, std::vector<Elem>& seq)
{
    is >> *(seq.begin());
    for (auto it = seq.begin() + 1; it != seq.end(); it++) {
        is.ignore(1, VECTOR_IO_DELIM);
        is >> *it;
    }

    return is;
}

#endif
