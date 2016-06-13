/*! @file
    @brief CRTP（奇妙に再帰したテンプレートパターン，上からの混入）の実験
    @author templateaholic10
    @date 11/9
*/
#ifndef CRTP_HPP
#define CRTP_HPP

#include <iostream>
#include <cassert>
#include <util>
#include <util_int>

/*! @namespace
    @brief CRTPとは，基底クラスに派生クラスのメンバを（上から）混入する技術．具体的には基底クラスをテンプレートクラスとして実装し，派生クラス自身で特殊化して継承する．基底クラスから派生クラスのメンバにアクセスするには，thisをstatic_castする
*/
namespace crtp {
    /*! @class
        @brief +の実装を，+=に委譲することで自動化するための基底クラス
        @tparam T +を実装したいクラス
    */
    template <class T>
    struct addable {
        /*! @brief 基底クラスでありながら派生クラスのメンバにアクセスできる
        */
        T operator+(const T &rhs) const
        {
            T tmp(static_cast <T const&>(*this));
            tmp += rhs;

            return tmp;
        }
    };

    /*! @class
        @brief *の実装を，*=に委譲することで自動化するための基底クラス
        @tparam T *を実装したいクラス
    */
    template <class T>
    struct multipliable {
        /*! @brief 基底クラスでありながら派生クラスのメンバにアクセスできる
        */
        T operator*(const T &rhs) const
        {
            T retval = static_cast <T const&>(*this);
            retval *= rhs;

            return retval;
        }
    };

    /*! @class
        @brief -=，-（二項）の実装を，-（単項）に委譲することで自動化するための基底クラス
        @tparam T -=，-（二項）を実装したいクラス
    */
    template <class T>
    struct subtractable {
        /*! @brief 基底クラスでありながら派生クラスのメンバにアクセスできる
        */
        T&operator-()
        {
            T *pointer = static_cast <T *>(this);
            pointer->opposite();

            return *pointer;
        }

        T&operator-=(const T &rhs)
        {
            T *pointer = static_cast <T *>(this);
            pointer->operator+=(rhs.opposite());

            return *pointer;
        }

        T operator-(const T &rhs) const
        {
            T retval = static_cast <T const&>(*this);
            retval -= rhs;

            return retval;
        }
    };

    /*! @class
        @brief /=，/の実装を，inverseに委譲することで自動化するための基底クラス
        @tparam T /=，/を実装したいクラス
    */
    template <class T>
    struct dividable {
        /*! @brief 基底クラスでありながら派生クラスのメンバにアクセスできる
        */
        T&operator/=(const T &rhs)
        {
            T *pointer = static_cast <T *>(this);
            pointer->operator*=(rhs.inverse());

            return *pointer;
        }

        T operator/(const T &rhs) const
        {
            T retval = static_cast <T const&>(*this);
            retval /= rhs;

            return retval;
        }
    };

    /*! @class
        @brief >=，==の実装を，<=に委譲することで自動化するための基底クラス
        @tparam T >=，==を実装したいクラス
    */
    template <class T>
    struct ordered {
        /*! @brief 基底クラスでありながら派生クラスのメンバにアクセスできる
        */
        bool operator>=(const T &rhs) const
        {
            const T *pointer = static_cast <const T *>(this);

            return rhs <= (*pointer);
        }

        bool operator==(const T &rhs) const
        {
            const T *pointer = static_cast <const T *>(this);

            return (*pointer) <= rhs && (*pointer) >= rhs;
        }

        bool operator!=(const T &rhs) const
        {
            const T *pointer = static_cast <const T *>(this);

            return !((*pointer) == rhs);
        }

        bool operator<(const T &rhs) const
        {
            const T *pointer = static_cast <const T *>(this);

            return (*pointer) <= rhs && (*pointer) != rhs;
        }

        bool operator>(const T &rhs) const
        {
            const T *pointer = static_cast <const T *>(this);

            return (*pointer) >= rhs && (*pointer) != rhs;
        }
    };

    /*! @class
        @brief 不完全な有理数クラス
    */
    class Rational
        : public addable <Rational>, public multipliable <Rational>, public subtractable <Rational>, public dividable <Rational>, public ordered <Rational>
    {
    private:
        int numerator;
        int denominator;
    public:
        Rational(const int numerator_, const int denominator_)
            : numerator(numerator_), denominator(denominator_)
        {
            assert(denominator_ != 0);
            reduce();
        }

        void reduce()
        {
            const int _gcd = util_int::gcd(numerator, denominator);
            numerator   = numerator / _gcd;
            denominator = denominator / _gcd;
        }

        Rational&operator+=(const Rational &rhs)
        {
            // 更新順に注意
            numerator   = numerator * rhs.denominator + rhs.numerator * denominator;
            denominator = denominator * rhs.denominator;
            reduce();

            return *this;
        }

        Rational&opposite()
        {
            numerator = -numerator;

            return *this;
        }

        Rational opposite() const
        {
            return Rational(-numerator, denominator);
        }

        Rational&operator*=(const Rational &rhs)
        {
            // 更新順に注意
            numerator   = numerator * rhs.numerator;
            denominator = denominator * rhs.denominator;
            reduce();

            return *this;
        }

        Rational &inverse()
        {
            std::swap(numerator, denominator);

            return *this;
        }

        Rational inverse() const
        {
            return Rational(denominator, numerator);
        }

        bool operator<=(const Rational &rhs) const
        {
            return numerator * rhs.denominator <= rhs.numerator * denominator;
        }

        friend std::ostream&operator<<(std::ostream &os, const Rational &r);
    };

    std::ostream&operator<<(std::ostream &os, const Rational &r)
    {
        os << r.numerator << '/' << r.denominator;

        return os;
    }

    /*! @brief テスト関数
    */
    void test_CRTP()
    {
        std::cout << util::Repeat("-", 20) << std::endl;
        std::cout << "TEST CRTP" << std::endl;
        Rational r1(1, 4);
        Rational r2(2, 4);

        _DISPLAY(r1)
        _DISPLAY(r2)

        _DISPLAY(r1 + r2)
        _DISPLAY(r1 - r2)
        _DISPLAY(r1 * r2)
        _DISPLAY(r1 / r2)
        _DISPLAY(r1 > r2)

        std::cout << util::Repeat("-", 20) << std::endl;
    }
}

#endif
