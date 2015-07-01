#ifndef TREESHAPE_PAREN
#define TREESHAPE_PAREN

#include <cstddef>
#include <sprout/algorithm/fixed/reverse.hpp>
#include "util.hpp"

// 木構造に関する名前空間
namespace tree {
    // 木構造の形状に関する名前空間
    namespace shape {
        // 木構造の形状の括弧列表現に関する名前空間
        namespace paren {
            // 木の形状のみを抽出した括弧表現型を定義する．
            // インデックスは1-origin．
            // インデックスを返す関数が未定義であるとき，0を返す．
            template <unsigned long bit_expression, size_t length_>
            struct Paren
            {
                // 括弧の釣り合いがとれているかチェック．
                static_assert(2 * util::bit_sum2(bit_expression) == length_, "parentheses are not balanced.");
                static constexpr unsigned long value  = bit_expression;
                static constexpr size_t        length = length_;
            };

            // ユーティリティメタ関数．
            struct _zero
            {
                static constexpr size_t value = 0;
            };

            template <size_t index>
            struct _identity
            {
                static constexpr size_t value = index;
            };

            // indexがopenかcloseかを返すメタ関数．
            // indexが[1, length]以外のとき未定義であり，0．
            template <class Paren_, size_t index>
            struct AT;

            template <typename T, size_t index>
            struct _paren_getbit;

            template <unsigned long bit_expression, size_t length_, size_t index>
            struct _paren_getbit <Paren <bit_expression, length_>, index>
            {
                using _Paren = Paren <bit_expression, length_>;
                static constexpr size_t value = (_Paren::value >> (_Paren::length - index)) & 1;
            };

            template <unsigned long bit_expression, size_t length_, size_t index>
            struct AT <Paren <bit_expression, length_>, index>
            {
                using _Paren = Paren <bit_expression, length_>;
                static constexpr unsigned long value = std::conditional <
                                                       0 < index && index <= length_,
                                               _paren_getbit <_Paren, index>,
                _zero >
                ::type::value;
            };

            // open indexに対応するclose indexを返すメタ関数．
            template <typename T, size_t now_index, size_t left_excess>
            struct _FINDCLOSE;

            template <unsigned long bit_expression, size_t length_, size_t now_index, size_t left_excess>
            struct _FINDCLOSE <Paren <bit_expression, length_>, now_index, left_excess>
            {
                using _Paren = Paren <bit_expression, length_>;
                static constexpr size_t value = std::conditional <
                    AT <_Paren, now_index + 1>::value == 0,
                    _FINDCLOSE <
                        _Paren,
                        now_index + 1,
                        left_excess + 1
                        >,
                    _FINDCLOSE <
                        _Paren,
                        now_index + 1,
                        left_excess - 1
                        >
                    >::type::value;
            };

            template <unsigned long bit_expression, size_t length_, size_t now_index>
            struct _FINDCLOSE <Paren <bit_expression, length_>, now_index, 0>
            {
                using _Paren = Paren <bit_expression, length_>;
                static constexpr size_t value = now_index;
            };

            template <typename T, size_t open_index>
            struct FINDCLOSE;

            template <unsigned long bit_expression, size_t length_, size_t open_index>
            struct FINDCLOSE <Paren <bit_expression, length_>, open_index>
            {
                using _Paren = Paren <bit_expression, length_>;
                static_assert(AT <_Paren, open_index>::value == 0, "not open at index.");
                static constexpr size_t value = _FINDCLOSE <_Paren, open_index, 1>::value;
            };

            // close indexに対応するopen indexを返すメタ関数．
            template <typename T, size_t now_index, size_t right_excess>
            struct _FINDOPEN;

            template <unsigned long bit_expression, size_t length_, size_t now_index, size_t right_excess>
            struct _FINDOPEN <Paren <bit_expression, length_>, now_index, right_excess>
            {
                using _Paren = Paren <bit_expression, length_>;
                static constexpr size_t value = std::conditional <
                    AT <_Paren, now_index - 1>::value == 0,
                    _FINDOPEN <
                        _Paren,
                        now_index - 1,
                        right_excess - 1
                        >,
                    _FINDOPEN <
                        _Paren,
                        now_index - 1,
                        right_excess + 1
                        >
                    >::type::value;
            };

            template <unsigned long bit_expression, size_t length_, size_t now_index>
            struct _FINDOPEN <Paren <bit_expression, length_>, now_index, 0>
            {
                using _Paren = Paren <bit_expression, length_>;
                static constexpr size_t value = now_index;
            };

