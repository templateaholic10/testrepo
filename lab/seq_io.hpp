/*! @file
    @brief シーケンスI/O
    @author templateaholic10
    @date 11/18
*/
#ifndef SEQ_IO_HPP
#define SEQ_IO_HPP

#include <iostream>
#include <cstdlib>
#include <type_traits>
#include <util_sfinae>

/*! @macro
    @brief デリミタ．インクルード前にSEQ_IO_DELIMを定義することで変更が可能
*/
#ifndef SEQ_IO_DELIM
#define SEQ_IO_DELIM ','
#endif

/*! @brief シーケンス抽出子
*/
template <template <typename, size_t> class Seq, typename Elem, size_t n, typename std::enable_if<util::has_iterator<Seq<Elem, n>>::value>::type* = nullptr>
std::ostream& operator<<(std::ostream& os, const Seq<Elem, n>& seq)
{
    if (n == 0) {
        return os;
    }
    os << *(seq.begin());
    for (auto it = seq.begin()+1; it != seq.end(); it++) {
        os << SEQ_IO_DELIM << *it;
    }
    return os;
}

/*! @brief シーケンス挿入子
*/
template <template <typename, size_t> class Seq, typename Elem, size_t n, typename std::enable_if<util::has_iterator<Seq<Elem, n>>::value>::type* = nullptr>
std::istream& operator>>(std::istream& is, Seq<Elem, n>& seq)
{
    for (auto elem : seq) {
        is >> elem;
        is.ignore(1, SEQ_IO_DELIM);
    }
    return is;
}

#endif
