#ifndef DETAIL_UNIONBITARRAY
#define DETAIL_UNIONBITARRAY

#include "../unionbitarray.hpp"

namespace unionbitarray {
    template <std::size_t length>
    constexpr Unionbitarray <length>::Unionbitarray()
        : _org_array(sprout::array <halfblock_t, halfblock_num>()), _superblock_rank1(sprout::array <superblock_rank_t, superblock_num>()), _block_rank1(sprout::array <block_rank_t, block_num>()), _lookuptable(
            sprout::array <lookuptable_elem_t, lookuptable_size>())
    {
        build();
    }

    template <std::size_t length>
    constexpr Unionbitarray <length>::Unionbitarray(const char org_array[length])
        : _org_array(util::c_str_to_block_bitseq <halfblock_t, halfblock_num>(org_array, length)), _superblock_rank1(sprout::array <superblock_rank_t, superblock_num>()), _block_rank1(sprout::array <block_rank_t, block_num>()),
        _lookuptable(
            sprout::array <lookuptable_elem_t, lookuptable_size>())
    {
        build();
    }

    template <std::size_t length>
    constexpr Unionbitarray <length>::Unionbitarray(char org_array[length])
        : _org_array(util::c_str_to_block_bitseq <halfblock_t, halfblock_num>(org_array, length)), _superblock_rank1(sprout::array <superblock_rank_t, superblock_num>()), _block_rank1(sprout::array <block_rank_t, block_num>()),
        _lookuptable(
            sprout::array <lookuptable_elem_t, lookuptable_size>())
    {
        build();
    }

    template <std::size_t length>
    constexpr Unionbitarray <length>::Unionbitarray(const unsigned long org_array)
        : _org_array(util::numeric_to_block_bitseq <halfblock_t, halfblock_num>(org_array)), _superblock_rank1(sprout::array <superblock_rank_t, superblock_num>()), _block_rank1(sprout::array <block_rank_t, block_num>()), _lookuptable(
            sprout::array <lookuptable_elem_t, lookuptable_size>())
    {
        build();
    }

    template <std::size_t length>
    constexpr Unionbitarray <length>::Unionbitarray(const sprout::array <unsigned char, length> &org_array)
        : _org_array(util::array_to_block_bitseq <halfblock_t, halfblock_num>(org_array)), _superblock_rank1(sprout::array <superblock_rank_t, superblock_num>()), _block_rank1(sprout::array <block_rank_t, block_num>()), _lookuptable(
            sprout::array <lookuptable_elem_t, lookuptable_size>())
    {
        build();
    }

    template <std::size_t length>
    constexpr Unionbitarray <length>::Unionbitarray(const sprout::bitset <length> &org_array)
        : _org_array(util::bitset_to_block_bitseq <halfblock_t, halfblock_num>(org_array)), _superblock_rank1(sprout::array <superblock_rank_t, superblock_num>()), _block_rank1(sprout::array <block_rank_t, block_num>()), _lookuptable(
            sprout::array <lookuptable_elem_t, lookuptable_size>())
    {
        build();
    }

    template <std::size_t length>
    constexpr void Unionbitarray <length>::set(const char org_array[length], bool rebuild)
    {
        _org_array = util::c_str_to_block_bitseq <halfblock_t, halfblock_num>(org_array);
        if (rebuild) {
            build();
        }
    }

    template <std::size_t length>
    constexpr void Unionbitarray <length>::set(const unsigned long org_array, bool rebuild)
    {
        _org_array = util::numeric_to_block_bitseq <halfblock_t, halfblock_num>(org_array);
        if (rebuild) {
            build();
        }
    }

    template <std::size_t length>
    constexpr void Unionbitarray <length>::set(const sprout::array <unsigned char, length> &org_array, bool rebuild)
    {
        _org_array = util::array_to_block_bitseq <halfblock_t, halfblock_num>(org_array);
        if (rebuild) {
            build();
        }
    }

