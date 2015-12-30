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
#include <complex>
#include "autooperation.hpp"
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
}

template <class Seq>
std::ostream&operator<<(std::ostream &os, const std::Elemwise <Seq> &elemwise);

/*! @brief 実装
*/
namespace std {
    template <class Seq, typename T>
    class Elemwise_impl : public Seq, public devel::Five_operations <Elemwise_impl <Seq, T> >, public devel::Five_operations <Elemwise_impl <Seq, T>, T>, public devel::Order_operations2 <Elemwise_impl <Seq, T> > {
    public:
        using seq_type   = Seq;
        using value_type = T;
        using type       = Elemwise_impl <seq_type, value_type>;
    private:
        seq_type       *seq_this;
        const seq_type *cseq_this;
    public:
        /*! @brief コンストラクタ
        */
        Elemwise_impl(const seq_type &data)
            : seq_type(data), seq_this(static_cast <seq_type *>(this)), cseq_this(static_cast <const seq_type *>(this))
        {
        }

        Elemwise_impl() = delete;

        Elemwise_impl(const type &obj)
            : seq_type(static_cast <const seq_type&>(obj)), seq_this(static_cast <seq_type *>(this)), cseq_this(static_cast <const seq_type *>(this))
        {
        }

        Elemwise_impl&operator=(const type &obj)
        {
            seq_this->operator=(static_cast <const seq_type&>(obj));
            seq_this  = static_cast <seq_type *>(this);
            cseq_this = static_cast <const seq_type *>(this);

            return *this;
        }

        Elemwise_impl(type &&obj)
            : seq_type(std::move(static_cast <const seq_type&>(obj))), seq_this(static_cast <seq_type *>(this)), cseq_this(static_cast <const seq_type *>(this))
        {
        }

        Elemwise_impl&operator=(type &&obj)
        {
            seq_this->operator=(std::move(static_cast <const seq_type&>(obj)));
            seq_this  = static_cast <seq_type *>(this);
            cseq_this = static_cast <const seq_type *>(this);

            return *this;
        }

        /*! @brief 演算子たち
        */
        type&operator+=(const value_type &rhs)
        {
            std::for_each(seq_this->begin(), seq_this->end(), [&rhs](value_type &lelem) {
                lelem += rhs;
            });

            return *this;
        }

        type&operator+=(const type &rhs)
        {
            std::for_each(seq_this->begin(), seq_this->end(), rhs.begin(), [](value_type &lelem, const value_type &relem) {
                lelem += relem;
            });

            return *this;
        }

        type&operator-=(const value_type &rhs)
        {
            std::for_each(seq_this->begin(), seq_this->end(), [&rhs](value_type &lelem) {
                lelem -= rhs;
            });

            return *this;
        }

        type&operator-=(const type &rhs)
        {
            std::for_each(seq_this->begin(), seq_this->end(), rhs.begin(), [](value_type &lelem, const value_type &relem) {
                lelem -= relem;
            });

            return *this;
        }

        type&operator*=(const value_type &rhs)
        {
            std::for_each(seq_this->begin(), seq_this->end(), [&rhs](value_type &lelem) {
                lelem *= rhs;
            });

            return *this;
        }

        type&operator*=(const type &rhs)
        {
            std::for_each(seq_this->begin(), seq_this->end(), rhs.begin(), [](value_type &lelem, const value_type &relem) {
                lelem *= relem;
            });

            return *this;
        }

        type&operator/=(const value_type &rhs)
        {
            std::for_each(seq_this->begin(), seq_this->end(), [&rhs](value_type &lelem) {
                lelem /= rhs;
            });

            return *this;
        }

        type&operator/=(const type &rhs)
        {
            std::for_each(seq_this->begin(), seq_this->end(), rhs.begin(), [](value_type &lelem, const value_type &relem) {
                lelem /= relem;
            });

            return *this;
        }

        type&operator%=(const value_type &rhs)
        {
            std::for_each(seq_this->begin(), seq_this->end(), [&rhs](value_type &lelem) {
                lelem %= rhs;
            });

            return *this;
        }

        type&operator%=(const type &rhs)
        {
            std::for_each(seq_this->begin(), seq_this->end(), rhs.begin(), [](value_type &lelem, const value_type &relem) {
                lelem %= relem;
            });

            return *this;
        }

        bool operator<(const type &rhs) const
        {
            return std::lexicographical_compare(cseq_this->begin(), cseq_this->end(), rhs.begin(), rhs.end());
        }
    };

    namespace {
        template <class Seq, typename Scalar>
        struct Scalarchange;

        template <typename T, size_t N, typename Scalar>
        struct Scalarchange <std::array<T, N>, Scalar> {
            using type = std::array<Scalar, N>;
        };

        template <typename T, typename Scalar>
        struct Scalarchange <std::vector<T>, Scalar> {
            using type = std::vector<Scalar>;
        };
    }

    template <class Seq, typename Floating>
    class Elemwise_impl <Seq, std::complex<Floating>> : public Seq, public devel::Five_operations <Elemwise_impl <Seq, std::complex<Floating>> >, public devel::Five_operations <Elemwise_impl <Seq, std::complex<Floating>>, std::complex<Floating>>, public devel::Order_operations2 <Elemwise_impl <Seq, std::complex<Floating>> > {
    public:
        using seq_type   = Seq;
        using value_type = std::complex<Floating>;
        using type       = Elemwise_impl <seq_type, value_type>;
    private:
        seq_type       *seq_this;
        const seq_type *cseq_this;
    public:
        /*! @brief コンストラクタ
        */
        Elemwise_impl(const seq_type &data)
            : seq_type(data), seq_this(static_cast <seq_type *>(this)), cseq_this(static_cast <const seq_type *>(this))
        {
        }

