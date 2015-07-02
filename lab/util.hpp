#ifndef UTIL
#define UTIL

#include <iostream>
#include <string>
#include <array>
#include <iterator>
#include <typeinfo>
#include <type_traits>
#include <utility>
#include <cstddef>
#include <tuple>
#include <bitset>
#include <limits>
#include <boost/type.hpp>
#include <boost/optional.hpp>
#include <boost/mpl/less_equal.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/mpl/sizeof.hpp>
#include <boost/mpl/bool.hpp>
#include <sprout/array.hpp>
#include <sprout/bitset.hpp>
#include <sprout/string.hpp>
#include <sprout/algorithm/fixed/reverse.hpp>

namespace util {
    // ・epsilon
    constexpr double epsilon = 10e-6;

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

    // ・repeat関数
    // 文字列strをdelimで区切ってn回osに出力する
    void repeat(std::ostream &os, const std::string &str, int n)
    {
        for (int i = 0; i < n; i++) {
            os << str;
        }
    }

    void repeat(std::ostream &os, const std::string &str, int n, char delim)
    {
        for (int i = 0; i < n - 1; i++) {
            os << str << delim;
        }
        os << str;
    }

    // ・Repeatクラス
    // std::cout << Repeat("gfn", "2") << std::endl;
    // のように使う．
    class Repeat
    {
    public:
        Repeat(const std::string &str, const int n, const char delim='\0');
        friend std::ostream&operator<<(std::ostream &os, const Repeat &rep);

    private:
        const std::string _str;
        const int         _n;
        const char        _delim;
    };

    Repeat::Repeat(const std::string &str, const int n, const char delim)
        : _str(str), _n(n), _delim(delim)
    {
    }

    std::ostream&operator<<(std::ostream &os, const Repeat &rep)
    {
        for (int i = 0; i < rep._n; i++) {
            os << rep._str;
            if (rep._delim == '\0' && i != rep._n - 1) {
                os << rep._delim;
            }
        }

        return os;
    }

    // ・imprementation_test関数
    // 処理系依存の環境をテストする関数．
    void imprementation_test()
    {
        std::cout << Repeat("-", 20) << std::endl;
        std::cout << "imprementation test" << std::endl;

        std::cout << std::endl;

        std::cout << "size test" << std::endl;
        std::cout << "sizeof(char) : " << sizeof(char) << "[byte]" << std::endl;
        std::cout << "sizeof(short): " << sizeof(short) << "[byte]" << std::endl;
        std::cout << "sizeof(int)  : " << sizeof(int) << "[byte]" << std::endl;
        std::cout << "sizeof(long) : " << sizeof(long) << "[byte]" << std::endl;

        std::cout << std::endl;

        std::cout << "downcast test" << std::endl;
        unsigned long ul = 549755813887;
        std::cout << "unsigned long : " << std::bitset <64>(static_cast <unsigned long>(ul)).to_string() << std::endl;
        std::cout << "unsigned int  : " << std::bitset <64>(static_cast <unsigned int>(ul)).to_string() << std::endl;
        std::cout << "unsigned short: " << std::bitset <64>(static_cast <unsigned short>(ul)).to_string() << std::endl;
        std::cout << "unsigned char : " << std::bitset <64>(static_cast <unsigned char>(ul)).to_string() << std::endl;

        std::cout << Repeat("-", 20) << std::endl;
    }

    // ・power関数
    template <typename T>
    constexpr T power(const T base, int exponent)
    {
        T result = base;
        for (int i = 1; i < exponent; i++) {
            result *= base;
        }

        return result;
    }

    // ・lg関数
    // ビット長を求める．
    template <typename Numeric>
    constexpr Numeric lg(const Numeric n)
    {
        Numeric _n  = n;
        Numeric lgn = 0;
        while (_n) {
            lgn++;
            _n >>= 1;
        }

        return lgn;
    }

    // ・ifloor関数
    constexpr int ifloor(const double d)
    {
        if (d > 0) {
            // 正の場合
            return static_cast <int>(d);
        } else {
            // 負の場合
            return static_cast <int>(d + epsilon) - 1;
        }
    }