            template <typename T, size_t close_index>
            struct FINDOPEN;

            template <unsigned long bit_expression, size_t length_, size_t close_index>
            struct FINDOPEN <Paren <bit_expression, length_>, close_index>
            {
                using _Paren = Paren <bit_expression, length_>;
                static_assert(AT <_Paren, close_index>::value == 1, "not close at index.");
                static constexpr size_t value = _FINDOPEN <_Paren, close_index, 1>::value;
            };

            // indexの括弧を最もきつく括る括弧のopen indexを返すメタ関数．
            template <typename T, size_t index>
            struct ENCLOSE;

            template <unsigned long bit_expression, size_t length_, size_t index>
            struct ENCLOSE <Paren <bit_expression, length_>, index>
            {
                using _Paren = Paren <bit_expression, length_>;
                using at     = AT <_Paren, index>;
                static constexpr size_t open_index = std::conditional <
                    at::value == 0,
                    _identity <index>,
                    FINDOPEN <_Paren, index>
                    >::type::value;
                // 括弧が開になるまで繰り返す．
                static constexpr size_t value = std::conditional <
                                                0 < open_index && open_index <= length_,
                typename std::conditional <
                    AT <_Paren, open_index - 1>::value == 0,
                    _identity <open_index - 1>,
                    ENCLOSE <_Paren, open_index - 1>
                    >::type,
                _zero >
                ::type::value;
            };

            // エイリアステンプレート．
            template <typename T, size_t index>
            using PARENT = ENCLOSE <T, index>;

            // indexの最初の子のopen indexを返すメタ関数．
            template <typename T, size_t index>
            struct FIRSTCHILD;

            template <unsigned long bit_expression, size_t length_, size_t index>
            struct FIRSTCHILD <Paren <bit_expression, length_>, index>
            {
                using _Paren = Paren <bit_expression, length_>;
                using at     = AT <_Paren, index>;
                static constexpr size_t open_index = std::conditional <
                    at::value == 0,
                    _identity <index>,
                    FINDOPEN <_Paren, index>
                    >::type::value;
                // open_index+1の括弧が開のとき，最初の子．
                // open_index+1の括弧が閉のとき，子は存在しない．
                static constexpr size_t value = (AT <_Paren, open_index + 1>::value == 0) ? open_index + 1 : 0;
            };

            // indexの最後の子のopen indexを返すメタ関数．
            template <typename T, size_t index>
            struct LASTCHILD;

            template <unsigned long bit_expression, size_t length_, size_t index>
            struct LASTCHILD <Paren <bit_expression, length_>, index>
            {
                using _Paren = Paren <bit_expression, length_>;
                using at     = AT <_Paren, index>;
                static constexpr size_t close_index = std::conditional <
                    at::value == 0,
                    FINDCLOSE <_Paren, index>,
                    _identity <index>
                    >::type::value;
                // close_index-1の括弧が開のとき，子は存在しない．
                // close_index-1の括弧が閉のとき，対応する開括弧が子．
                static constexpr size_t value = std::conditional <
                    AT <_Paren, close_index - 1>::value == 0,
                    _zero,
                    FINDOPEN <_Paren, close_index - 1>
                    >::type::value;
            };

            // indexの次の兄弟のindexを返すメタ関数．
            template <typename T, size_t index>
            struct NEXTSIBLING;

            template <unsigned long bit_expression, size_t length_, size_t index>
            struct NEXTSIBLING <Paren <bit_expression, length_>, index>
            {
                using _Paren = Paren <bit_expression, length_>;
                using at     = AT <_Paren, index>;
                static constexpr size_t close_index = std::conditional <
                    at::value == 0,
                    FINDCLOSE <_Paren, index>,
                    _identity <index>
                    >::type::value;
                // close_index+1の括弧が開のとき，次の兄弟．
                // close_index+1の括弧が閉のとき，次の兄弟は存在しない．
                static constexpr size_t value = (close_index < length_ && AT <_Paren, close_index + 1>::value == 0) ? close_index + 1 : 0;
            };

            // indexの前の兄弟のindexを返すメタ関数．
            template <typename T, size_t index>
            struct PREVSIBLING;

