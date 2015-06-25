#ifndef PROTOARRAY
#define PROTOARRAY

#include <iostream>
#include <cstddef>
#include <bitset>
#include <boost/optional.hpp>
#include "util.hpp"

namespace protoarray {
    // バイナリ上の長さnの配列
    // ビット""列""として見ているので1-origin
    template <std::size_t length>
    class Protobitarray
    {
    public:
        using length_t = util::bit_container_t<util::lg(length)>;
        using element_t = int;   // 要素（2値）
        // lengthが収まる型を用いる．
        using time_t     = length_t;  // 出現回数
        using position_t = length_t;  // 出現位置（1-origin）
    public:
        // Protobitarray();
        // virtual ~Protobitarray()                      = default;
        // Protobitarray(const Protobitarray&)           = delete;
        // Protobitarray&operator=(const Protobitarray&) = delete;
        //
        // Protobitarray(Protobitarray&&)           = delete;
        // Protobitarray&operator=(Protobitarray&&) = delete;

        // index \nin [1,n]で失敗する．
        virtual constexpr boost::optional <element_t> access(const position_t index) const = 0;

        // index \nin [1,n]で失敗する．
        virtual constexpr boost::optional <time_t> rank(const element_t a, const position_t index) const = 0;

        // order <= 0またはorder > rank(a,length)で失敗する．
        virtual constexpr boost::optional <position_t> select(const element_t a, const time_t order) const = 0;

        virtual constexpr size_t                       size() const = 0;

        friend std::ostream                           &operator<<(std::ostream &os, const Protobitarray <length> &pb);
    };

    // アルファベット[0, alphasup)上の長さnの配列
    template <int alphasup, std::size_t length>
    class Protoarray
    {
        using element_t  = int;
        using time_t     = int;
        using position_t = int;
    public:
        Protoarray();
        virtual ~Protoarray()                   = default;
        Protoarray(const Protoarray&)           = delete;
        Protoarray&operator=(const Protoarray&) = delete;

        Protoarray(Protoarray&&)                                    = delete;
        Protoarray                         &operator=(Protoarray&&) = delete;

        virtual boost::optional <element_t> access(const position_t index) const = 0;

        virtual boost::optional <time_t>    rank(const element_t a, const position_t index) const = 0;

        const
        virtual boost::optional <position_t> select(const element_t a, const time_t order) const = 0;

        virtual constexpr size_t             size() const = 0;
    };
}

#include "detail/protoarray.hpp"

#endif
