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
#include <exalgorithm>
#include <array>
#include <vector>
#include <excomplex>
#include <autooperation>
#include <debug>

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

    template <class Derived, typename Seq_type, typename Value_type, typename Ignored = void>
    class Elemwise_impl;
}

template <class Seq>
std::ostream&operator<<(std::ostream &os, const std::Elemwise <Seq> &elemwise);

/*! @brief 実装
*/
namespace std {
    template <typename T, size_t N>
    class Elemwise <std::array <T, N> > : public Elemwise_impl <Elemwise <std::array <T, N> >, std::array <T, N>, T> {
    public:
        using seq_type   = std::array <T, N>;
        using value_type = T;
        using type       = Elemwise <seq_type>;
        using base_type  = Elemwise_impl <type, seq_type, value_type>;
    public:
        /*! @brief コンストラクタ
        */
        Elemwise(const seq_type &data)
            : base_type(data)
        {
        }

        Elemwise(seq_type &&data)
            : base_type(std::move(data))
        {
        }

        Elemwise() = delete;

        /*! @brief arrayの取り出し
        */
        seq_type &array()
        {
            return static_cast <seq_type&>(*this);
        }

        /*! @brief 挿入子
        */
        template <typename T_, int N_>
        friend std::ostream&operator<<(std::ostream &os, const Elemwise <std::array <T_, N_> > &elemwise);
    };
}

template <typename T, size_t N>
std::ostream&operator<<(std::ostream &os, const std::Elemwise <std::array <T, N> > &elemwise)
{
    if (N <= 0) {
        return os;
    }
    os << *(elemwise.begin());
    for (auto it = elemwise.begin() + 1; it != elemwise.end(); it++) {
        os << delim << (*it);
    }

    return os;
}

namespace std {
    template <typename T>
    class Elemwise <std::vector <T> > : public Elemwise_impl <Elemwise <std::vector <T> >, std::vector <T>, T> {
    public:
        using seq_type   = std::vector <T>;
        using value_type = T;
        using type       = Elemwise <seq_type>;
        using base_type  = Elemwise_impl <type, seq_type, value_type>;
    public:
        /*! @brief コンストラクタ
        */
        Elemwise(const seq_type &data)
            : base_type(data)
        {
        }

        Elemwise(seq_type &&data)
            : base_type(std::move(data))
        {
        }

        Elemwise() = delete;

        /*! @brief vectorの取り出し
        */
        seq_type &vector()
        {
            return static_cast <seq_type&>(*this);
        }

        /*! @brief 挿入子
        */
        template <typename T_>
        friend std::ostream&operator<<(std::ostream &os, const Elemwise <std::vector <T_> > &elemwise);
    };
}

template <typename T>
std::ostream&operator<<(std::ostream &os, const std::Elemwise <std::vector <T> > &elemwise)
{
    const int N = elemwise.size();
    if (N <= 0) {
        return os;
    }
    os << *(elemwise.begin());
    for (auto it = elemwise.begin() + 1; it != elemwise.end(); it++) {
        os << delim << (*it);
    }

    return os;
}