            template <unsigned long bit_expression, size_t length_, size_t index>
            struct PREVSIBLING <Paren <bit_expression, length_>, index>
            {
                using _Paren = Paren <bit_expression, length_>;
                using at     = AT <_Paren, index>;
                static constexpr size_t open_index = std::conditional <
                    at::value == 0,
                    _identity <index>,
                    FINDOPEN <_Paren, index>
                    >::type::value;
                // open_index-1の括弧が開のとき，前の兄弟は存在しない．
                // open_index-1の括弧が閉のとき，対応する開括弧が前の兄弟．
                static constexpr size_t value = std::conditional <
                    AT <_Paren, open_index - 1>::value == 0,
                    _zero,
                    FINDOPEN <_Paren, open_index - 1>
                    >::type::value;
            };

            // indexの子の数を返すメタ関数．
            template <typename T, size_t now_index, size_t sum>
            struct _CHILDRENNUM;

            template <unsigned long bit_expression, size_t length_, size_t now_open_index, size_t sum>
            struct _CHILDRENNUM <Paren <bit_expression, length_>, now_open_index, sum>
            {
                using _Paren = Paren <bit_expression, length_>;
                // 次の兄弟が存在するとき，再帰．
                static constexpr size_t _next_sibling = NEXTSIBLING <_Paren, now_open_index>::value;
                static constexpr size_t value         = std::conditional <
                    _next_sibling != 0,
                    _CHILDRENNUM <_Paren, _next_sibling, sum + 1>,
                    _identity <sum>
                    >::type::value;
            };

            template <typename T, size_t index>
            struct CHILDRENNUM;

            template <unsigned long bit_expression, size_t length_, size_t index>
            struct CHILDRENNUM <Paren <bit_expression, length_>, index>
            {
                using _Paren = Paren <bit_expression, length_>;
                // FINDCLOSE+1の括弧が開のとき，次の兄弟．
                // FINDCLOSE+1の括弧が閉のとき，次の兄弟は存在しない．
                static constexpr size_t _first_child = FIRSTCHILD <_Paren, index>::value;
                static constexpr size_t value        = std::conditional <
                    _first_child != 0,
                    _CHILDRENNUM <_Paren, _first_child, 1>,
                    _zero
                    >::type::value;
            };

            // indexのノードが先頭から何番目のノードかを返すメタ関数．
            template <typename T, size_t index>
            struct RANK;

            template <unsigned long bit_expression, size_t length_, size_t index>
            struct RANK <Paren <bit_expression, length_>, index>
            {
                using _Paren = Paren <bit_expression, length_>;
                using at     = AT <_Paren, index>;
                static constexpr size_t open_index = std::conditional <
                    at::value == 0,
                    _identity <index>,
                    FINDOPEN <_Paren, index>
                    >::type::value;
                static constexpr size_t value = util::rank0(bit_expression, index, length_);
            };

            // 先頭からorder番目のノードを返すメタ関数．
            template <typename T, size_t order>
            struct SELECT;

            template <unsigned long bit_expression, size_t length_, size_t order>
            struct SELECT <Paren <bit_expression, length_>, order>
            {
                static constexpr size_t value = util::select0(bit_expression, order, length_);
            };

            // Parenかどうかを判定するメタ関数．
            template <typename T>
            struct is_Paren
            {
                static constexpr bool value = false;
            };

            template <unsigned long bit_expression, size_t length_>
            struct is_Paren <Paren <bit_expression, length_> >
            {
                static constexpr bool value = true;
            };

            void test_paren()
            {
                #include <iostream>
                #include <sprout/algorithm/fixed/reverse.hpp>
                #include <sprout/string.hpp>

                constexpr auto   str     = sprout::to_string("(()(()()(())))");
                constexpr auto   rev_str = sprout::fixed::reverse(str);
                constexpr size_t len     = str.size();
                constexpr auto   bs      = util::paren_to_bitseq(str);
                std::cout << bs.to_string() << std::endl;
                std::cout << bs.to_ulong() << std::endl;
                constexpr unsigned long compressed = util::paren_to_bitseq(rev_str).to_ulong();

                using paren = Paren <compressed, len>;
                std::cout << "Paren: " << str.c_str() << std::endl;

                constexpr size_t index = 3;
                std::cout << "AT " << index << ": " << AT <paren, index>::value << std::endl;
                std::cout << "PARENT " << index << ": " << PARENT <paren, index>::value << std::endl;
                std::cout << "FIRSTCHILD " << index << ": " << FIRSTCHILD <paren, index>::value << std::endl;
                std::cout << "NEXTSIBLING " << index << ": " << NEXTSIBLING <paren, index>::value << std::endl;
                std::cout << "RANK0 " << index << ": " << util::rank0(compressed, index, len) << std::endl;
                std::cout << "SELECT0 " << index << ": " << util::select0(compressed, index, len) << std::endl;
            }
        }
    }
}

#endif
