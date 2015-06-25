#ifndef UNIONARRAY
#define UNIONARRAY

#include <cmath>
#include <algorithm>
#include "protoarray.hpp"
#include "util.hpp"

namespace unionarray {
    // bit_container_tがnoneのとき，コンパイルエラー．
    template <std::size_t length>
    class Unionbitarray
        : protoarray::Protobitarray <length>
    {
    public:
        using length_t   = typename protoarray::Protobitarray <length>::length_t;
        using element_t  = typename protoarray::Protobitarray <length>::element_t;
        using time_t     = typename protoarray::Protobitarray <length>::time_t;
        using position_t = typename protoarray::Protobitarray <length>::position_t;
    public:
        constexpr Unionbitarray();
        constexpr Unionbitarray(const char org_array[length]);
        constexpr Unionbitarray(const unsigned long org_array);
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

        using block_t = util::bit_container_t <util::lg(length)>;

        static constexpr length_t block_size         = 8 * sizeof(block_t);
        static constexpr length_t block_num          = util::iceil(static_cast <double>(length) / block_size);
        static constexpr length_t block_num_in_super = 8 * sizeof(block_t);
        using block_rank_t = util::bit_container_t <util::lg(util::power(util::lg(length), 2))>;

        static constexpr length_t superblock_size = block_size * block_num_in_super;
        static constexpr length_t superblock_num  = util::iceil(static_cast <double>(length) / superblock_size);
        using superblock_rank_t = util::bit_container_t <util::lg(length)>;

        // 先頭からインデックスづけする．
        const util::constexpr_array <block_t, block_num>          _org_array;
        util::constexpr_array <superblock_rank_t, superblock_num> _superblock_rank1;
        util::constexpr_array <block_rank_t, block_num>           _block_rank1;
    };

    void testUnionbitarray();
}

#include "detail/unionarray.hpp"

#endif