        Elemwise_impl() = delete;

        Elemwise_impl(const type &obj)
            : seq_type(static_cast <const seq_type&>(obj)), seq_this(static_cast <seq_type *>(this)), cseq_this(static_cast <const seq_type *>(this))
        {
        }

        Elemwise_impl&operator=(const type &obj)
        {
            seq_this->operator=(static_cast <const seq_type&>(obj));
            seq_this  = static_cast <seq_type *>(this);
            cseq_this = static_cast <const seq_type *>(this);

            return *this;
        }

        Elemwise_impl(type &&obj)
            : seq_type(std::move(static_cast <const seq_type&>(obj))), seq_this(static_cast <seq_type *>(this)), cseq_this(static_cast <const seq_type *>(this))
        {
        }

        Elemwise_impl&operator=(type &&obj)
        {
            seq_this->operator=(std::move(static_cast <const seq_type&>(obj)));
            seq_this  = static_cast <seq_type *>(this);
            cseq_this = static_cast <const seq_type *>(this);

            return *this;
        }

        /*! @brief 演算子たち
        */
        type&operator+=(const value_type &rhs)
        {
            std::for_each(seq_this->begin(), seq_this->end(), [&rhs](value_type &lelem) {
                lelem += rhs;
            });

            return *this;
        }

        type&operator+=(const type &rhs)
        {
            std::for_each(seq_this->begin(), seq_this->end(), rhs.begin(), [](value_type &lelem, const value_type &relem) {
                lelem += relem;
            });

            return *this;
        }

        type&operator-=(const value_type &rhs)
        {
            std::for_each(seq_this->begin(), seq_this->end(), [&rhs](value_type &lelem) {
                lelem -= rhs;
            });

            return *this;
        }

        type&operator-=(const type &rhs)
        {
            std::for_each(seq_this->begin(), seq_this->end(), rhs.begin(), [](value_type &lelem, const value_type &relem) {
                lelem -= relem;
            });

            return *this;
        }

        type&operator*=(const value_type &rhs)
        {
            std::for_each(seq_this->begin(), seq_this->end(), [&rhs](value_type &lelem) {
                lelem *= rhs;
            });

            return *this;
        }

        type&operator*=(const type &rhs)
        {
            std::for_each(seq_this->begin(), seq_this->end(), rhs.begin(), [](value_type &lelem, const value_type &relem) {
                lelem *= relem;
            });

            return *this;
        }

        type&operator/=(const value_type &rhs)
        {
            std::for_each(seq_this->begin(), seq_this->end(), [&rhs](value_type &lelem) {
                lelem /= rhs;
            });

            return *this;
        }

        type&operator/=(const type &rhs)
        {
            std::for_each(seq_this->begin(), seq_this->end(), rhs.begin(), [](value_type &lelem, const value_type &relem) {
                lelem /= relem;
            });

            return *this;
        }

        type&operator%=(const value_type &rhs)
        {
            std::for_each(seq_this->begin(), seq_this->end(), [&rhs](value_type &lelem) {
                lelem %= rhs;
            });

            return *this;
        }

        type&operator%=(const type &rhs)
        {
            std::for_each(seq_this->begin(), seq_this->end(), rhs.begin(), [](value_type &lelem, const value_type &relem) {
                lelem %= relem;
            });

            return *this;
        }

        /*! @brief 複素数演算
        */
        Elemwise_impl<typename Scalarchange<Seq, Floating>::type, Floating> real() const
        {
            const seq_type &cseq_this(static_cast<const seq_type &>(*this));
            using tmp_result_type = typename Scalarchange<Seq, Floating>::type;
            using result_type = Elemwise_impl<tmp_result_type, Floating>;
            tmp_result_type retdata;
            std::transform(cseq_this->begin(), cseq_this->end(), retdata.begin(), [](value_type &elem)->Floating {
                return elem.real();
            });
            return result_type(retdata);
        }

    };
}

namespace std {
    template <typename T, size_t N>
    class Elemwise <std::array <T, N> > : public Elemwise_impl <std::array <T, N>, T> {
    public:
        using seq_type   = std::array<T, N>;
        using value_type = T;
        using type       = Elemwise <seq_type>;
        using base_type  = Elemwise_impl <std::array <T, N>, T>;
    public:
        /*! @brief コンストラクタ
        */
        Elemwise(const seq_type &data)
            : base_type(data)
        {
        }

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
    template <typename Floating, size_t N>
    class Elemwise <std::array <std::complex<Floating>, N> > : public Elemwise_impl <std::array <std::complex<Floating>, N>, std::complex<Floating>> {
    public:
        using base_type  = Elemwise_impl <std::array <std::complex<Floating>, N>, std::complex<Floating>>;
        using seq_type   = typename base_type::seq_type;
        using value_type = typename base_type::value_type;
        using type       = Elemwise <seq_type>;
    public:
        /*! @brief コンストラクタ
        */
        Elemwise(const seq_type &data)
            : base_type(data)
        {
        }

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

template <typename Floating, size_t N>
std::ostream&operator<<(std::ostream &os, const std::Elemwise <std::array <std::complex<Floating>, N> > &elemwise)
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

#endif
