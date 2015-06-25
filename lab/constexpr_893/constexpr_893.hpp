#ifndef CONSTEXPR_893
#define CONSTEXPR_893

#include "../util.hpp"

namespace constexpr_893 {
    // コンパイル時構築可能なユーザ定義クラス（リテラルクラス）の定義．
    class Simple_class
    {
        using block_t = int;
        static constexpr size_t block_num = 4;
        static constexpr size_t superblock_num = 2;
        using superblock_rank_t = int;
        using block_rank_t = int;
    public:
        constexpr Simple_class()
            : _org_array(util::constexpr_array <block_t, block_num>()), _superblock_rank1(util::constexpr_array<superblock_rank_t, superblock_num>()), _block_rank1(util::constexpr_array<block_rank_t, block_num>())
        {
        }

        constexpr int akari() const
        {
            return 0;
        }

    private:
        const util::constexpr_array <block_t, block_num>          _org_array;
        util::constexpr_array <superblock_rank_t, superblock_num> _superblock_rank1;
        util::constexpr_array <block_rank_t, block_num>           _block_rank1;
    };

    void testSimple_class()
    {
        constexpr Simple_class sc;
        constexpr auto         akari = sc.akari();
        std::cout << akari << std::endl;
    }
}

#endif