    // ・iceil関数
    constexpr int iceil(const double d)
    {
        if (d > 0) {
            // 正の場合
            return static_cast <int>(d - epsilon) + 1;
        } else {
            // 負の場合
            return static_cast <int>(d);
        }
    }

    // ・iround関数
    constexpr int iround(const double d)
    {
        if (d > 0) {
            // 正の場合
            return static_cast <int>(d + 0.5);
        } else {
            // 負の場合
            return static_cast <int>(d - 0.5);
        }
    }

    // ・extract_digit関数
    // 整数列のn桁目を抽出して整数列を作る．
    template <typename Numeric, size_t length>
    std::array <Numeric, length> extract_digit(const std::array <Numeric, length> &org_array, const size_t radix, const size_t digit)
    {
        std::array <Numeric, length> result = std::array <Numeric, length>();
        if (digit == 0) {
            return result;
        }

        for (size_t i = 0; i < length; i++) {
            result[i] = (org_array[i] / power(radix, digit - 1)) % power(radix, digit);
        }

        return result;
    }

    template <typename Numeric, size_t length>
    constexpr sprout::array <Numeric, length> extract_digit(const sprout::array <Numeric, length> &org_array, const size_t radix, const size_t digit)
    {
        sprout::array <Numeric, length> result = std::array <Numeric, length>();
        if (digit == 0) {
            return result;
        }

        for (size_t i = 0; i < length; i++) {
            result[i] = (org_array[i] / power(radix, digit - 1)) % power(radix, digit);
        }

        return result;
    }

    // ・extract_bit関数
    // extract_digit関数の2進数版．
    // テンプレート関数を部分特殊化させろ（おこ）
    template <typename Numeric, size_t length>
    std::bitset <length> extract_bit(const std::array <Numeric, length> &org_array, const size_t digit, const bool from_top=false)
    {
        std::bitset <length> result = std::bitset <length>();
        if (digit == 0) {
            return result;
        }

        // ビットシフトは既存の領域を利用するためコピーしなければならない．
        for (size_t i = 0; i < length; i++) {
            Numeric buffer_for_bitshift = org_array[i];
            buffer_for_bitshift >>= (from_top) ? (8 * sizeof(Numeric) - digit) : (digit - 1);
            result[i]             = buffer_for_bitshift & 1;
        }

        return result;
    }

    template <typename Numeric, size_t length>
    std::bitset <length> extract_bit(std::array <Numeric, length> &&org_array, const size_t digit, const bool from_top=false)
    {
        std::bitset <length> result = std::bitset <length>();
        if (digit == 0) {
            return result;
        }

        // 右辺値の場合は破壊してよい．
        for (size_t i = 0; i < length; i++) {
            org_array[i] >>= (from_top) ? (8 * sizeof(Numeric) - digit) : (digit - 1);
            result[i]      = org_array[i] & 1;
        }

        return result;
    }

    template <typename Numeric, size_t length>
    constexpr sprout::bitset <length> extract_bit(const sprout::array <Numeric, length> &org_array, const size_t digit, const bool from_top=false)
    {
        sprout::bitset <length> result = std::bitset <length>();
        if (digit == 0) {
            return result;
        }

        // ビットシフトは既存の領域を利用するためコピーしなければならない．
        for (size_t i = 0; i < length; i++) {
            Numeric buffer_for_bitshift = org_array[i];
            buffer_for_bitshift >>= (from_top) ? (8 * sizeof(Numeric) - digit) : (digit - 1);
            result[i]             = buffer_for_bitshift & 1;
        }

        return result;
    }

    template <typename Numeric, size_t length>
    constexpr sprout::bitset <length> extract_bit(sprout::array <Numeric, length> &&org_array, const size_t digit, const bool from_top=false)
    {
        sprout::bitset <length> result = std::bitset <length>();
        if (digit == 0) {
            return result;
        }

        // 右辺値の場合は破壊してよい．
        for (size_t i = 0; i < length; i++) {
            org_array[i] >>= (from_top) ? (8 * sizeof(Numeric) - digit) : (digit - 1);
            result[i]      = org_array[i] & 1;
        }

        return result;
    }