namespace std {
    /*! @brief 要素演算用の構造体であるから，例えば+=でなく+であっても自らを更新すればよい
    */
    template <class Derived, typename Seq_type, typename Value_type, typename Ignored>
    class Elemwise_impl : public Seq_type, public
                          devel::Order_operations2 <Elemwise_impl <Derived, Seq_type, Value_type> > {
    public:
        using seq_type   = Seq_type;
        using value_type = Value_type;
        using type       = Elemwise_impl <Derived, Seq_type, Value_type>;
    public:
        /*! @brief コンストラクタ
        */
        Elemwise_impl(const seq_type &data)
            : seq_type(data)
        {
        }

        Elemwise_impl(seq_type &&data)
            : seq_type(std::move(data))
        {
        }

        Elemwise_impl() = delete;

        /*! @brief 演算子たち．演算用の一時的な構造体なので，+=でなく+であっても自身を更新する．再利用には注意が必要
        */
        Derived&operator+=(const value_type &rhs)
        {
            seq_type &seq_this(static_cast<seq_type &>(*this));
            std::for_each(seq_this.begin(), seq_this.end(), [&rhs](value_type &lelem) {
                lelem += rhs;
            });

            return static_cast<Derived&>(*this);
        }

        Derived&operator+(const value_type &rhs)
        {
            return operator+=(rhs);
        }

        Derived&operator+=(const seq_type &rhs)
        {
            seq_type &seq_this(static_cast<seq_type &>(*this));
            std::for_each(seq_this.begin(), seq_this.end(), rhs.begin(), [](value_type &lelem, const value_type &relem) {
                lelem += relem;
            });

            return static_cast<Derived&>(*this);
        }

        Derived&operator+(const seq_type &rhs)
        {
            return operator+=(rhs);
        }

        Derived&operator+=(const type &rhs)
        {
            seq_type &seq_this(static_cast<seq_type &>(*this));
            std::for_each(seq_this.begin(), seq_this.end(), rhs.begin(), [](value_type &lelem, const value_type &relem) {
                lelem += relem;
            });

            return static_cast<Derived&>(*this);
        }

        Derived&operator+(const type &rhs)
        {
            return operator+=(rhs);
        }

        Derived&operator-=(const value_type &rhs)
        {
            seq_type &seq_this(static_cast<seq_type &>(*this));
            std::for_each(seq_this.begin(), seq_this.end(), [&rhs](value_type &lelem) {
                lelem -= rhs;
            });

            return static_cast<Derived&>(*this);
        }

        Derived&operator-(const value_type &rhs)
        {
            return operator-=(rhs);
        }

        Derived&operator-=(const seq_type &rhs)
        {
            seq_type &seq_this(static_cast<seq_type &>(*this));
            std::for_each(seq_this.begin(), seq_this.end(), rhs.begin(), [](value_type &lelem, const value_type &relem) {
                lelem -= relem;
            });

            return static_cast<Derived&>(*this);
        }

        Derived&operator-(const seq_type &rhs)
        {
            return operator-=(rhs);
        }

        Derived&operator-=(const type &rhs)
        {
            seq_type &seq_this(static_cast<seq_type &>(*this));
            std::for_each(seq_this.begin(), seq_this.end(), rhs.begin(), [](value_type &lelem, const value_type &relem) {
                lelem -= relem;
            });

            return static_cast<Derived&>(*this);
        }

        Derived&operator-(const type &rhs)
        {
            return operator-=(rhs);
        }

        Derived&operator-()
        {
            seq_type &seq_this(static_cast<seq_type &>(*this));
            std::for_each(seq_this.begin(), seq_this.end(), [](value_type &lelem) {
                lelem = -lelem;
            });

            return static_cast<Derived&>(*this);
        }

        Derived&operator*=(const value_type &rhs)
        {
            seq_type &seq_this(static_cast<seq_type &>(*this));
            std::for_each(seq_this.begin(), seq_this.end(), [&rhs](value_type &lelem) {
                lelem *= rhs;
            });

            return static_cast<Derived&>(*this);
        }

        Derived&operator*(const value_type &rhs)
        {
            return operator*=(rhs);
        }

        Derived&operator*=(const seq_type &rhs)
        {
            seq_type &seq_this(static_cast<seq_type &>(*this));
            std::for_each(seq_this.begin(), seq_this.end(), rhs.begin(), [](value_type &lelem, const value_type &relem) {
                lelem *= relem;
            });

            return static_cast<Derived&>(*this);
        }

        Derived&operator*(const seq_type &rhs)
        {
            return operator*=(rhs);
        }

        Derived&operator*=(const type &rhs)
        {
            seq_type &seq_this(static_cast<seq_type &>(*this));
            std::for_each(seq_this.begin(), seq_this.end(), rhs.begin(), [](value_type &lelem, const value_type &relem) {
                lelem *= relem;
            });

            return static_cast<Derived&>(*this);
        }

        Derived&operator*(const type &rhs)
        {
            return operator*=(rhs);
        }

        Derived&operator/=(const value_type &rhs)
        {
            seq_type &seq_this(static_cast<seq_type &>(*this));
            std::for_each(seq_this.begin(), seq_this.end(), [&rhs](value_type &lelem) {
                lelem /= rhs;
            });

            return static_cast<Derived&>(*this);
        }

        Derived&operator/(const value_type &rhs)
        {
            return operator/=(rhs);
        }

        Derived&operator/=(const seq_type &rhs)
        {
            seq_type &seq_this(static_cast<seq_type &>(*this));
            std::for_each(seq_this.begin(), seq_this.end(), rhs.begin(), [](value_type &lelem, const value_type &relem) {
                lelem /= relem;
            });

            return static_cast<Derived&>(*this);
        }

        Derived&operator/(const seq_type &rhs)
        {
            return operator/=(rhs);
        }

        Derived&operator/=(const type &rhs)
        {
            seq_type &seq_this(static_cast<seq_type &>(*this));
            std::for_each(seq_this.begin(), seq_this.end(), rhs.begin(), [](value_type &lelem, const value_type &relem) {
                lelem /= relem;
            });

            return static_cast<Derived&>(*this);
        }

        Derived&operator/(const type &rhs)
        {
            return operator/=(rhs);
        }

        Derived&operator%=(const value_type &rhs)
        {
            seq_type &seq_this(static_cast<seq_type &>(*this));
            std::for_each(seq_this.begin(), seq_this.end(), [&rhs](value_type &lelem) {
                lelem %= rhs;
            });

            return static_cast<Derived&>(*this);
        }

        Derived&operator%(const value_type &rhs)
        {
            return operator%=(rhs);
        }

        Derived&operator%=(const seq_type &rhs)
        {
            seq_type &seq_this(static_cast<seq_type &>(*this));
            std::for_each(seq_this.begin(), seq_this.end(), rhs.begin(), [](value_type &lelem, const value_type &relem) {
                lelem %= relem;
            });

            return static_cast<Derived&>(*this);
        }

        Derived&operator%(const seq_type &rhs)
        {
            return operator%=(rhs);
        }

        Derived&operator%=(const type &rhs)
        {
            seq_type &seq_this(static_cast<seq_type &>(*this));
            std::for_each(seq_this.begin(), seq_this.end(), rhs.begin(), [](value_type &lelem, const value_type &relem) {
                lelem %= relem;
            });

            return static_cast<Derived&>(*this);
        }

        Derived&operator%(const type &rhs)
        {
            return operator%=(rhs);
        }

        bool operator<(const type &rhs) const
        {
            const seq_type &cseq_this(static_cast<const seq_type &>(*this));
            return std::lexicographical_compare(cseq_this.begin(), cseq_this.end(), rhs.begin(), rhs.end());
        }
    };

