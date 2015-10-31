#ifndef UTIL_BIT
#define UTIL_BIT

#include <iostream>
#include <array>
#include <bitset>
#include <string>
#include <sprout/array.hpp>
#include <sprout/bitset.hpp>
#include <sprout/string.hpp>

namespace util {
    // ・bit_sum関数
    // バイナリにおける1の数を返す関数．
    constexpr int BITS_COUNT_TABLE[256] = {
        0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
        1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
        1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
        1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
        3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
        1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
        3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
        3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
        3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
        4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8,
    };

    // 上限は64ビットなのでintで十分．
    template <typename Numeric, typename std::enable_if <std::is_unsigned <Numeric>::value>::type * = nullptr>
    constexpr int bit_sum(Numeric bits)
    {
        int num = 0;

        for (int i = 0; i < sizeof(bits); i++) {
            num += BITS_COUNT_TABLE[((unsigned char *) &bits)[i]];
        }

        return num;
    }

    template <typename Numeric, typename std::enable_if <std::is_unsigned <Numeric>::value>::type * = nullptr>
    constexpr int bit_sum2(Numeric bits)
    {
        int num = 0;
        for (; bits != 0; bits &= bits - 1) {
            num++;
        }

        return num;
    }

    template <typename Numeric, typename std::enable_if <std::is_unsigned <Numeric>::value>::type * = nullptr>
    constexpr Numeric bit_roll(Numeric bits)
    {
        size_t bit_length = 8*sizeof(Numeric);
        Numeric result = 0;
        for (size_t i = 0; i < bit_length; i++) {
            result <<= 1;
            result += (bits & 1);
            bits >>= 1;
        }
        return result;
    }

    template <typename Numeric, typename std::enable_if <std::is_unsigned <Numeric>::value>::type * = nullptr>
    constexpr Numeric bit_roll(Numeric bits, size_t bit_length)
    {
        size_t numeric_size = 8 * sizeof(Numeric);
        if (bit_length > numeric_size) {
            bit_length = numeric_size;
        }
        Numeric result = 0;
        for (size_t i = 0; i < bit_length; i++) {
            result <<= 1;
            result += (bits & 1);
            bits >>= 1;
        }
        return result;
    }

    template <typename Numeric, typename std::enable_if <std::is_unsigned <Numeric>::value>::type * = nullptr>
    constexpr size_t rank1(Numeric bits, size_t index)
    {
        size_t bit_length = 8 * sizeof(Numeric);

        return (index <= 0)
               ? 0
               : ((bit_length < index)
                  ? bit_sum2(bits)
                  : util::bit_sum2(bits >> (bit_length - index))
                  );
    }

    template <typename Numeric, typename std::enable_if <std::is_unsigned <Numeric>::value>::type * = nullptr>
    constexpr size_t rank1(Numeric bits, size_t index, size_t bit_length)
    {
        size_t numeric_size = 8 * sizeof(Numeric);
        if (bit_length > numeric_size) {
            bit_length = numeric_size;
        }
        bits <<= (numeric_size - bit_length);

        return rank1(bits, index);
    }

    template <typename Numeric, typename std::enable_if <std::is_unsigned <Numeric>::value>::type * = nullptr>
    constexpr size_t rank0(Numeric bits, size_t index)
    {
        size_t bit_length = 8 * sizeof(Numeric);

        return (index <= 0)
               ? 0
               : ((bit_length < index)
                  ? bit_length - rank1(bits, index)
                  : index - rank1(bits, index)
                  );
    }

    template <typename Numeric, typename std::enable_if <std::is_unsigned <Numeric>::value>::type * = nullptr>
    constexpr size_t rank0(Numeric bits, size_t index, size_t bit_length)
    {
        return (index <= 0)
               ? 0
               : ((bit_length < index)
                  ? bit_length - rank1(bits, index, bit_length)
                  : index - rank1(bits, index, bit_length)
                  );
    }

    template <typename Numeric, typename std::enable_if <std::is_unsigned <Numeric>::value>::type * = nullptr>
    constexpr size_t select1(Numeric bits, size_t order)
    {
        if (order <= 0) {
            return 0;
        }
        size_t bit_length = 8 * sizeof(Numeric);
        bits = bit_roll(bits, bit_length);

        size_t index = 0;
        size_t sum   = 0;
        for (; index < bit_length; index++) {
            if (sum >= order) {
                break;
            }
            sum   += (bits & 1);
            bits >>= 1;
        }

        return (sum < order) ? 0 : index;
    }

