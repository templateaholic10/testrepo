#ifndef DETAIL_UNIONARRAY
#define DETAIL_UNIONARRAY

#include "../unionarray.hpp"

namespace unionarray {
    template <std::size_t length>
    constexpr Unionbitarray <length>::Unionbitarray()
        : _org_array(util::constexpr_array <block_t, block_num>()), _superblock_rank1(util::constexpr_array<superblock_rank_t, superblock_num>()), _block_rank1(util::constexpr_array<block_rank_t, block_num>())
    {
        build();
    }

    template <std::size_t length>
    constexpr Unionbitarray <length>::Unionbitarray(const char org_array[length])
        : _org_array(util::c_str_to_bin <block_t, block_num>(org_array, length)), _superblock_rank1(util::constexpr_array<superblock_rank_t, superblock_num>()), _block_rank1(util::constexpr_array<block_rank_t, block_num>())
    {
        build();
    }

    template <std::size_t length>
    constexpr Unionbitarray <length>::Unionbitarray(const unsigned long org_array)
        : _org_array(util::numeric_to_bin<block_t, block_num>(org_array)), _superblock_rank1(util::constexpr_array<superblock_rank_t, superblock_num>()), _block_rank1(util::constexpr_array<block_rank_t, block_num>())
    {
        build();
    }

    template <std::size_t length>
    constexpr void Unionbitarray <length>::build()
    {
        // ブロックランク，スーパーブロックランクの作成．
        time_t lastrank       = 0;
        time_t subrank        = 0;
        // i番目のブロックの開始線でのrankを格納．
        for (size_t i = 0; i < length; i++) {
            if (i % superblock_size == 0) {
                lastrank                               = lastrank + subrank;
                _superblock_rank1[i / superblock_size] = lastrank;
                subrank                                = 0;
            }
            if (i % block_size == 0) {
                _block_rank1[i / block_size] = subrank;
            }
            subrank += _org_array[i];
        }
    }

    template <std::size_t length>
    constexpr boost::optional <typename Unionbitarray <length>::element_t> Unionbitarray <length>::access(const position_t index) const
    {
        if (index < 0 || index >= length) {
            return boost::none;
        } else {
            return *rank1(index) - *rank1(index - 1);
        }
    }

    template <std::size_t length>
    constexpr boost::optional <typename Unionbitarray <length>::time_t> Unionbitarray <length>::rank1(const position_t index) const
    {
        if (index <= 0 || index > length) {
            return boost::none;
        } else {
            // どのsuperblockに属しているか（0-origin）．
            const position_t superblock_index = (index - 1) / superblock_size;

            // どのblockに属しているか（0-origin）．
            const position_t block_index = (index - 1) / block_size;

            // 線形探索
            time_t rest_rank = 0;
            for (position_t i = block_index * block_size; i < index; i++) {
                rest_rank += _org_array[i];
            }

            return _superblock_rank1[superblock_index] +
                   _block_rank1[block_index] + rest_rank;
        }
    }

    template <std::size_t length>
    constexpr boost::optional <typename Unionbitarray <length>::time_t> Unionbitarray <length>::rank(const element_t a, const position_t index) const
    {
        if (index <= 0 || index > length) {
            return boost::none;
        } else if (a == 0) {
            return index - *rank1(index);
        } else if (a == 1) {
            return *rank1(index);
        } else {
            return boost::none;
        }
    }

