/*! @file
    @brief シーケンスのelement_wise演算子
    @auther templateaholic10
    @date 11/19
*/

#ifndef ELEMENT_WISE_HPP
#define ELEMENT_WISE_HPP

#include <cstdlib>
#include <type_traits>
#include <util_sfinae>

/*! @brief シーケンスの要素ごとの恒等写像
*/
template <template <typename, size_t> class Seq, typename Elem, size_t n, typename std::enable_if<util::has_iterator<Seq<Elem, n>>::value>::type* = nullptr>
Seq<Elem, n> operator+(const Seq<Elem, n>& lhs)
{
    return lhs;
}

/*! @brief シーケンスの要素ごとの和
*/
template <template <typename, size_t> class Seq, typename Elem, size_t n, typename std::enable_if<util::has_iterator<Seq<Elem, n>>::value>::type* = nullptr>
Seq<Elem, n>& operator+=(Seq<Elem, n>& lhs, const Seq<Elem, n>& rhs)
{
    auto lit = lhs.begin();
    auto rit = rhs.begin();
    for (; lit != lhs.end(); lit++, rit++) {
        (*lit) += (*rit);
    }
    return lhs;
}

/*! @brief シーケンスの要素ごとの和
*/
template <template <typename, size_t> class Seq, typename Elem, size_t n, typename std::enable_if<util::has_iterator<Seq<Elem, n>>::value>::type* = nullptr>
Seq<Elem, n> operator+(const Seq<Elem, n>& lhs, const Seq<Elem, n>& rhs)
{
    Seq<Elem, n> retval(lhs);
    retval += rhs;
    return retval;
}

template <template <typename, size_t> class Seq, typename Elem, size_t n, typename std::enable_if<util::has_iterator<Seq<Elem, n>>::value>::type* = nullptr>
Seq<Elem, n>& operator+(Seq<Elem, n>&& lhs, const Seq<Elem, n>& rhs)
{
    lhs += rhs;
    return lhs;
}

template <template <typename, size_t> class Seq, typename Elem, size_t n, typename std::enable_if<util::has_iterator<Seq<Elem, n>>::value>::type* = nullptr>
Seq<Elem, n> operator+(const Seq<Elem, n>& lhs, Seq<Elem, n>&& rhs)
{
    // +が非可換の場合を考慮する
    auto lit = lhs.begin();
    auto rit = rhs.begin();
    for (; lit != lhs.end(); lit++, rit++) {
        (*rit) = (*lit) + (*rit);
    }
    return rhs;
}

template <template <typename, size_t> class Seq, typename Elem, size_t n, typename std::enable_if<util::has_iterator<Seq<Elem, n>>::value>::type* = nullptr>
Seq<Elem, n> operator+(Seq<Elem, n>&& lhs, Seq<Elem, n>&& rhs)
{
    lhs += rhs;
    return lhs;
}

/*! @brief シーケンスの要素ごとの反数
*/
template <template <typename, size_t> class Seq, typename Elem, size_t n, typename std::enable_if<util::has_iterator<Seq<Elem, n>>::value>::type* = nullptr>
Seq<Elem, n> operator-(const Seq<Elem, n>& lhs)
{
    Seq<Elem, n> retval;
    auto retit = retval.begin();
    auto lit = lhs.begin();
    for (; lit != lhs.end(); lit++, retit++) {
        (*retit) = -(*lit);
    }
    return retval;
}

template <template <typename, size_t> class Seq, typename Elem, size_t n, typename std::enable_if<util::has_iterator<Seq<Elem, n>>::value>::type* = nullptr>
Seq<Elem, n> operator-(Seq<Elem, n>&& lhs)
{
    auto lit = lhs.begin();
    for (; lit != lhs.end(); lit++) {
        (*lit) = -(*lit);
    }
    return lhs;
}

/*! @brief シーケンスの要素ごとの差
*/
template <template <typename, size_t> class Seq, typename Elem, size_t n, typename std::enable_if<util::has_iterator<Seq<Elem, n>>::value>::type* = nullptr>
Seq<Elem, n>& operator-=(Seq<Elem, n>& lhs, const Seq<Elem, n>& rhs)
{
    auto lit = lhs.begin();
    auto rit = rhs.begin();
    for (; lit != lhs.end(); lit++, rit++) {
        (*lit) -= (*rit);
    }
    return lhs;
}

