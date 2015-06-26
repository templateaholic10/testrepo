#ifndef DETAIL_UNIONARRAY
#define DETAIL_UNIONARRAY

#include "../unionarray.hpp"

namespace unionarray {
    template <std::size_t length>
    constexpr Unionbitarray <length>::Unionbitarray()
        : _org_array(sprout::array <halfblock_t, halfblock_num>()), _superblock_rank1(sprout::array <superblock_rank_t, superblock_num>()), _block_rank1(sprout::array <block_rank_t, block_num>()), _lookuptable(
            sprout::array <lookuptable_elem_t, lookuptable_size>())
    {
        build();
    }

    template <std::size_t length>
    constexpr Unionbitarray <length>::Unionbitarray(const char org_array[length])
        : _org_array(util::c_str_to_bin <halfblock_t, halfblock_num>(org_array, length)), _superblock_rank1(sprout::array <superblock_rank_t, superblock_num>()), _block_rank1(sprout::array <block_rank_t, block_num>()), _lookuptable(
            sprout::array <lookuptable_elem_t, lookuptable_size>())
    {
        build();
    }

    template <std::size_t length>
    constexpr Unionbitarray <length>::Unionbitarray(const unsigned long org_array)
        : _org_array(util::numeric_to_bin <halfblock_t, halfblock_num>(org_array)), _superblock_rank1(sprout::array <superblock_rank_t, superblock_num>()), _block_rank1(sprout::array <block_rank_t, block_num>()), _lookuptable(
            sprout::array <lookuptable_elem_t, lookuptable_size>())
    {
        build();
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

    // template <std::size_t length>
    // constexpr boost::optional <typename Unionbitarray <length>::element_t> Unionbitarray <length>::access(const position_t index) const
    // {
    //     if (index < 0 || index >= length) {
    //         return boost::none;
    //     } else {
    //         return *rank1(index) - *rank1(index - 1);
    //     }
    // }

    template <std::size_t length>
    constexpr unsigned long Unionbitarray <length>::rank1(const position_t index) const
    {
        if (index <= 0 || index > length) {
            return invalid_value();
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

    // template <std::size_t length>
    // constexpr boost::optional <typename Unionbitarray <length>::time_t> Unionbitarray <length>::rank(const element_t a, const position_t index) const
    // {
    //     if (index <= 0 || index > length) {
    //         return boost::none;
    //     } else if (a == 0) {
    //         return index - *rank1(index);
    //     } else if (a == 1) {
    //         return *rank1(index);
    //     } else {
    //         return boost::none;
    //     }
    // }
    //
    // template <std::size_t length>
    // constexpr boost::optional <typename Unionbitarray <length>::position_t> Unionbitarray <length>::select1(const time_t order) const
    // {
    //     if (order <= 0) {
    //         return boost::none;
    //     } else {
    //         time_t rest_order = order;
    //
    //         // superblockの二分探索
    //         position_t lower_superblock  = 0, upper_superblock = superblock_num;
    //         position_t center_superblock = 0;
    //         for (;; ) {
    //             center_superblock = (lower_superblock + upper_superblock) / 2;
    //             if (_superblock_rank1[center_superblock] >= order) {
    //                 // 前半に入っている場合
    //                 // orderと一致する場合もこちら
    //                 upper_superblock = center_superblock;
    //             } else {
    //                 // 後半に入っている場合
    //                 lower_superblock = center_superblock;
    //             }
    //             // 停止条件
    //             if (upper_superblock - lower_superblock <= 1) {
    //                 break;
    //             }
    //         }
    //         rest_order -= _superblock_rank1[upper_superblock];
    //
    //         // blockの二分探索
    //         position_t lower_block = lower_superblock * block_num_in_super;
    //         // min
    //         position_t upper_block = (upper_superblock * block_num_in_super < block_num) ? (upper_superblock * block_num_in_super) : block_num;
    //
    //         position_t center_block = 0;
    //         for (;; ) {
    //             center_block = (lower_block + upper_block) / 2;
    //             if (_block_rank1[center_block] >= rest_order) {
    //                 // 前半に入っている場合
    //                 // orderと一致する場合もこちら
    //                 upper_block = center_block;
    //             } else {
    //                 // 後半に入っている場合
    //                 lower_block = center_block;
    //             }
    //             // 停止条件
    //             if (upper_block - lower_block <= 1) {
    //                 break;
    //             }
    //         }
    //         rest_order -=  _block_rank1[upper_block];
    //
    //         // 線形探索
    //         position_t  lower     = lower_block * block_size;
    //         // min
    //         position_t  upper     = (upper_block * block_size < length) ? (upper_block * block_size) : length;
    //         position_t  index     = 0;
    //         bool finded_tf = false;
    //         for (index = lower; index < upper; index++) {
    //             if (_org_array[index] == 1) {
    //                 rest_order--;
    //                 if (rest_order <= 0) {
    //                     finded_tf = true;
    //                     break;
    //                 }
    //             }
    //         }
    //         if (finded_tf) {
    //             return index + 1;
    //         } else {
    //             // インデックスを抜けてしまった．
    //             return boost::none;
    //         }
    //     }
    // }
    //
    // template <std::size_t length>
    // constexpr boost::optional <typename Unionbitarray <length>::position_t> Unionbitarray <length>::select0(const time_t order) const
    // {
    //     if (order <= 0) {
    //         return boost::none;
    //     } else {
    //         time_t rest_order = order;
    //
    //         // superblockの二分探索
    //         position_t lower_superblock  = 0, upper_superblock = superblock_num;
    //         position_t center_superblock = 0;
    //         for (;; ) {
    //             center_superblock = (lower_superblock + upper_superblock) / 2;
    //             if (superblock_size * center_superblock - _superblock_rank1[center_superblock] >= order) {
    //                 // 前半に入っている場合
    //                 // orderと一致する場合もこちら
    //                 upper_superblock = center_superblock;
    //             } else {
    //                 // 後半に入っている場合
    //                 lower_superblock = center_superblock;
    //             }
    //             // 停止条件
    //             if (upper_superblock - lower_superblock <= 1) {
    //                 break;
    //             }
    //         }
    //         rest_order -= superblock_size * upper_superblock - _superblock_rank1[center_superblock];
    //
    //         // blockの二分探索
    //         position_t lower_block           = lower_superblock * block_num_in_super;
    //         // min
    //         position_t upper_block = (upper_superblock * block_num_in_super < block_num) ? (upper_superblock * block_num_in_super) : block_num;
    //         position_t center_block          = 0;
    //         position_t center_block_relative = 0;
    //         for (;; ) {
    //             center_block          = (lower_block + upper_block) / 2;
    //             center_block_relative = center_block - block_num_in_super * upper_superblock;
    //             if (block_size * center_block_relative - _block_rank1[center_block] >= rest_order) {
    //                 // 前半に入っている場合
    //                 // orderと一致する場合もこちら
    //                 upper_block = center_block;
    //             } else {
    //                 // 後半に入っている場合
    //                 lower_block = center_block;
    //             }
    //             // 停止条件
    //             if (upper_block - lower_block <= 1) {
    //                 break;
    //             }
    //         }
    //         position_t upper_block_relative = upper_block - block_num_in_super * upper_superblock;
    //         rest_order -=  block_size * upper_block_relative - _block_rank1[upper_block];
    //
    //         // 線形探索
    //         position_t  lower     = lower_block * block_size;
    //         // min
    //         position_t  upper     = (upper_block * block_size < length) ? (upper_block * block_size) : length;
    //         position_t  index     = 0;
    //         bool finded_tf = false;
    //         for (index = lower; index < upper; index++) {
    //             if (_org_array[index] == 0) {
    //                 rest_order--;
    //                 if (rest_order <= 0) {
    //                     finded_tf = true;
    //                     break;
    //                 }
    //             }
    //         }
    //         if (finded_tf) {
    //             return index + 1;
    //         } else {
    //             // インデックスを抜けてしまった．
    //             return boost::none;
    //         }
    //     }
    // }
    //
    // template <std::size_t length>
    // constexpr boost::optional <typename Unionbitarray <length>::position_t> Unionbitarray <length>::select(const element_t a, const time_t order) const
    // {
    //     if (a != 0 && a != 1) {
    //         return boost::none;
    //     } else if (a == 0) {
    //         return select0(order);
    //     } else {
    //         return select1(order);
    //     }
    // }

    template <std::size_t length>
    constexpr size_t Unionbitarray <length>::size() const
    {
        return length;
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
        constexpr auto a_str = util::c_str_to_bin <unsigned char, 8>(str, 64);
        for (int i = 0; i < a_str.size(); i++) {
            std::cout << static_cast <unsigned long>(a_str[i]) << " ";
        }
        std::cout << std::endl;

        // 数値
        constexpr const unsigned long num = 549755813887;
        std::cout << "numeric_to_bin" << "(" << num << ")" << std::endl;
        std::cout << "numeric_to_bin" << "(" << std::bitset <64>(num).to_string() << ")" << std::endl;
        constexpr auto a_num = util::numeric_to_bin <unsigned char, 8>(num);
        for (int i = 0; i < a_num.size(); i++) {
            std::cout << static_cast <unsigned long>(a_num[i]) << " ";
        }
        std::cout << std::endl;

        // Unionbitarray
        static_assert(std::is_same <Unionbitarray <257>::block_t, unsigned short>::value, "not unsigned short");
        static_assert(std::is_same <Unionbitarray <257>::halfblock_t, unsigned char>::value, "not unsigned char");
        static_assert(Unionbitarray <257>::block_num == 17, "block_num error");
        static_assert(Unionbitarray <257>::halfblock_num == 34, "halfblock_num error");

        constexpr size_t length = 16;
        constexpr Unionbitarray <length> ub   = Unionbitarray <length>(29260);
        constexpr size_t             size = ub.size();
        std::cout << ub.str() << std::endl;
        std::cout << ub.superblock_rank() << std::endl;
        std::cout << ub.block_rank() << std::endl;
        std::cout << ub.lookuptable() << std::endl;
        for (size_t i = 1; i <= length; i++) {
            std::cout << "rank1(" << i << ") = " << ub.rank1(i) << std::endl;
        }

        constexpr size_t length2 = 40;
        constexpr Unionbitarray <length2> ub2   = Unionbitarray <length2>("1110010010011001000101010100000111110111");
        constexpr size_t             size2 = ub2.size();
        std::cout << ub2.str() << std::endl;
        // std::cout << ub2.superblock_rank() << std::endl;
        // std::cout << ub2.block_rank() << std::endl;
        // std::cout << ub2.lookuptable() << std::endl;
        for (size_t i = 1; i <= length2; i++) {
            std::cout << "rank1(" << i << ") = " << ub2.rank1(i) << std::endl;
        }
    }
}

#endif