    template <typename Numeric>
    constexpr size_t select1(Numeric bits, size_t order, size_t bit_length)
    {
        if (order <= 0) {
            return 0;
        }
        bits = bit_roll(bits, bit_length);

        size_t index = 0;
        size_t sum   = 0;
        for (; index < bit_length; index++) {
            if (sum >= order) {
                break;
            }
            sum   += (bits & 1);
            bits >>= 1;
        }

        return (sum < order) ? 0 : index;
    }

    template <typename Numeric, typename std::enable_if <std::is_unsigned <Numeric>::value>::type * = nullptr>
    constexpr size_t select0(Numeric bits, size_t order)
    {
        if (order <= 0) {
            return 0;
        }
        size_t bit_length = 8 * sizeof(Numeric);
        bits = bit_roll(bits, bit_length);

        size_t index = 0;
        size_t sum   = 0;
        for (; index < bit_length; index++) {
            if (sum >= order) {
                break;
            }
            sum   += 1 - (bits & 1);
            bits >>= 1;
        }

        return (sum < order) ? 0 : index;
    }

    template <typename Numeric>
    constexpr size_t select0(Numeric bits, size_t order, size_t bit_length)
    {
        if (order <= 0) {
            return 0;
        }
        bits = bit_roll(bits, bit_length);

        size_t index = 0;
        size_t sum   = 0;
        for (; index < bit_length; index++) {
            if (sum >= order) {
                break;
            }
            sum   += 1 - (bits & 1);
            bits >>= 1;
        }

        return (sum < order) ? 0 : index;
    }

    // ・paren_to_bitseq関数
    template <size_t length>
    constexpr sprout::bitset <length> paren_to_bitseq(sprout::basic_string <char, length> paren)
    {
        sprout::bitset <length> result = 0;
        for (size_t i = 0; i < length; i++) {
            if (paren[i] == ')') {
                result.set(i);
            }
        }

        return result;
    }

    // ・bit_containerメタ関数
    struct none
    {
        using type = none;
    };

    template <std::size_t bit_length, bool use_none = true>
    struct bit_container
    {
        // ビット長を覆う最小バイト長
        static constexpr size_t bin_length = (bit_length - 1) / 8 + 1;

        using none_type = none;

        using type = typename std::conditional <bin_length <= sizeof(unsigned char),
                                                unsigned char,
                                                typename std::conditional <bin_length <= sizeof(unsigned short),
                                                                           unsigned short,
                                                                           typename std::conditional <bin_length <= sizeof(unsigned int),
                                                                                                      unsigned int,
                                                                                                      typename std::conditional <bin_length <= sizeof(unsigned long),
                                                                                                                                 unsigned long,
                                                                                                                                 typename std::conditional <use_none,
                                                                                                                                                            none,
                                                                                                                                                            unsigned long
                                                                                                                                                            >::type
                                                                                                                                 >::type
                                                                                                      >::type
                                                                           >::type
                                                >::type;
    };

    template <std::size_t bit_length, bool use_none = true>
    using bit_container_t                           = typename bit_container <bit_length, use_none>::type;

    // ・containerメタ関数
    // 処理系依存のような気がする（size_t = unsigned long）．
    template <unsigned long num, bool use_none = true>
    struct container
    {
        static constexpr size_t max_uchar  = std::numeric_limits <unsigned char>::max();
        static constexpr size_t max_ushort = std::numeric_limits <unsigned short>::max();
        static constexpr size_t max_uint   = std::numeric_limits <unsigned int>::max();
        static constexpr size_t max_ulong  = std::numeric_limits <unsigned long>::max();

        using none_type = none;

        using type = typename std::conditional <num <= max_uchar,
                                                unsigned char,
                                                typename std::conditional <num <= max_ushort,
                                                                           unsigned short,
                                                                           typename std::conditional <num <= max_uint,
                                                                                                      unsigned int,
                                                                                                      typename std::conditional <num <= max_ulong,
                                                                                                                                 unsigned long,
                                                                                                                                 typename std::conditional <use_none,
                                                                                                                                                            none,
                                                                                                                                                            unsigned long
                                                                                                                                                            >::type
                                                                                                                                 >::type
                                                                                                      >::type
                                                                           >::type
                                                >::type;
    };