    namespace {
        template <class Seq, typename Scalar>
        struct Scalarchange;

        template <typename T, size_t N, typename Scalar>
        struct Scalarchange <std::array <T, N>, Scalar> {
            using type = std::array <Scalar, N>;
        };

        template <typename T, typename Scalar>
        struct Scalarchange <std::vector <T>, Scalar> {
            using type = std::vector <Scalar>;
        };
    }

    template <class Derived, typename Seq_type, typename Value_type>
    class Elemwise_impl <Derived, Seq_type, Value_type, typename std::enable_if <std::is_complex <Value_type>::value>::type> : public Seq_type, devel::Equi_operations<Elemwise_impl<Derived, Seq_type, Value_type>> {
    public:
        using seq_type   = Seq_type;
        using value_type = Value_type;
        using type       = Elemwise_impl <Derived, Seq_type, Value_type>;
    public:
        /*! @brief コンストラクタ
        */
        Elemwise_impl(const seq_type &data)
            : seq_type(data)
        {
        }

        Elemwise_impl(seq_type &&data)
            : seq_type(std::move(data))
        {
        }

        Elemwise_impl() = delete;

        /*! @brief 演算子たち．演算用の一時的な構造体なので，+=でなく+であっても自身を更新する．再利用には注意が必要
        */
        Derived&operator+=(const value_type &rhs)
        {
            seq_type &seq_this(static_cast<seq_type &>(*this));
            std::for_each(seq_this.begin(), seq_this.end(), [&rhs](value_type &lelem) {
                lelem += rhs;
            });

            return static_cast<Derived&>(*this);
        }

        Derived&operator+(const value_type &rhs)
        {
            return operator+=(rhs);
        }

        Derived&operator+=(const seq_type &rhs)
        {
            seq_type &seq_this(static_cast<seq_type &>(*this));
            std::for_each(seq_this.begin(), seq_this.end(), rhs.begin(), [](value_type &lelem, const value_type &relem) {
                lelem += relem;
            });

            return static_cast<Derived&>(*this);
        }

        Derived&operator+(const seq_type &rhs)
        {
            return operator+=(rhs);
        }

        Derived&operator+=(const type &rhs)
        {
            seq_type &seq_this(static_cast<seq_type &>(*this));
            std::for_each(seq_this.begin(), seq_this.end(), rhs.begin(), [](value_type &lelem, const value_type &relem) {
                lelem += relem;
            });

            return static_cast<Derived&>(*this);
        }

