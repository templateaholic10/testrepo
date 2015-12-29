/*! @file
    @brief シーケンスのelemwiseクラス
    @auther yanteyon10
    @date 12/30
*/

#ifndef ELEMWISE_HPP
#define ELEMWISE_HPP

#include <iostream>
#include <cstdlib>
#include <type_traits>
#include <algorithm>
#include "exalgorithm.hpp"
#include <array>

namespace {
    /*! @brief デリミタ
    */
    constexpr char delim = ' ';
}

/*! @brief フレンド挿入子のためのプロトタイプ宣言
*/
namespace std {
    template <class Seq>
    class Elemwise;
}

template <class Seq>
std::ostream &operator<<(std::ostream &os, const std::Elemwise<Seq> &elemwise);

/*! @brief 実装
*/
namespace std {
    template <typename T, size_t N>
    class Elemwise <std::array<T, N>> : public std::array<T, N> {
    public:
        using seq_type = std::array<T, N>;
        using value_type = typename seq_type::value_type;
        using type = Elemwise<seq_type>;
    private:
        seq_type &seq_this;
        const seq_type &cseq_this;
    public:
        /*! @brief コンストラクタ
        */
        Elemwise(const seq_type &data)
        : seq_type(data), seq_this(static_cast<seq_type &>(*this)), cseq_this(static_cast<const seq_type &>(*this))
        {
        }
        /*! @brief arrayの取り出し
        */
        seq_type &array()
        {
            return seq_this;
        }
        /*! @brief 演算子たち
        */
        type &operator+=(const value_type &rhs)
        {
            std::for_each(seq_this.begin(), seq_this.end(), [&rhs](value_type& lelem){lelem += rhs;});
            return (*this);
        }
        type &operator+=(const type &rhs)
        {
            std::for_each(seq_this.begin(), seq_this.end(), rhs.begin(), [](value_type& lelem, const value_type &relem){lelem += relem;});
            return (*this);
        }
        /*! @brief 挿入子
        */
        template <typename T_, int N_>
        friend std::ostream &operator<<(std::ostream &os, const Elemwise<std::array<T_, N_>> &elemwise);
    };
}

template <typename T, size_t N>
std::ostream &operator<<(std::ostream &os, const std::Elemwise<std::array<T, N>> &elemwise)
{
    if (N <= 0) {
        return os;
    }
    os << *(elemwise.begin());
    for (auto it = elemwise.begin()+1; it != elemwise.end(); it++) {
        os << delim << (*it);
    }
    return os;
}

#endif