/*! @brief シーケンスの要素ごとの差
*/
template <template <typename, size_t> class Seq, typename Elem, size_t n, typename std::enable_if<util::has_iterator<Seq<Elem, n>>::value>::type* = nullptr>
Seq<Elem, n> operator-(const Seq<Elem, n>& lhs, const Seq<Elem, n>& rhs)
{
    Seq<Elem, n> retval(lhs);
    retval -= rhs;
    return retval;
}

template <template <typename, size_t> class Seq, typename Elem, size_t n, typename std::enable_if<util::has_iterator<Seq<Elem, n>>::value>::type* = nullptr>
Seq<Elem, n>& operator-(Seq<Elem, n>&& lhs, const Seq<Elem, n>& rhs)
{
    lhs -= rhs;
    return lhs;
}

template <template <typename, size_t> class Seq, typename Elem, size_t n, typename std::enable_if<util::has_iterator<Seq<Elem, n>>::value>::type* = nullptr>
Seq<Elem, n> operator-(const Seq<Elem, n>& lhs, Seq<Elem, n>&& rhs)
{
    auto lit = lhs.begin();
    auto rit = rhs.begin();
    for (; lit != lhs.end(); lit++, rit++) {
        (*rit) = (*lit) - (*rit);
    }
    return rhs;
}

template <template <typename, size_t> class Seq, typename Elem, size_t n, typename std::enable_if<util::has_iterator<Seq<Elem, n>>::value>::type* = nullptr>
Seq<Elem, n> operator-(Seq<Elem, n>&& lhs, Seq<Elem, n>&& rhs)
{
    lhs -= rhs;
    return lhs;
}

/*! @brief シーケンスの要素ごとの積
*/
template <template <typename, size_t> class Seq, typename Elem, size_t n, typename std::enable_if<util::has_iterator<Seq<Elem, n>>::value>::type* = nullptr>
Seq<Elem, n>& operator*=(Seq<Elem, n>& lhs, const Seq<Elem, n>& rhs)
{
    auto lit = lhs.begin();
    auto rit = rhs.begin();
    for (; lit != lhs.end(); lit++, rit++) {
        (*lit) *= (*rit);
    }
    return lhs;
}

/*! @brief シーケンスの要素ごとの積
*/
template <template <typename, size_t> class Seq, typename Elem, size_t n, typename std::enable_if<util::has_iterator<Seq<Elem, n>>::value>::type* = nullptr>
Seq<Elem, n> operator*(const Seq<Elem, n>& lhs, const Seq<Elem, n>& rhs)
{
    Seq<Elem, n> retval(lhs);
    retval *= rhs;
    return retval;
}

template <template <typename, size_t> class Seq, typename Elem, size_t n, typename std::enable_if<util::has_iterator<Seq<Elem, n>>::value>::type* = nullptr>
Seq<Elem, n>& operator*(Seq<Elem, n>&& lhs, const Seq<Elem, n>& rhs)
{
    lhs *= rhs;
    return lhs;
}

template <template <typename, size_t> class Seq, typename Elem, size_t n, typename std::enable_if<util::has_iterator<Seq<Elem, n>>::value>::type* = nullptr>
Seq<Elem, n> operator*(const Seq<Elem, n>& lhs, Seq<Elem, n>&& rhs)
{
    // *が非可換の場合を考慮する
    auto lit = lhs.begin();
    auto rit = rhs.begin();
    for (; lit != lhs.end(); lit++, rit++) {
        (*rit) = (*lit) * (*rit);
    }
    return rhs;
}

template <template <typename, size_t> class Seq, typename Elem, size_t n, typename std::enable_if<util::has_iterator<Seq<Elem, n>>::value>::type* = nullptr>
Seq<Elem, n> operator*(Seq<Elem, n>&& lhs, Seq<Elem, n>&& rhs)
{
    lhs *= rhs;
    return lhs;
}

