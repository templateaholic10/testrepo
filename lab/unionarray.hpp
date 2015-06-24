#ifndef UNIONARRAY
#define UNIONARRAY

#include <cmath>
#include <algorithm>
#include "protoarray.hpp"
#include "util.hpp"

namespace unionarray {
    template <std::size_t length>
    class Unionbitarray
        : protoarray::Protobitarray <length>
    {
    public:
        using element_t  = typename protoarray::Protobitarray <length>::element_t;
        using time_t     = typename protoarray::Protobitarray <length>::time_t;
        using position_t = typename protoarray::Protobitarray <length>::position_t;
    public:
        constexpr Unionbitarray();
        constexpr Unionbitarray(const std::bitset <length> org_array);
        constexpr Unionbitarray(const int org_array);
        // ~Unionbitarray()                                  = default;
        // Unionbitarray(const Unionbitarray&)           = default;
        // Unionbitarray&operator=(const Unionbitarray&) = default;

        // Unionbitarray(Unionbitarray&&)           = default;
        // Unionbitarray&operator=(Unionbitarray&&) = default;

        // O(logn)時間．
        constexpr boost::optional <element_t> access(const position_t index) const;

        // Jacobsonの方法を2分探索によってシンプルにしたもの．O(logn)時間．
        constexpr boost::optional <time_t> rank(const element_t a, const position_t index) const;

        // 2分探索．O(logn)時間．
        constexpr boost::optional <position_t> select(const element_t a, const time_t order) const;

        constexpr size_t                       size() const;

        template <std::size_t length1>
        friend std::ostream&operator<<(std::ostream &os, const Unionbitarray <length1> &pb);

    private:
        constexpr void                         build();

        constexpr boost::optional <time_t>     rank1(const position_t index) const;

        constexpr boost::optional <position_t> select1(const time_t order) const;

        constexpr boost::optional <position_t> select0(const time_t order) const;

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
    };

    void testUnionbitarray();
}

#include "detail/unionarray.hpp"

#endif