    // ・reverse関数
    // イテレータがない場合．
    template <class T>
    constexpr T reverse(const T &container)
    {
        T result = container;
        for (size_t i = 0; i < container.size(); i++) {
            result[i] = container[container.size() - 1 - i];
        }

        return std::move(result);
    }

    // ・slice関数
    template <int i, int j>
    struct Slice
    {
        template <template <int> class T, int n>
        constexpr static T <j - i> slice(const T <n> &container)
        {
            T <j - i> result;
            for (size_t k = i; k < j; k++) {
                result[k - i] = container[k];
            }

            return std::move(result);
        }
    };

    // ・HSVtoRGB関数
    // hue \in \set{Z}, saturation = 0, ..., 255, value = 0, ..., 255.
    boost::optional <std::tuple <int, int, int> > HSVtoRGB(int hue, int saturation, int value)
    {
        if (saturation < 0 || saturation > 255) {
            return boost::none;
        }
        if (value < 0 || value > 255) {
            return boost::none;
        }

        int    hi, p, q, t;
        double f;

        hi = (hue / 60) % 6;
        f  = hue / 60. - static_cast <int>(hue / 60.);
        p  = static_cast <int>(round(value * (1. - (saturation / 255.))));
        q  = static_cast <int>(round(value * (1. - (saturation / 255.) * f)));
        t  = static_cast <int>(round(value * (1. - (saturation / 255.) * (1. - f))));

        int red, green, blue;
        switch (hi)
        {
            case 0:
                red   = value;
                green = t;
                blue  = p;
                break;
            case 1:
                red   = q;
                green = value;
                blue  = p;
                break;
            case 2:
                red   = p;
                green = value;
                blue  = t;
                break;
            case 3:
                red   = p;
                green = q;
                blue  = value;
                break;
            case 4:
                red   = t;
                green = p;
                blue  = value;
                break;
            case 5:
                red   = value;
                green = p;
                blue  = q;
                break;
            default:

                return boost::none;
                break;
        }

        return std::tuple <int, int, int>(red, green, blue);
    }

    // ・nresult_ofメタ関数
    // 関数fにT型の引数をn個渡した時の返り値の型を返すメタ関数

    // 内部的に用いる_nresult_of_sub関数
    template <typename Functor, typename T, int n, typename ... Args>
    struct _nresult_of_sub
    {
        using type = typename _nresult_of_sub <Functor, T, n - 1, T, Args ...>::type;
    };

    template <typename Functor, typename T, typename ... Args>
    struct _nresult_of_sub <Functor, T, 0, Args ...>
    {
        using type = typename std::result_of <Functor(Args ...)>::type;
    };

    template <typename Functor, typename T, int n>
    struct nresult_of
    {
        using type = typename _nresult_of_sub <Functor, T, n - 1, T>::type;
    };

    // nresult_of<F, T, 2>::type
    // = _nresult_of_sub<F, T, 1, T>::type
    // = _nresult_of_sub<F, T, 0, T, T>::type
    // = std::result_of<F(T, T)>::type

    // nresult_of<F, T, 1>::type
    // = _nresult_of_sub<F, T, 0, T>::type
    // = std::result_of<F(T)>::type

    // エイリアステンプレート
    template <typename Functor, typename T, int n>
    using nresult_of_t = typename nresult_of <Functor, T, n>::type;

    // std::arrayの拡張
    // --- ここから人様のコピペ

    // [1次元]

    // ・make_array関数
    // 関数テンプレートの型推論によってconstexprなarrayを要素数なしで生成する．
    template <typename T, typename ... Args>
    constexpr std::array <T, sizeof ... (Args)> make_array(Args&& ... args)
    {
        return std::array <T, sizeof ... (Args)> { static_cast <Args &&>(args) ... };
    }

    // コンパイル時に作成したstd::arrayについてはコンパイル時assert（static_assert）で要素数などをチェック可能．そのためのconstexprなsize関数を定義する．

    // ・array_size関数
    // std::arrayの要素数を取得する．
    template <typename T, std::size_t N>
    constexpr std::size_t array_size(const std::array <T, N>&)
    {
        return N;
    }

    // 固定長配列の要素数を取得する．
    template <typename T, std::size_t N>
    constexpr std::size_t array_size(T(&)[N])
    {
        return N;
    }

    // [多次元]

