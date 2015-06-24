#ifndef DETAIL_GOODARRAY
#define DETAIL_GOODARRAY

#include "../goodarray.hpp"

namespace goodarray {
    template <std::size_t length>
    constexpr Primitivebitarray <length>::Primitivebitarray()
        : _org_array(0)
    {
        build();
    }

    template <std::size_t length>
    constexpr Primitivebitarray <length>::Primitivebitarray(const std::bitset <length> org_array)
        : _org_array(org_array)
    {
        build();
    }

    template <std::size_t length>
    constexpr Primitivebitarray <length>::Primitivebitarray(const int org_array)
        : _org_array(util::reverse(std::bitset <length>(org_array)))
    {
        build();
    }

    template <std::size_t length>
    constexpr void Primitivebitarray <length>::build()
    {
        // ブロックランク，スーパーブロックランクの作成．
        int superblock_now = 0;
        int lastrank       = 0;
        int subrank        = 0;
        // i番目のブロックの開始線でのrankを格納．
        for (size_t i = 0; i < length; i++) {
            if (i % superblock_size == 0) {
                lastrank = _superblock_rank1[i / superblock_size] = lastrank + subrank;
                subrank  = 0;
            }
            if (i % block_size == 0) {
                _block_rank1[i / block_size] = subrank;
            }
            subrank += _org_array[i];
        }

        // ルックアップテーブルの作成．
        for (size_t i = 0; i < table_size; i++) {
            _lookup_table[i] = std::bitset <table_elemsize>(i).count();
        }
    }

    template <std::size_t length>
    constexpr boost::optional <typename Primitivebitarray <length>::element_t> Primitivebitarray <length>::access(const position_t index) const
    {
        if (index < 0 || index >= length) {
            return boost::none;
        } else {
            return rank1(index) - rank(index - 1);
        }
    }

    template <std::size_t length>
    constexpr boost::optional <typename Primitivebitarray <length>::time_t> Primitivebitarray <length>::rank1(const position_t index) const
    {
        if (index < 0 || index >= length) {
            return boost::none;
        } else {
            // どのsuperblockに属しているか（0-origin）．
            const int superblock_index = index / superblock_size;
            // どのblockに属しているか（0-origin）．
            const int block_index = index / block_size;
            int       rest_index  = index % block_index;

            const auto rest = util::Slice <block_index *block_size, block_index *block_size + rest_index>::slice(_org_array);

            return _superblock_rank1[superblock_index] +
                   _block_rank1[block_index] +
                   _lookup_table[util::Slice < 0, table_elemsize > ::slice(rest).to_ulong()] +
                   _lookup_table[util::Slice < table_elemsize, table_elemsize * 2 > ::slice(rest).to_ulong()];
        }
    }

    template <std::size_t length>
    constexpr boost::optional <typename Primitivebitarray <length>::time_t> Primitivebitarray <length>::rank(const element_t a, const position_t index) const
    {
        if (index < 0 || index >= length) {
            return boost::none;
        } else if (a == 0) {
            return index + 1 - rank1(index);
        } else if (a == 1) {
            return rank1(index);
        } else {
            return boost::none;
        }
    }

    template <std::size_t length>
    constexpr boost::optional <typename Primitivebitarray <length>::position_t> Primitivebitarray <length>::select(const element_t a, const time_t order) const
    {
        if (a != 0 && a != 1) {
            return boost::none;
        } else if (order <= 0 || order > *(this->rank(a, length - 1))) {
            return boost::none;
        } else {
            int rest_order = order;

            // superblockの二分探索
            auto superblock_rank = [&](int a, int index) -> int {
                                       return (a == 0) ? (superblock_size * index + 1 - _superblock_rank1[index]) : (_superblock_rank1[index]);
                                   };
            int lower_superblock  = 0, upper_superblock = superblock_num;
            int center_superblock = 0;
            for (;; ) {
                center_superblock = (lower_superblock + upper_superblock) / 2;
                if (superblock_rank(a, center_superblock) >= order) {
                    // 前半に入っている場合
                    // orderと一致する場合もこちら
                    upper_superblock = center_superblock;
                } else {
                    // 後半に入っている場合
                    lower_superblock = center_superblock;
                }
                // 停止条件
                if (upper_superblock - lower_superblock <= 1) {
                    break;
                }
            }
            rest_order -= superblock_rank(a, upper_superblock);

            // blockの二分探索
            auto block_rank = [&](int a, int index) -> int {
                                  return (a == 0) ? (index + 1 - _block_rank1[index]) : (_block_rank1[index]);
                              };
            int lower_block  = lower_superblock * block_num_in_super, upper_block = std::min(upper_superblock * block_num_in_super, block_num);
            int center_block = 0;
            for (;; ) {
                center_block = (lower_block + upper_block) / 2;
                if (_block_rank1[center_block] >= rest_order) {
                    // 前半に入っている場合
                    // orderと一致する場合もこちら
                    upper_block = center_block;
                } else {
                    // 後半に入っている場合
                    lower_block = center_block;
                }
                // 停止条件
                if (upper_block - lower_block <= 1) {
                    break;
                }
            }
            rest_order -=  _block_rank1[upper_block];

            // 線形探索
            int lower = lower_block * block_size;
            int upper = std::min(upper_block * block_size, length);
            int index = 0;
            for (index = lower; index < upper; index++) {
                if (_org_array[index] == 1) {
                    rest_order--;
                    if (rest_order <= 0) {
                        break;
                    }
                }
            }

            return index;
        }
    }

    template <std::size_t length>
    constexpr boost::optional <typename Primitivebitarray <length>::position_t> Primitivebitarray <length>::select(const element_t a, const time_t order) const
    {
        if (order <= 0 || order > *(this->rank(a, length - 1))) {
            return boost::none;
        } else {
            // superblockの二分探索
            int lower  = 0, upper = superblock_num;
            int center = 0;
            for (;; ) {
                center = (lower + upper) / 2;
                if (_superblock_rank1[center] >= order) {
                    // 前半に入っている場合
                    // orderと一致する場合もこちら
                    upper = center;
                } else if (_superblock_rank1[center] < order) {
                    // 後半に入っている場合
                    lower = center;
                }
                // 停止条件
                if (upper - lower <= 1) {
                    break;
                }
            }
        }
    }

    void testPrimitivebitarray()
    {
        constexpr std::bitset <4> bs(9);
        constexpr int             n = bs[1];
    }
}

#endif