    template <unsigned long num, bool use_none = true>
    using container_t                          = typename container <num, use_none>::type;

    // ・half_containerメタ関数
    // 半分のビット長を持つコンテナを返す．
    template <class Numeric, bool use_none = true>
    struct half_container
    {
        using none_type = none;

        using type = typename std::conditional <std::is_same <Numeric, char>::value,
                                                typename std::conditional <use_none,
                                                                           none,
                                                                           char
                                                                           >::type,
                                                typename std::conditional <std::is_same <Numeric, unsigned char>::value,
                                                                           typename std::conditional <use_none,
                                                                                                      none,
                                                                                                      unsigned char
                                                                                                      >::type,
                                                                           typename std::conditional <std::is_same <Numeric, short>::value,
                                                                                                      char,
                                                                                                      typename std::conditional <std::is_same <Numeric, unsigned short>::value,
                                                                                                                                 unsigned char,
                                                                                                                                 typename std::conditional <std::is_same <Numeric, int>::value,
                                                                                                                                                            short,
                                                                                                                                                            typename std::conditional <std::is_same <Numeric, unsigned int>::value,
                                                                                                                                                                                       unsigned short,
                                                                                                                                                                                       typename std::conditional <std::is_same <Numeric,
                                                                                                                                                                                                                                long>::value,
                                                                                                                                                                                                                  int,
                                                                                                                                                                                                                  typename std::conditional <std
                                                                                                                                                                                                                                             ::
                                                                                                                                                                                                                                             is_same
                                                                                                                                                                                                                                             <
                                                                                                                                                                                                                                                 Numeric,
                                                                                                                                                                                                                                                 unsigned
                                                                                                                                                                                                                                                 long>
                                                                                                                                                                                                                                             ::
                                                                                                                                                                                                                                             value,
                                                                                                                                                                                                                                             unsigned
                                                                                                                                                                                                                                             int,
                                                                                                                                                                                                                                             none
                                                                                                                                                                                                                                             >::
                                                                                                                                                                                                                  type
                                                                                                                                                                                                                  >::type
                                                                                                                                                                                       >::type
                                                                                                                                                            >::type
                                                                                                                                 >::type
                                                                                                      >::type
                                                                           >::type
                                                >::type;
    };

    template <class Numeric, bool use_none = true>
    using half_container_t                 = typename half_container <Numeric, use_none>::type;

    // ・linspace関数
    template <typename Numeric, size_t length>
    constexpr sprout::array <Numeric, length> linspace(Numeric start, Numeric difference=1)
    {
        sprout::array <Numeric, length> result = sprout::array <Numeric, length>();
        result[0] = start;
        for (size_t i = 1; i < length; i++) {
            result[i] = result[i - 1] + difference;
        }

        return result;
    }

    // ・c_str_to_array関数
    // char型の配列から0-9からなる配列を生成する．
    // 非数が出現したら終了して残りを0で埋める．
    template <typename Numeric, size_t length>
    constexpr sprout::array <Numeric, length> c_str_to_array(const char *org_array)
    {
        sprout::array <Numeric, length> result = sprout::array <Numeric, length>();
        for (size_t i = 0; i < length; i++) {
            if (std::isdigit(org_array[i])) {
                result[i] = org_array[i] - '0';
            } else {
                break;
            }
        }

        return result;
    }

    // ・c_str_to_block_bitseq関数
    // char型の配列からブロックビット列を生成する．
    // ビット列はblock_num個のblock_tに格納する．
    template <typename block_t, int block_num>
    constexpr sprout::array <block_t, block_num> c_str_to_block_bitseq(const char *org_array, std::size_t n)
    {
        constexpr std::size_t block_size = 8 * sizeof(block_t);

        sprout::array <block_t, block_num> result = sprout::array <block_t, block_num>();
        for (size_t i = 0; i < n && i < block_size * block_num; i++) {
            result[i / block_size] <<= 1;
            if (org_array[i] == '\0') {
                break;
            } else if (org_array[i] == '1') {
                result[i / block_size]++;
            }
        }

        return result;
    }

