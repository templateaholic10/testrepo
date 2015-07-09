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
    constexpr boost::optional <typename Primitivebitarray <length>::element_t> Primitivebitarray <length>::access(const position_t index) const
    {
        if (index < 0 || index >= length) {
            return boost::none;
        } else {
            return *rank1(index) - *rank1(index - 1);
        }
    }

    template <std::size_t length>
    constexpr boost::optional <typename Primitivebitarray <length>::time_t> Primitivebitarray <length>::rank1(const position_t index) const
    {
        if (index <= 0 || index > length) {
            return boost::none;
        } else {
            // どのsuperblockに属しているか（0-origin）．
            const int superblock_index = (index - 1) / superblock_size;

            // どのblockに属しているか（0-origin）．
            const int block_index = (index - 1) / block_size;

            // 線形探索
            int rest_rank = 0;
            for (size_t i = block_index * block_size; i < index; i++) {
                rest_rank += _org_array[i];
            }

            return _superblock_rank1[superblock_index].to_ulong() +
                   _block_rank1[block_index].to_ulong() + rest_rank;
        }
    }

    template <std::size_t length>
    constexpr boost::optional <typename Primitivebitarray <length>::time_t> Primitivebitarray <length>::rank(const element_t a, const position_t index) const
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
    constexpr boost::optional <typename Primitivebitarray <length>::position_t> Primitivebitarray <length>::select1(const time_t order) const
    {
        if (order <= 0) {
            return boost::none;
        } else {
            int rest_order = order;

            // superblockの二分探索
            int lower_superblock  = 0, upper_superblock = superblock_num;
            int center_superblock = 0;
            for (;; ) {
                center_superblock = (lower_superblock + upper_superblock) / 2;
                if (_superblock_rank1[center_superblock].to_ulong() >= order) {
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
            rest_order -= _superblock_rank1[upper_superblock].to_ulong();

            // blockの二分探索
            int lower_block = lower_superblock * block_num_in_super;
            int tmp         = block_num;
            int upper_block = std::min(upper_superblock * block_num_in_super, tmp);

            int center_block = 0;
            for (;; ) {
                center_block = (lower_block + upper_block) / 2;
                if (_block_rank1[center_block].to_ulong() >= rest_order) {
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
            rest_order -=  _block_rank1[upper_block].to_ulong();

            // 線形探索
            int  lower     = lower_block * block_size;
            int  upper     = std::min(upper_block * block_size, length);
            int  index     = 0;
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
    constexpr boost::optional <typename Primitivebitarray <length>::position_t> Primitivebitarray <length>::select0(const time_t order) const
    {
        if (order <= 0) {
            return boost::none;
        } else {
            int rest_order = order;

            // superblockの二分探索
            int lower_superblock  = 0, upper_superblock = superblock_num;
            int center_superblock = 0;
            for (;; ) {
                center_superblock = (lower_superblock + upper_superblock) / 2;
                if (superblock_size * center_superblock - _superblock_rank1[center_superblock].to_ulong() >= order) {
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
            rest_order -= superblock_size * upper_superblock - _superblock_rank1[center_superblock].to_ulong();

            // blockの二分探索
            int lower_block           = lower_superblock * block_num_in_super;
            int tmp                   = block_num;
            int upper_block           = std::min(upper_superblock * block_num_in_super, tmp);
            int center_block          = 0;
            int center_block_relative = 0;
            for (;; ) {
                center_block          = (lower_block + upper_block) / 2;
                center_block_relative = center_block - block_num_in_super * upper_superblock;
                if (block_size * center_block_relative - _block_rank1[center_block].to_ulong() >= rest_order) {
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
            int upper_block_relative = upper_block - block_num_in_super * upper_superblock;
            rest_order -=  block_size * upper_block_relative - _block_rank1[upper_block].to_ulong();

            // 線形探索
            int  lower     = lower_block * block_size;
            int  upper     = std::min(upper_block * block_size, length);
            int  index     = 0;
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
    constexpr boost::optional <typename Primitivebitarray <length>::position_t> Primitivebitarray <length>::select(const element_t a, const time_t order) const
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
    constexpr size_t Primitivebitarray <length>::size() const
    {
        return length;
    }

    template <std::size_t length>
    std::ostream&operator<<(std::ostream &os, const Primitivebitarray <length> &pb)
    {
        os << util::reverse(pb._org_array);

        return os;
    }

    void testPrimitivebitarray()
    {
        Primitivebitarray <9> pb = Primitivebitarray <9>(44);
        std::cout << pb << std::endl;
        std::cout << "access(4) = " << *pb.access(4) << std::endl;
        std::cout << "rank0(4) = " << *pb.rank(0, 4) << std::endl;
        std::cout << "select1(2) = " << *pb.select(1, 2) << std::endl;
    }
}

#endif