    // ・multi_arrayクラス
    // std::arrayの多次元版のラッパークラス
    //
    // ＜動機＞T x[a][b][c]なる多次元配列のstd::array版をmulti_array<T, a, b, c> xで記述したい．
    //     array<array<array<T, c>, b>, a>
    // のエイリアステンプレートを定義することによって実現する．

    // 可変長テンプレートパラメータを再帰で展開する．

    // 多次元版（再帰）
    template <typename T, std::size_t Size, std::size_t ... Sizes>
    struct multi_array_type
    {
        using type = std::array <typename multi_array_type <T, Sizes ...>::type, Size>;
    };

    // 1次元版（再帰の末端）
    template <typename T, std::size_t Size>
    struct multi_array_type <T, Size>
    {
        using type = std::array <T, Size>;
    };

    // エイリアステンプレート
    template <typename T, std::size_t Size, std::size_t ... Sizes>
    using multi_array = typename multi_array_type <T, Size, Sizes ...>::type;

    // ・make_common_array関数
    // 関数テンプレートの型推論によってconstexprなmulti_arrayを型名，要素数なしで生成する．

    // 引数は1次元．多次元array（multi_array）を生成するときは1次元arrayからなる1次元arrayを引数にとる．
    // 自身が挙動をあまり理解していない．

    // 戻り値の型を定めるメタ関数
    template <typename ... TArgs>
    struct common_array_type
    {
        using type = std::array <typename std::decay <typename std::common_type <TArgs ...>::type>::type, sizeof ... (TArgs)>;
    };

    // 関数本体
    template <typename ... TArgs>
    constexpr typename common_array_type <TArgs ...>::type make_common_array(TArgs&& ... args)
    {
        return typename common_array_type <TArgs ...>::type { std::forward <TArgs>(args) ... };
    }

    // --- ここまで人様のコピペ

    // ・convert_arrayメタ関数
    // multi_array<T1, ...>からmulti_array<T2, ...>を得る．
    // ただ，multi_arrayの次元には任意性があるので（arrayを要素に持つmulti_arrayかもしれない），m(>= n)次元のmulti_array型をn次元で切ってT2に置換した型を得ることにする．

    // プライマリテンプレート．ダミー
    template <class From, typename T2, int dim>
    struct convert_array
    {
        using type = From;
    };

    // 再帰版
    template <typename T2, int dim, class Fromsub, int n>
    struct convert_array <std::array <Fromsub, n>, T2, dim>
    {
        using type = std::array <typename convert_array <Fromsub, T2, dim - 1>::type, n>;
    };

    // 再帰の末端
    template <typename T2, typename T1, int n>
    struct convert_array <std::array <T1, n>, T2, 1>
    {
        using type = std::array <T2, n>;
    };

    // ・convert_array_f関数
    // 実際のmulti_arrayを受け取って型変換してデフォルトコンストラクトしたものを返す関数
    template <class From, typename T2, int dim, class Result = typename convert_array <From, T2, dim>::type>
    Result convert_array_f(From A)
    {
        return std::move(Result());
    }

    // ・apply関数
    // Rにおけるapply関数を実装する．

    // (1)基本的に副作用を認めないものとし，副作用を生じる場合はapply_side_effect関数を用いる．
    // (2)多次元版と1次元版を次元に関する部分特殊化で分岐する．
    // (3)左辺値版と右辺値版をオーバーロードする．
    // (4)voidな関数と値を返す関数はデフォルトテンプレートパラメータの部分特殊化で分岐する．
    // (5)multi_arrayの要素の型による多態は関数テンプレートの型推論が担保する．

    // 以上の設計から，クラステンプレートにメンバ関数テンプレートを持たせる実装を行う．

    // 返り値を持つ版
    // プライマリテンプレート
    template <typename T, int dim, typename Resultvoid = T>
    struct Apply
    {
        // const lvalue版
        template <class Fromsub, std::size_t n, typename Functor, typename FResult = typename std::result_of <Functor(T)>::type, class Result = typename convert_array <std::array <Fromsub, n>, FResult, dim>::type>
        constexpr static Result apply(const std::array <Fromsub, n> &x, const Functor &f)
        {
            Result applied_array;

            {
                auto from = x.begin();
                auto to   = applied_array.begin();
                for (; from != x.end(); ++from, ++to) {
                    *to = Apply <T, dim - 1>::apply(*from, f);
                }
            }

            return std::move(applied_array);
        }