    template <std::size_t length>
    constexpr void Unionbitarray <length>::set(const sprout::bitset <length> &org_array, bool rebuild)
    {
        _org_array = util::bitset_to_block_bitseq <halfblock_t, halfblock_num>(org_array);
        if (rebuild) {
            build();
        }
    }

    template <std::size_t length>
    constexpr void Unionbitarray <length>::build()
    {
        // ブロックランク，スーパーブロックランクの作成．
        time_t lastrank = 0;
        time_t subrank  = 0;
        // i番目のブロックの開始線でのrankを格納．
        for (size_t i = 0; i < block_num; i++) {
            _block_rank1[i] = subrank;
            if (i % block_num_in_super == 0) {
                lastrank                                 += subrank;
                _superblock_rank1[i / block_num_in_super] = lastrank;
                subrank                                   = 0;
            }
            if (std::is_same <block_t, halfblock_t>::value) {
                subrank += sprout::bitset <block_size>(_org_array[i]).count();
            } else {
                subrank += sprout::bitset <block_size>(_org_array[2 * i]).count() + sprout::bitset <block_size>(_org_array[2 * i + 1]).count();
            }
        }

        // ルックアップテーブルの作成．
        for (size_t i = 0; i < lookuptable_size; i++) {
            _lookuptable[i] = sprout::bitset <util::lg(lookuptable_size)>(i).count();
        }
    }

    template <std::size_t length>
    constexpr unsigned long Unionbitarray <length>::access(const position_t index) const
    {
        return rank1(index) - rank1(index - 1);
    }

    template <std::size_t length>
    constexpr unsigned long Unionbitarray <length>::rank1(const position_t index) const
    {
        if (index <= 0) {
            return 0;
        } else if (index > length) {
            return rank1(length);
        } else {
            // どのsuperblockに属しているか（0-origin）．
            const position_t superblock_index = (index - 1) / superblock_size;

            // どのblockに属しているか（0-origin）．
            const position_t block_index = (index - 1) / block_size;

            position_t rest_index = index - block_index * block_size;
            // indexは1-originであった．
            assert(rest_index <= block_size);

            time_t rest_rank = 0;
            if (std::is_same <block_t, halfblock_t>::value) {
                halfblock_t rest_halfblock = _org_array[block_index];
                rest_halfblock >>= block_size - rest_index;
                rest_rank       += _lookuptable[rest_halfblock];
            } else {
                if (rest_index < halfblock_size) {
                    // (halfblock_size - rest_index)だけシフト後の前半のみ．
                    halfblock_t rest_halfblock = _org_array[2 * block_index];
                    rest_halfblock >>= halfblock_size - rest_index;
                    rest_rank       += _lookuptable[rest_halfblock];
                } else {
                    // シフト前の前半と(block_size - rest_index)だけシフト後の後半のrankの和．
                    // 前半
                    rest_rank += _lookuptable[_org_array[2 * block_index]];
                    // 後半
                    halfblock_t rest_halfblock = _org_array[2 * block_index + 1];
                    rest_halfblock >>= block_size - rest_index;
                    rest_rank       += _lookuptable[rest_halfblock];
                }
            }

            // std::cout << static_cast<unsigned long>(_superblock_rank1[superblock_index]) << " " << static_cast<unsigned long>(_block_rank1[block_index]) << " " << static_cast<unsigned long>(rest_rank) << std::endl;

            return _superblock_rank1[superblock_index] +
                   _block_rank1[block_index] + rest_rank;
        }
    }

    template <std::size_t length>
    constexpr unsigned long Unionbitarray <length>::rank(const element_t a, const position_t index) const
    {
        if (a == 0) {
            return index - rank1(index);
        } else if (a == 1) {
            return rank1(index);
        } else {
            return invalid_value();
        }
    }