    // ・numeric_to_block_bitseq関数
    // numeric型からブロックビット列を生成する．
    // ビット列はblock_num個のblock_tに格納する．
    template <typename block_t, int block_num>
    constexpr sprout::array <block_t, block_num> numeric_to_block_bitseq(const unsigned long org_array)
    {
        constexpr std::size_t block_size = 8 * sizeof(block_t);

        unsigned long                      source = org_array;
        sprout::array <block_t, block_num> result = sprout::array <block_t, block_num>();
        // org_arrayを表現できるか，残りのblockがなくなるかすれば終了．
        for (size_t i = 0; i < block_num; i++) {
            if (source <= 0) {
                break;
            }
            result[block_num - 1 - i] = static_cast <block_t>(source);
            source                  >>= block_size;
        }

        return result;
    }

    // ・array_to_block_bitseq関数
    // 配列からブロックビット列を生成する．
    // ビット列はblock_num個のblock_tに格納する．
    template <typename block_t, int block_num>
    constexpr sprout::array <block_t, block_num> array_to_block_bitseq(const sprout::array <unsigned char, 8 *sizeof(block_t) *block_num> org_array)
    {
        constexpr std::size_t block_size = 8 * sizeof(block_t);

        sprout::array <block_t, block_num> result = sprout::array <block_t, block_num>();
        for (size_t i = 0; i < block_num; i++) {
            for (size_t j = 0; j < block_size; j++) {
                if (org_array[block_size * i + j] == 1) {
                    result[i]++;
                }
                result[i] <<= 1;
            }
        }

        return result;
    }

    // ・bitset_to_block_bitseq関数
    // bitsetからブロックビット列を生成する．
    // ビット列はblock_num個のblock_tに格納する．
    template <typename block_t, int block_num>
    constexpr sprout::array <block_t, block_num> bitset_to_block_bitseq(const sprout::bitset <8 *sizeof(block_t) *block_num> org_array)
    {
        constexpr std::size_t block_size = 8 * sizeof(block_t);

        sprout::array <block_t, block_num> result = sprout::array <block_t, block_num>();
        for (size_t i = 0; i < block_num; i++) {
            for (size_t j = 0; j < block_size; j++) {
                result[i]  += org_array[block_size * i + j];
                result[i] <<= 1;
            }
        }

        return result;
    }

    // ・rank関数
    // O(n)時間．小さいbitsetに使用する．
    template <size_t length>
    constexpr unsigned long rank(int a, const sprout::bitset <length> &bs, unsigned long index)
    {
        unsigned long order = 0;
        // indexは1-origin，bitsetは0-origin．
        for (size_t i = 0; i < index && i < length; i++) {
            if (bs[length - 1 - i] == a) {
                order++;
            }
        }

        return order;
    }

    template <size_t length>
    unsigned long rank(int a, const std::bitset <length> &bs, unsigned long index)
    {
        unsigned long order = 0;
        // indexは1-origin，bitsetは0-origin．
        for (size_t i = 0; i < index && i < length; i++) {
            if (bs[length - 1 - i] == a) {
                order++;
            }
        }

        return order;
    }

    // ・access関数
    template <size_t length>
    constexpr unsigned long access(const sprout::bitset <length> &bs, unsigned long index)
    {
        return bs[length - 1 - index];
    }

    template <size_t length>
    unsigned long access(const std::bitset <length> &bs, unsigned long index)
    {
        return bs[length - 1 - index];
    }

    // ・select関数
    // O(n)時間．小さいbitsetに使用する．
    template <size_t length>
    constexpr unsigned long select(int a, const sprout::bitset <length> &bs, unsigned long order)
    {
        // 0以下の場合
        if (order <= 0) {
            return 0;
        }
        unsigned long rank  = 0;
        unsigned long index = 0;
        // indexは1-origin，bitsetは0-origin．
        for (index = 0; index < length; index++) {
            if (bs[length - 1 - index] == a) {
                rank++;
                if (rank >= order) {
                    break;
                }
            }
        }

        return index + 1;
    }

    // O(n)時間．小さいbitsetに使用する．
    template <size_t length>
    unsigned long select(int a, const std::bitset <length> &bs, unsigned long order)
    {
        // 0以下の場合
        if (order <= 0) {
            return 0;
        }
        unsigned long rank  = 0;
        unsigned long index = 0;
        // indexは1-origin，bitsetは0-origin．
        for (index = 0; index < length; index++) {
            if (bs[length - 1 - index] == a) {
                rank++;
                if (rank >= order) {
                    break;
                }
            }
        }

        return index + 1;
    }

}

#endif