        Derived&operator+(const type &rhs)
        {
            return operator+=(rhs);
        }

        Derived&operator-=(const value_type &rhs)
        {
            seq_type &seq_this(static_cast<seq_type &>(*this));
            std::for_each(seq_this.begin(), seq_this.end(), [&rhs](value_type &lelem) {
                lelem -= rhs;
            });

            return static_cast<Derived&>(*this);
        }

        Derived&operator-(const value_type &rhs)
        {
            return operator-=(rhs);
        }

        Derived&operator-=(const seq_type &rhs)
        {
            seq_type &seq_this(static_cast<seq_type &>(*this));
            std::for_each(seq_this.begin(), seq_this.end(), rhs.begin(), [](value_type &lelem, const value_type &relem) {
                lelem -= relem;
            });

            return static_cast<Derived&>(*this);
        }

        Derived&operator-(const seq_type &rhs)
        {
            return operator-=(rhs);
        }

        Derived&operator-=(const type &rhs)
        {
            seq_type &seq_this(static_cast<seq_type &>(*this));
            std::for_each(seq_this.begin(), seq_this.end(), rhs.begin(), [](value_type &lelem, const value_type &relem) {
                lelem -= relem;
            });

            return static_cast<Derived&>(*this);
        }

        Derived&operator-(const type &rhs)
        {
            return operator-=(rhs);
        }

        Derived&operator*=(const value_type &rhs)
        {
            seq_type &seq_this(static_cast<seq_type &>(*this));
            std::for_each(seq_this.begin(), seq_this.end(), [&rhs](value_type &lelem) {
                lelem *= rhs;
            });

            return static_cast<Derived&>(*this);
        }

        Derived&operator*(const value_type &rhs)
        {
            return operator*=(rhs);
        }

        Derived&operator*=(const seq_type &rhs)
        {
            seq_type &seq_this(static_cast<seq_type &>(*this));
            std::for_each(seq_this.begin(), seq_this.end(), rhs.begin(), [](value_type &lelem, const value_type &relem) {
                lelem *= relem;
            });

            return static_cast<Derived&>(*this);
        }

        Derived&operator*(const seq_type &rhs)
        {
            return operator*=(rhs);
        }

        Derived&operator*=(const type &rhs)
        {
            seq_type &seq_this(static_cast<seq_type &>(*this));
            std::for_each(seq_this.begin(), seq_this.end(), rhs.begin(), [](value_type &lelem, const value_type &relem) {
                lelem *= relem;
            });

            return static_cast<Derived&>(*this);
        }

        Derived&operator*(const type &rhs)
        {
            return operator*=(rhs);
        }

        Derived&operator/=(const value_type &rhs)
        {
            seq_type &seq_this(static_cast<seq_type &>(*this));
            std::for_each(seq_this.begin(), seq_this.end(), [&rhs](value_type &lelem) {
                lelem /= rhs;
            });

            return static_cast<Derived&>(*this);
        }

        Derived&operator/(const value_type &rhs)
        {
            return operator/=(rhs);
        }

        Derived&operator/=(const seq_type &rhs)
        {
            seq_type &seq_this(static_cast<seq_type &>(*this));
            std::for_each(seq_this.begin(), seq_this.end(), rhs.begin(), [](value_type &lelem, const value_type &relem) {
                lelem /= relem;
            });

            return static_cast<Derived&>(*this);
        }

        Derived&operator/(const seq_type &rhs)
        {
            return operator/=(rhs);
        }

        Derived&operator/=(const type &rhs)
        {
            seq_type &seq_this(static_cast<seq_type &>(*this));
            std::for_each(seq_this.begin(), seq_this.end(), rhs.begin(), [](value_type &lelem, const value_type &relem) {
                lelem /= relem;
            });

            return static_cast<Derived&>(*this);
        }

        Derived&operator/(const type &rhs)
        {
            return operator/=(rhs);
        }

        Derived&operator%=(const value_type &rhs)
        {
            seq_type &seq_this(static_cast<seq_type &>(*this));
            std::for_each(seq_this.begin(), seq_this.end(), [&rhs](value_type &lelem) {
                lelem %= rhs;
            });

            return static_cast<Derived&>(*this);
        }

        Derived&operator%(const value_type &rhs)
        {
            return operator%=(rhs);
        }