    template <std::size_t length>
    constexpr unsigned long Unionbitarray <length>::select1(const time_t order) const
    {
        if (order <= 0) {
            return 0;
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
            rest_order -= _superblock_rank1[lower_superblock];

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
            rest_order -=  _block_rank1[lower_block];

            // 線形探索
            util::container_t <block_size> index_in_block = 0;
            if (std::is_same <block_t, halfblock_t>::value) {
                sprout::bitset <halfblock_size> halfblock = _org_array[lower_block];
                index_in_block += util::select(1, halfblock, rest_order);
            } else {
                sprout::bitset <block_size> block = util::power(2, halfblock_size) * _org_array[2 * lower_block] + _org_array[2 * lower_block + 1];
                index_in_block += util::select(1, block, rest_order);
            }

            return block_size * lower_block + index_in_block;
        }
    }

    template <std::size_t length>
    constexpr unsigned long Unionbitarray <length>::select0(const time_t order) const
    {
        if (order <= 0) {
            return 0;
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
            rest_order -= superblock_size * lower_superblock - _superblock_rank1[lower_superblock];
            // std::cout << "lower_superblock: " << static_cast<unsigned long>(lower_superblock) << std::endl;

            // blockの二分探索
            position_t lower_block = lower_superblock * block_num_in_super;
            // min
            position_t upper_block           = (upper_superblock * block_num_in_super < block_num) ? (upper_superblock * block_num_in_super) : block_num;
            position_t center_block          = 0;
            position_t center_block_relative = 0;
            for (;; ) {
                center_block          = (lower_block + upper_block) / 2;
                center_block_relative = center_block - block_num_in_super * lower_superblock;
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
            position_t lower_block_relative = lower_block - block_num_in_super * lower_superblock;
            rest_order -=  block_size * lower_block_relative - _block_rank1[lower_block];
            // std::cout << "lower_block: " << static_cast<unsigned long>(lower_block) << std::endl;

            // 線形探索
            util::container_t <block_size> index_in_block = 0;
            if (std::is_same <block_t, halfblock_t>::value) {
                sprout::bitset <halfblock_size> halfblock = _org_array[lower_block];
                index_in_block += util::select(0, halfblock, rest_order);
            } else {
                sprout::bitset <block_size> block = util::power(2, halfblock_size) * _org_array[2 * lower_block] + _org_array[2 * lower_block + 1];
                index_in_block += util::select(0, block, rest_order);
            }

            return block_size * lower_block + index_in_block;
        }
    }

    template <std::size_t length>
    constexpr unsigned long Unionbitarray <length>::select(const element_t a, const time_t order) const
    {
        if (a == 0) {
            return select0(order);
        } else if (a == 1) {
            return select1(order);
        } else {
            return invalid_value();
        }
    }

    template <std::size_t length>
    constexpr size_t Unionbitarray <length>::size() const
    {
        return length;
    }

    template <std::size_t length>
    constexpr size_t Unionbitarray <length>::alphabet_size() const
    {
        return 2;
    }

    template <std::size_t length>
    std::string Unionbitarray <length>::to_string() const
    {
        std::string result = "";
        for (size_t i = 0; i < block_num; i++) {
            result += std::bitset <block_size>(_org_array[i]).to_string();
        }

        return result;
    }

    template <std::size_t length>
    constexpr unsigned long Unionbitarray <length>::invalid_value() const
    {
        return std::numeric_limits <length_t>::max();
    }

    template <std::size_t length>
    std::string Unionbitarray <length>::str() const
    {
        std::string result = "";
        result += to_string() + '\n';
        // 全体のビット長
        result += "length: " + std::to_string(length) + '\n';
        // superblockについて
        result += "superblock_size: " + std::to_string(superblock_size) + '\n';
        result += "superblock_num: " + std::to_string(superblock_num) + '\n';
        result += "superblock_rank_size: " + std::to_string(8 * sizeof(superblock_rank_t)) + '\n';
        // blockについて
        result += "block_size: " + std::to_string(block_size) + '\n';
        result += "block_num_in_super: " + std::to_string(block_num_in_super) + '\n';
        result += "block_num: " + std::to_string(block_num) + '\n';
        result += "block_rank_size: " + std::to_string(8 * sizeof(block_rank_t)) + '\n';
        // halfblockについて
        result += "halfblock_size: " + std::to_string(halfblock_size) + '\n';
        result += "halfblock_num: " + std::to_string(halfblock_num) + '\n';
        // loouptableについて
        result += "lookuptable_size: " + std::to_string(lookuptable_size) + '\n';
        result += "lookuptable_elem_size: " + std::to_string(8 * sizeof(lookuptable_elem_t)) + '\n';

        return result;
    }

    template <std::size_t length>
    std::string Unionbitarray <length>::superblock_rank() const
    {
        std::string result = "";
        for (auto rank : _superblock_rank1) {
            result += std::to_string(rank) + " ";
        }

        return result;
    }

    template <std::size_t length>
    std::string Unionbitarray <length>::block_rank() const
    {
        std::string result = "";
        for (auto rank : _block_rank1) {
            result += std::to_string(rank) + " ";
        }

        return result;
    }

    template <std::size_t length>
    std::string Unionbitarray <length>::lookuptable() const
    {
        std::string result = "";
        for (size_t i = 0; i < lookuptable_size; i++) {
            result += std::to_string(i) + ": " + std::to_string(_lookuptable[i]) + '\n';
        }

        return result;
    }

    template <std::size_t length>
    std::ostream&operator<<(std::ostream &os, const Unionbitarray <length> &pb)
    {
        os << pb.to_string();

        return os;
    }

    void testUnionbitarray()
    {
        // 文字列
        constexpr const char *str = "0000000000000000000000000111111111111111111111111111111111111111";
        std::cout << "c_str_to_bin" << "(" << str << ")" << std::endl;
        constexpr auto a_str = util::c_str_to_block_bitseq <unsigned char, 8>(str, 64);
        for (int i = 0; i < a_str.size(); i++) {
            std::cout << static_cast <unsigned long>(a_str[i]) << " ";
        }
        std::cout << std::endl;

        // 数値
        constexpr const unsigned long num = 549755813887;
        std::cout << "numeric_to_bin" << "(" << num << ")" << std::endl;
        std::cout << "numeric_to_bin" << "(" << std::bitset <64>(num).to_string() << ")" << std::endl;
        constexpr auto a_num = util::numeric_to_block_bitseq <unsigned char, 8>(num);
        for (int i = 0; i < a_num.size(); i++) {
            std::cout << static_cast <unsigned long>(a_num[i]) << " ";
        }
        std::cout << std::endl;

        // Unionbitarray
        constexpr size_t                  length2 = 40;
        constexpr const char *str2 = "1110010010011001000101010100000111110111";
        constexpr Unionbitarray <length2> ub2     = Unionbitarray <length2>(str2);
        constexpr size_t size2 = ub2.size();
        std::cout << ub2.str() << std::endl;
        // std::cout << ub2.superblock_rank() << std::endl;
        // std::cout << ub2.block_rank() << std::endl;
        // std::cout << ub2.lookuptable() << std::endl;
        for (size_t i = 1; i <= length2; i++) {
            std::cout << "rank0(" << i << ") = " << ub2.rank(0, i) << std::endl;
        }
        for (size_t i = 1; i <= length2; i++) {
            std::cout << "rank1(" << i << ") = " << ub2.rank(1, i) << std::endl;
        }
        for (size_t i = 1; i <= length2; i++) {
            std::cout << "access(" << i << ") = " << ub2.access(i) << std::endl;
        }
        for (size_t i = 1; i <= length2; i++) {
            std::cout << "select0(" << i << ") = " << ub2.select(0, i) << std::endl;
        }
        for (size_t i = 1; i <= length2; i++) {
            std::cout << "select1(" << i << ") = " << ub2.select(1, i) << std::endl;
        }
        constexpr size_t sele = ub2.select(1, 4);
        constexpr auto line = util::linspace<double, 1000>(0., 0.01);
        std::for_each(line.begin(), line.end(), [](double a){std::cout << a << " ";});
        std::cout << std::endl;
    }
}

#endif
