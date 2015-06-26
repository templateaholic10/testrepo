#ifndef UNIONBITARRAY
#define UNIONBITARRAY

#include <cmath>
#include <algorithm>
#include <sprout/array.hpp>
#include <sprout/bitset.hpp>
#include "protoarray.hpp"
#include "util.hpp"

namespace unionbitarray {
    // bit_container_tがnoneのとき，コンパイルエラー．
    template <std::size_t length>
    class Unionbitarray
    : protoarray::Protobitarray<length>
    {
    public:
        using length_t  = typename protoarray::Protobitarray<length>::length_t;
        using element_t = typename protoarray::Protobitarray<length>::element_t;
        using time_t     = typename protoarray::Protobitarray<length>::time_t;
        using position_t = typename protoarray::Protobitarray<length>::position_t;

    public:
        constexpr Unionbitarray();
        constexpr Unionbitarray(const char org_array[length]);
        constexpr Unionbitarray(char org_array[length]);
        constexpr Unionbitarray(const unsigned long org_array);
        constexpr Unionbitarray(const sprout::array<unsigned char, length>& org_array);
        constexpr Unionbitarray(const sprout::bitset<length>& org_array);

        constexpr void set(const char org_array[length], bool rebuild = true);
        constexpr void set(const unsigned long org_array, bool rebuild = true);
        constexpr void set(const sprout::array<unsigned char, length>& org_array, bool rebuild = true);
        constexpr void set(const sprout::bitset<length>& org_array, bool rebuild = true);
        // ~Unionbitarray()                                  = default;
        // Unionbitarray(const Unionbitarray&)           = default;
        // Unionbitarray&operator=(const Unionbitarray&) = default;

        // Unionbitarray(Unionbitarray&&)           = default;
        // Unionbitarray&operator=(Unionbitarray&&) = default;

        // O(1)時間．
        constexpr unsigned long access(const position_t index) const;

        // Jacobsonの方法．O(1)時間．
        constexpr unsigned long rank(const element_t a, const position_t index) const;

        // 2分探索．O(logn)時間．
        constexpr unsigned long select(const element_t a, const time_t order) const;

        constexpr size_t        alphabet_size() const;
        constexpr size_t        size() const;
        std::string             to_string() const;
        constexpr unsigned long invalid_value() const;
        std::string             str() const;
        std::string             superblock_rank() const;
        std::string             block_rank() const;
        std::string             lookuptable() const;

        template <std::size_t length1>
        friend std::ostream&operator<<(std::ostream &os, const Unionbitarray <length1> &pb);

    private:
        constexpr void          build();

        constexpr unsigned long rank1(const position_t index) const;

        constexpr unsigned long select1(const time_t order) const;

        constexpr unsigned long select0(const time_t order) const;

    private:
        using block_t = util::bit_container_t <util::lg(length), false>;

        static constexpr length_t block_size         = 8 * sizeof(block_t);
        static constexpr length_t block_num          = util::iceil(static_cast <double>(length) / block_size);
        static constexpr length_t block_num_in_super = block_size;

        static constexpr length_t superblock_size = block_size * block_num_in_super;
        static constexpr length_t superblock_num  = util::iceil(static_cast <double>(length) / superblock_size);
        using superblock_rank_t = util::bit_container_t <util::lg(length), false>;

        using block_rank_t = util::bit_container_t <util::lg(superblock_size), false>;

        using halfblock_t = util::half_container_t <block_t, false>;
        static constexpr length_t halfblock_size = 8 * sizeof(halfblock_t);
        // 偶数になるように．つまりblockと範囲が一致するように．
        static constexpr length_t halfblock_num = std::is_same <block_t, halfblock_t>::value ? block_num : 2 * block_num;

        // block_sizeはビット長なので必ず偶数になる点に注意．
        static constexpr util::container_t <util::power(2, halfblock_size)> lookuptable_size = util::power(2, halfblock_size);
        // 最大halfblock_sizeの1が存在するため，halfblock_sizeを表現できる必要がある．
        using lookuptable_elem_t = util::container_t <halfblock_size>;
    private:
        // 先頭からインデックスづけする．
        sprout::array <halfblock_t, halfblock_num>           _org_array;
        sprout::array <superblock_rank_t, superblock_num>    _superblock_rank1;
        sprout::array <block_rank_t, block_num>              _block_rank1;
        sprout::array <lookuptable_elem_t, lookuptable_size> _lookuptable;
    };

    void testUnionbitarray();
}

#include "detail/unionbitarray.hpp"

#endif