        Derived&operator%=(const seq_type &rhs)
        {
            seq_type &seq_this(static_cast<seq_type &>(*this));
            std::for_each(seq_this.begin(), seq_this.end(), rhs.begin(), [](value_type &lelem, const value_type &relem) {
                lelem %= relem;
            });

            return static_cast<Derived&>(*this);
        }

        Derived&operator%(const seq_type &rhs)
        {
            return operator%=(rhs);
        }

        Derived&operator%=(const type &rhs)
        {
            seq_type &seq_this(static_cast<seq_type &>(*this));
            std::for_each(seq_this.begin(), seq_this.end(), rhs.begin(), [](value_type &lelem, const value_type &relem) {
                lelem %= relem;
            });

            return static_cast<Derived&>(*this);
        }

        Derived&operator%(const type &rhs)
        {
            return operator%=(rhs);
        }

        /*! @brief 複素数演算
        */
        Elemwise <typename Scalarchange <seq_type, typename std::decomplexify <value_type>::type>::type> real() const
        {
            const seq_type&cseq_this(static_cast <const seq_type&>(*this));
            using tmp_result_type = typename Scalarchange <seq_type, typename std::decomplexify <value_type>::type>::type;
            using result_type     = Elemwise <tmp_result_type>;
            tmp_result_type retdata;
            std::transform(cseq_this.begin(), cseq_this.end(), retdata.begin(), [](const value_type &elem) {
                return elem.real();
            });

            return result_type(retdata);
        }

        Elemwise <typename Scalarchange <seq_type, typename std::decomplexify <value_type>::type>::type> imag() const
        {
            const seq_type&cseq_this(static_cast <const seq_type&>(*this));
            using tmp_result_type = typename Scalarchange <seq_type, typename std::decomplexify <value_type>::type>::type;
            using result_type     = Elemwise <tmp_result_type>;
            tmp_result_type retdata;
            std::transform(cseq_this.begin(), cseq_this.end(), retdata.begin(), [](const value_type &elem) {
                return elem.imag();
            });

            return result_type(retdata);
        }

        Elemwise <typename Scalarchange <seq_type, typename std::decomplexify <value_type>::type>::type> abs() const
        {
            const seq_type&cseq_this(static_cast <const seq_type&>(*this));
            using tmp_result_type = typename Scalarchange <seq_type, typename std::decomplexify <value_type>::type>::type;
            using result_type     = Elemwise <tmp_result_type>;
            tmp_result_type retdata;
            std::transform(cseq_this.begin(), cseq_this.end(), retdata.begin(), [](const value_type &elem) {
                return std::abs(elem);
            });

            return result_type(retdata);
        }

        Elemwise <typename Scalarchange <seq_type, typename std::decomplexify <value_type>::type>::type> arg() const
        {
            const seq_type&cseq_this(static_cast <const seq_type&>(*this));
            using tmp_result_type = typename Scalarchange <seq_type, typename std::decomplexify <value_type>::type>::type;
            using result_type     = Elemwise <tmp_result_type>;
            tmp_result_type retdata;
            std::transform(cseq_this.begin(), cseq_this.end(), retdata.begin(), [](const value_type &elem) {
                return std::arg(elem);
            });

            return result_type(retdata);
        }

        Elemwise <typename Scalarchange <seq_type, typename std::decomplexify <value_type>::type>::type> norm() const
        {
            const seq_type&cseq_this(static_cast <const seq_type&>(*this));
            using tmp_result_type = typename Scalarchange <seq_type, typename std::decomplexify <value_type>::type>::type;
            using result_type     = Elemwise <tmp_result_type>;
            tmp_result_type retdata;
            std::transform(cseq_this.begin(), cseq_this.end(), retdata.begin(), [](const value_type &elem) {
                return std::norm(elem);
            });

            return result_type(retdata);
        }

        Derived conj() const
        {
            const seq_type&cseq_this(static_cast <const seq_type&>(*this));
            using tmp_result_type = seq_type;
            using result_type     = Derived;
            tmp_result_type retdata;
            std::transform(cseq_this.begin(), cseq_this.end(), retdata.begin(), [](const value_type &elem) {
                return std::conj(elem);
            });

            return result_type(retdata);
        }

        bool operator==(const type &rhs) const
        {
            const seq_type &cseq_this(static_cast<const seq_type &>(*this));
            return std::equal(cseq_this.begin(), cseq_this.end(), rhs.begin(), rhs.end());
        }
    };
}

#endif