    template <std::size_t length>
    constexpr boost::optional <typename Unionbitarray <length>::position_t> Unionbitarray <length>::select1(const time_t order) const
    {
        if (order <= 0) {
            return boost::none;
        } else {
            time_t rest_order = order;

            // superblockの二分探索
            position_t lower_superblock  = 0, upper_superblock = superblock_num;
            position_t center_superblock = 0;
            for (;; ) {
                center_superblock = (lower_superblock + upper_superblock) / 2;
                if (_superblock_rank1[center_superblock] >= order) {
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
            rest_order -= _superblock_rank1[upper_superblock];

            // blockの二分探索
            position_t lower_block = lower_superblock * block_num_in_super;
            // min
            position_t upper_block = (upper_superblock * block_num_in_super < block_num) ? (upper_superblock * block_num_in_super) : block_num;

            position_t center_block = 0;
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
            position_t  lower     = lower_block * block_size;
            // min
            position_t  upper     = (upper_block * block_size < length) ? (upper_block * block_size) : length;
            position_t  index     = 0;
            bool finded_tf = false;
            for (index = lower; index < upper; index++) {
                if (_org_array[index] == 1) {
                    rest_order--;
                    if (rest_order <= 0) {
                        finded_tf = true;
                        break;
                    }
                }
            }
            if (finded_tf) {
                return index + 1;
            } else {
                // インデックスを抜けてしまった．
                return boost::none;
            }
        }
    }

    template <std::size_t length>
    constexpr boost::optional <typename Unionbitarray <length>::position_t> Unionbitarray <length>::select0(const time_t order) const
    {
        if (order <= 0) {
            return boost::none;
        } else {
            time_t rest_order = order;

            // superblockの二分探索
            position_t lower_superblock  = 0, upper_superblock = superblock_num;
            position_t center_superblock = 0;
            for (;; ) {
                center_superblock = (lower_superblock + upper_superblock) / 2;
                if (superblock_size * center_superblock - _superblock_rank1[center_superblock] >= order) {
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
            rest_order -= superblock_size * upper_superblock - _superblock_rank1[center_superblock];

            // blockの二分探索
            position_t lower_block           = lower_superblock * block_num_in_super;
            // min
            position_t upper_block = (upper_superblock * block_num_in_super < block_num) ? (upper_superblock * block_num_in_super) : block_num;
            position_t center_block          = 0;
            position_t center_block_relative = 0;
            for (;; ) {
                center_block          = (lower_block + upper_block) / 2;
                center_block_relative = center_block - block_num_in_super * upper_superblock;
                if (block_size * center_block_relative - _block_rank1[center_block] >= rest_order) {
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
            position_t upper_block_relative = upper_block - block_num_in_super * upper_superblock;
            rest_order -=  block_size * upper_block_relative - _block_rank1[upper_block];

            // 線形探索
            position_t  lower     = lower_block * block_size;
            // min
            position_t  upper     = (upper_block * block_size < length) ? (upper_block * block_size) : length;
            position_t  index     = 0;
            bool finded_tf = false;
            for (index = lower; index < upper; index++) {
                if (_org_array[index] == 0) {
                    rest_order--;
                    if (rest_order <= 0) {
                        finded_tf = true;
                        break;
                    }
                }
            }
            if (finded_tf) {
                return index + 1;
            } else {
                // インデックスを抜けてしまった．
                return boost::none;
            }
        }
    }

    template <std::size_t length>
    constexpr boost::optional <typename Unionbitarray <length>::position_t> Unionbitarray <length>::select(const element_t a, const time_t order) const
    {
        if (a != 0 && a != 1) {
            return boost::none;
        } else if (a == 0) {
            return select0(order);
        } else {
            return select1(order);
        }
    }

    template <std::size_t length>
    constexpr size_t Unionbitarray <length>::size() const
    {
        return length;
    }

    template <std::size_t length>
    std::ostream&operator<<(std::ostream &os, const Unionbitarray <length> &pb)
    {
        os << util::reverse(pb._org_array);

        return os;
    }

    void testUnionbitarray()
    {
        // 文字列
        constexpr const char *str   = "0000000000000000000000000111111111111111111111111111111111111111";
        std::cout << "c_str_to_bin" << "(" << str << ")" << std::endl;
        constexpr auto        a_str = util::c_str_to_bin <unsigned char, 8>(str, 64);
        for (int i = 0; i < a_str.size(); i++) {
            std::cout << static_cast <unsigned long>(a_str[i]) << " ";
        }
        std::cout << std::endl;

        // 数値
        constexpr const unsigned long num   = 549755813887;
        std::cout << "numeric_to_bin" << "(" << num << ")" << std::endl;
        std::cout << "numeric_to_bin" << "(" << std::bitset<64>(num).to_string() << ")" << std::endl;
        constexpr auto                a_num = util::numeric_to_bin <unsigned char, 8>(num);
        for (int i = 0; i < a_num.size(); i++) {
            std::cout << static_cast <unsigned long>(a_num[i]) << " ";
        }
        std::cout << std::endl;

        // Unionbitarray
        constexpr Unionbitarray<64> ub = Unionbitarray<64>();
    }
}

#endif