        // lvalue版
        template <class Fromsub, std::size_t n, typename Functor, typename FResult = typename std::result_of <Functor(T)>::type, class Result = typename convert_array <std::array <Fromsub, n>, FResult, dim>::type>
        constexpr static Result apply_side_effect(std::array <Fromsub, n> &x, const Functor &f)
        {
            Result applied_array;

            {
                auto from = x.begin();
                auto to   = applied_array.begin();
                for (; from != x.end(); ++from, ++to) {
                    *to = Apply <T, dim - 1>::apply_side_effect(*from, f);
                }
            }

            return std::move(applied_array);
        }

        // const rvalue版
        template <class Fromsub, std::size_t n, typename Functor, typename FResult = typename std::result_of <Functor(T)>::type, class Result = typename convert_array <std::array <Fromsub, n>, FResult, dim>::type>
        constexpr static Result apply(const std::array <Fromsub, n> &&x, const Functor &f)
        {
            Result applied_array;

            {
                auto from = x.begin();
                auto to   = applied_array.begin();
                for (; from != x.end(); ++from, ++to) {
                    *to = Apply <T, dim - 1>::apply(*from, f);
                }
            }

            return std::move(applied_array);
        }

        // rvalue版
        template <class Fromsub, std::size_t n, typename Functor, typename FResult = typename std::result_of <Functor(T)>::type, class Result = typename convert_array <std::array <Fromsub, n>, FResult, dim>::type>
        constexpr static Result apply_side_effect(std::array <Fromsub, n> &&x, const Functor &f)
        {
            Result applied_array;

            {
                auto from = x.begin();
                auto to   = applied_array.begin();
                for (; from != x.end(); ++from, ++to) {
                    *to = Apply <T, dim - 1>::apply_side_effect(*from, f);
                }
            }

            return std::move(applied_array);
        }
    };

    // 1次元版
    template <typename T, typename Resultvoid>
    struct Apply <T, 1, Resultvoid>
    {
        // const lvalue版
        template <std::size_t n, typename Functor, typename FResult = typename std::result_of <Functor(T)>::type, class Result = typename convert_array <std::array <T, n>, FResult, 1>::type>
        constexpr static Result apply(const std::array <T, n> &x, const Functor &f)
        {
            // std::cout << "(const lvalue, nonvoid called)" << std::endl;
            Result applied_array;

            {
                auto from = x.begin();
                auto to   = applied_array.begin();
                for (; from != x.end(); ++from, ++to) {
                    *to = f(*from);
                }
            }

            return std::move(applied_array);
        }

        // lvalue版
        template <std::size_t n, typename Functor, typename FResult = typename std::result_of <Functor(T)>::type, class Result = typename convert_array <std::array <T, n>, FResult, 1>::type>
        constexpr static Result apply_side_effect(std::array <T, n> &x, const Functor &f)
        {
            // std::cout << "(lvalue, nonvoid called)" << std::endl;
            Result applied_array;

            {
                auto from = x.begin();
                auto to   = applied_array.begin();
                for (; from != x.end(); ++from, ++to) {
                    *to = f(*from);
                }
            }

            return std::move(applied_array);
        }

        // const rvalue﻿﻿版
        template <std::size_t n, typename Functor, typename FResult = typename std::result_of <Functor(T)>::type, class Result = typename convert_array <std::array <T, n>, FResult, 1>::type>
        constexpr static Result apply(const std::array <T, n> &&x, const Functor &f)
        {
            // std::cout << "(const rvalue, nonvoid called)" << std::endl;
            Result applied_array;

            {
                auto from = x.begin();
                auto to   = applied_array.begin();
                for (; from != x.end(); ++from, ++to) {
                    *to = f(*from);
                }
            }

            return std::move(applied_array);
        }

