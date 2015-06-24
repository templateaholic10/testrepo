#ifndef GOODARRAY
#define GOODARRAY

#include <cmath>
#include <algorithm>
#include "protoarray.hpp"
#include "util.hpp"

namespace goodarray {
    template <std::size_t length>
    class Primitivebitarray
        : protoarray::Protobitarray <length>
    {
        using element_t  = typename protoarray::Protobitarray <length>::element_t;
        using time_t     = typename protoarray::Protobitarray <length>::time_t;
        using position_t = typename protoarray::Protobitarray <length>::position_t;
    public:
        constexpr Primitivebitarray();
        constexpr Primitivebitarray(const std::bitset <length> org_array);
        constexpr Primitivebitarray(const int org_array);
        // ~Primitivebitarray()                                  = default;
        // Primitivebitarray(const Primitivebitarray&)           = default;
        // Primitivebitarray&operator=(const Primitivebitarray&) = default;

        // Primitivebitarray(Primitivebitarray&&)           = default;
        // Primitivebitarray&operator=(Primitivebitarray&&) = default;

        // o(n)ビットの補助領域を用いてO(1)時間で解答．
        constexpr boost::optional <element_t> access(const position_t index) const;

        // Jacobsonの方法．o(n)ビットの補助領域でO(1)時間．
        constexpr boost::optional <time_t> rank(const element_t a, const position_t index) const;

        // 2分探索．O(logn)時間．
        constexpr boost::optional <position_t> select(const element_t a, const time_t order) const;

    private:
        constexpr void                         build();

        constexpr boost::optional <time_t>     rank1(const position_t index) const;

        constexpr boost::optional <position_t> select1(const time_t order) const;

        static constexpr size_t superblock_size     = util::power(util::lg(length), 2);
        static constexpr int    superblock_num      = util::iceil(static_cast <double>(length) / superblock_size);
        static constexpr size_t superblock_elemsize = util::lg(length);

        static constexpr size_t block_size         = util::lg(length);
        static constexpr int    block_num          = util::iceil(static_cast <double>(length) / block_size);
        static constexpr int    block_num_in_super = util::lg(length);
        static constexpr size_t block_elemsize     = util::lg(util::power(util::lg(length), 2));

        static constexpr size_t table_size     = util::power(2, (util::lg(length) + 1) / 2);
        static constexpr size_t table_elemsize = (util::lg(length) + 1) / 2;

        // ビットセットは先頭からインデックスづけする．
        const std::bitset <length>                                     _org_array;
        std::array <std::bitset <superblock_elemsize>, superblock_num> _superblock_rank1;
        std::array <std::bitset <block_elemsize>, block_num>           _block_rank1;
        std::array <std::bitset <table_elemsize>, table_size>          _lookup_table;
    };

    void testPrimitivebitarray();
}

#include "detail/goodarray.hpp"

#endif
