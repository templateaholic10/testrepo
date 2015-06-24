#ifndef PROTOARRAY
#define PROTOARRAY

#include <iostream>
#include <cstddef>
#include <bitset>
#include <boost/optional.hpp>

namespace protoarray {
    // バイナリ上の長さnの配列
    template <std::size_t length>
    class Protobitarray
    {
        using element_t  = int;  // ただし2値
        using time_t     = int;
        using position_t = int;  // ただし0-origin
    public:
        Protobitarray();
        virtual ~Protobitarray()                      = default;
        Protobitarray(const Protobitarray&)           = delete;
        Protobitarray&operator=(const Protobitarray&) = delete;

        Protobitarray(Protobitarray&&)           = delete;
        Protobitarray&operator=(Protobitarray&&) = delete;

        // index \nin [0,n)で失敗する．
        virtual constexpr boost::optional <element_t> access(position_t index) = 0;

        // index \nin [0,n)で失敗する．
        virtual constexpr boost::optional <time_t> rank(element_t a, position_t index) = 0;

        // order <= 0またはorder > rank(a,length-1)で失敗する．
        virtual constexpr boost::optional <position_t> select(element_t a, time_t order) = 0;
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

        Protoarray(Protoarray&&)                                     = delete;
        Protoarray                          &operator=(Protoarray&&) = delete;

        virtual boost::optional <element_t>  access(position_t index) = 0;

        virtual boost::optional <time_t>     rank(element_t a, position_t index) = 0;

        virtual boost::optional <position_t> select(element_t a, time_t order) = 0;
    };
}

#include "detail/protoarray.hpp"

#endif