        // rvalue﻿﻿版
        template <std::size_t n, typename Functor, typename FResult = typename std::result_of <Functor(T)>::type, class Result = typename convert_array <std::array <T, n>, FResult, 1>::type>
        constexpr static Result apply_side_effect(std::array <T, n> &&x, const Functor &f)
        {
            // std::cout << "(rvalue, nonvoid called)" << std::endl;
            Result applied_array;

            {
                auto from = x.begin();
                auto to   = applied_array.begin();
                for (; from != x.end(); ++from, ++to) {
                    *to = f(*from);
                }
            }

            return std::move(applied_array);
        }
    };

    // void版
    // プライマリテンプレート
    template <typename T, int dim>
    struct Apply <T, dim, void>
    {
        // const lvalue版
        template <class Fromsub, std::size_t n, typename Functor>
        constexpr static void apply(const std::array <Fromsub, n> &x, const Functor &f)
        {
            {
                auto from = x.begin();
                for (; from != x.end(); ++from) {
                    Apply <T, dim - 1, void>::apply(*from, f);
                }
            }
        }

        // lvalue版
        template <class Fromsub, std::size_t n, typename Functor>
        constexpr static void apply_side_effect(std::array <Fromsub, n> &x, const Functor &f)
        {
            {
                auto from = x.begin();
                for (; from != x.end(); ++from) {
                    Apply <T, dim - 1, void>::apply_side_effect(*from, f);
                }
            }
        }

        // const rvalue版
        template <class Fromsub, std::size_t n, typename Functor>
        constexpr static void apply(const std::array <Fromsub, n> &&x, const Functor &f)
        {
            {
                auto from = x.begin();
                for (; from != x.end(); ++from) {
                    Apply <T, dim - 1, void>::apply(*from, f);
                }
            }
        }

        // rvalue版
        template <class Fromsub, std::size_t n, typename Functor>
        constexpr static void apply_side_effect(std::array <Fromsub, n> &&x, const Functor &f)
        {
            {
                auto from = x.begin();
                for (; from != x.end(); ++from) {
                    Apply <T, dim - 1, void>::apply_side_effect(*from, f);
                }
            }
        }
    };

    // 1次元版
    template <typename T>
    struct Apply <T, 1, void>
    {
        // const lvalue版
        template <std::size_t n, typename Functor>
        constexpr static void apply(const std::array <T, n> &x, const Functor &f)
        {
            // std::cout << "(const lvalue, void called)" << std::endl;
            {
                auto from = x.begin();
                for (; from != x.end(); ++from) {
                    f(*from);
                }
            }
        }

        // lvalue版
        template <std::size_t n, typename Functor>
        constexpr static void apply_side_effect(std::array <T, n> &x, const Functor &f)
        {
            // std::cout << "(lvalue, void called)" << std::endl;
            {
                auto from = x.begin();
                for (; from != x.end(); ++from) {
                    f(*from);
                }
            }
        }

        // const rvalue﻿﻿版
        template <std::size_t n, typename Functor>
        constexpr static void apply(const std::array <T, n> &&x, const Functor &f)
        {
            // std::cout << "(const rvalue, void called)" << std::endl;
            {
                auto from = x.begin();
                for (; from != x.end(); ++from) {
                    f(*from);
                }
            }
        }

        // rvalue﻿﻿版
        template <std::size_t n, typename Functor>
        constexpr static void apply_side_effect(std::array <T, n> &&x, const Functor &f)
        {
            // std::cout << "(rvalue, void called)" << std::endl;
            {
                auto from = x.begin();
                for (; from != x.end(); ++from) {
                    f(*from);
                }
            }
        }
    };

    // ・typename_of関数
    // typeid.name出力をデマングルする．
    // 野良C++erさんのコード．
    #include <cxxabi.h>

    // __cxa_demangleがmallocして返すためメモリリークがある．
    char *demangle(const char *demangled)
    {
        int status;

        return abi::__cxa_demangle(demangled, 0, 0, &status);
    }

    // Tの型名を取得．
    // 参照型を区別しない．
    template <typename T>
    char *typename_of()
    {
        return demangle(typeid(T).name() );
    }

    // Tの型名を取得．
    // 参照型を区別するが，boost::type<>の中に表示されるので冗長．
    template <typename T>
    char *typename_of_detail()
    {
        return demangle(typeid(boost::type<T>).name() );
    }
}

#endif