/*! @brief シーケンスの要素ごとの商
*/
template <template <typename, size_t> class Seq, typename Elem, size_t n, typename std::enable_if<util::has_iterator<Seq<Elem, n>>::value>::type* = nullptr>
Seq<Elem, n>& operator/=(Seq<Elem, n>& lhs, const Seq<Elem, n>& rhs)
{
    auto lit = lhs.begin();
    auto rit = rhs.begin();
    for (; lit != lhs.end(); lit++, rit++) {
        (*lit) /= (*rit);
    }
    return lhs;
}

/*! @brief シーケンスの要素ごとの商
*/
template <template <typename, size_t> class Seq, typename Elem, size_t n, typename std::enable_if<util::has_iterator<Seq<Elem, n>>::value>::type* = nullptr>
Seq<Elem, n> operator/(const Seq<Elem, n>& lhs, const Seq<Elem, n>& rhs)
{
    Seq<Elem, n> retval(lhs);
    retval /= rhs;
    return retval;
}

template <template <typename, size_t> class Seq, typename Elem, size_t n, typename std::enable_if<util::has_iterator<Seq<Elem, n>>::value>::type* = nullptr>
Seq<Elem, n>& operator/(Seq<Elem, n>&& lhs, const Seq<Elem, n>& rhs)
{
    lhs /= rhs;
    return lhs;
}

template <template <typename, size_t> class Seq, typename Elem, size_t n, typename std::enable_if<util::has_iterator<Seq<Elem, n>>::value>::type* = nullptr>
Seq<Elem, n> operator/(const Seq<Elem, n>& lhs, Seq<Elem, n>&& rhs)
{
    auto lit = lhs.begin();
    auto rit = rhs.begin();
    for (; lit != lhs.end(); lit++, rit++) {
        (*rit) = (*lit) / (*rit);
    }
    return rhs;
}

template <template <typename, size_t> class Seq, typename Elem, size_t n, typename std::enable_if<util::has_iterator<Seq<Elem, n>>::value>::type* = nullptr>
Seq<Elem, n> operator/(Seq<Elem, n>&& lhs, Seq<Elem, n>&& rhs)
{
    lhs /= rhs;
    return lhs;
}

/*! @brief シーケンスの要素ごとの剰余
*/
template <template <typename, size_t> class Seq, typename Elem, size_t n, typename std::enable_if<util::has_iterator<Seq<Elem, n>>::value>::type* = nullptr>
Seq<Elem, n>& operator%=(Seq<Elem, n>& lhs, const Seq<Elem, n>& rhs)
{
    auto lit = lhs.begin();
    auto rit = rhs.begin();
    for (; lit != lhs.end(); lit++, rit++) {
        (*lit) %= (*rit);
    }
    return lhs;
}

/*! @brief シーケンスの要素ごとの剰余
*/
template <template <typename, size_t> class Seq, typename Elem, size_t n, typename std::enable_if<util::has_iterator<Seq<Elem, n>>::value>::type* = nullptr>
Seq<Elem, n> operator%(const Seq<Elem, n>& lhs, const Seq<Elem, n>& rhs)
{
    Seq<Elem, n> retval(lhs);
    retval %= rhs;
    return retval;
}

template <template <typename, size_t> class Seq, typename Elem, size_t n, typename std::enable_if<util::has_iterator<Seq<Elem, n>>::value>::type* = nullptr>
Seq<Elem, n>& operator%(Seq<Elem, n>&& lhs, const Seq<Elem, n>& rhs)
{
    lhs %= rhs;
    return lhs;
}

template <template <typename, size_t> class Seq, typename Elem, size_t n, typename std::enable_if<util::has_iterator<Seq<Elem, n>>::value>::type* = nullptr>
Seq<Elem, n> operator%(const Seq<Elem, n>& lhs, Seq<Elem, n>&& rhs)
{
    auto lit = lhs.begin();
    auto rit = rhs.begin();
    for (; lit != lhs.end(); lit++, rit++) {
        (*rit) = (*lit) % (*rit);
    }
    return rhs;
}

template <template <typename, size_t> class Seq, typename Elem, size_t n, typename std::enable_if<util::has_iterator<Seq<Elem, n>>::value>::type* = nullptr>
Seq<Elem, n> operator%(Seq<Elem, n>&& lhs, Seq<Elem, n>&& rhs)
{
    lhs %= rhs;
    return lhs;
}

#endif
