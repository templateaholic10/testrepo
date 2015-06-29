#ifndef COMPILE_TREE
#define COMPILE_TREE

#include <iostream>
#include <boost/mpl/bitwise.hpp>
#include <sprout/numeric/iota.hpp>
#include <sprout/algorithm/fixed/reverse.hpp>
#include "util.hpp"

namespace paren {
    // インデックスは1-origin．
    // インデックスを返す関数が未定義であるとき，0を返す．

    // メタ関数が未定義であることを示す返り値．
    struct None
    {
    };

    // 木形の括弧表現．
    template <unsigned long bit_expression, size_t length_>
    struct Paren
    {
        // 括弧の釣り合いがとれているかチェック．
        static_assert(2 * util::bit_sum2(bit_expression) == length_, "parentheses are not balanced.");
        static constexpr unsigned long value  = bit_expression;
        static constexpr size_t        length = length_;
    };

    // 括弧列がindexにおいて開であるか閉であるかを返すメタ関数．
    // indexは1-originとする．
    // indexが[1, length]以外のとき必ず0．
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

    struct _zero
    {
        static constexpr size_t value = 0;
    };

    template <size_t index>
    struct _idendity
    {
        static constexpr size_t value = index;
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
        // AT<Paren<bit_expression, length_>, index>()
        // {
        //     std::cout << "paren: " << std::bitset<length_>(_Paren::value).to_string() << std::endl;
        //     std::cout << "index: " << index << std::endl;
        //     std::cout << "shifted: " << std::bitset<length_>(_Paren::value >> (_Paren::length - index)) << std::endl;
        //     std::cout << "bit on index: " << std::bitset<length_>((_Paren::value >> (_Paren::length - index)) & 1) << std::endl;
        // }
    };

    // 開括弧のインデックスに対応する閉括弧のインデックスを返すメタ関数．
    // indexは1-originとする．
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

    // 開括弧に対応する閉括弧のインデックスを返すメタ関数．
    template <typename T, size_t open_index>
    struct FINDCLOSE;

    template <unsigned long bit_expression, size_t length_, size_t open_index>
    struct FINDCLOSE <Paren <bit_expression, length_>, open_index>
    {
        using _Paren = Paren <bit_expression, length_>;
        static_assert(AT <_Paren, open_index>::value == 0, "not open at index.");
        static constexpr size_t value = _FINDCLOSE <_Paren, open_index, 1>::value;
    };

    // 閉括弧のインデックスに対応する開括弧のインデックスを返すメタ関数．
    // indexは1-originとする．
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

    // 閉括弧に対応する開括弧のインデックスを返すメタ関数．
    template <typename T, size_t close_index>
    struct FINDOPEN;

    template <unsigned long bit_expression, size_t length_, size_t close_index>
    struct FINDOPEN <Paren <bit_expression, length_>, close_index>
    {
        using _Paren = Paren <bit_expression, length_>;
        static_assert(AT <_Paren, close_index>::value == 1, "not close at index.");
        static constexpr size_t value = _FINDOPEN <_Paren, close_index, 1>::value;
    };

    // 最もきつく括る括弧を返すメタ関数．
    template <typename T, size_t index>
    struct ENCLOSE;

    template <unsigned long bit_expression, size_t length_, size_t index>
    struct ENCLOSE <Paren <bit_expression, length_>, index>
    {
        using _Paren = Paren <bit_expression, length_>;
        using at     = AT <_Paren, index>;
        static constexpr size_t open_index = std::conditional <
            at::value == 0,
            _idendity <index>,
            FINDOPEN <_Paren, index>
            >::type::value;
        // 括弧が開になるまで繰り返す．
        static constexpr size_t value = std::conditional <
                                        0 < open_index && open_index <= length_,
        typename std::conditional <
            AT <_Paren, open_index - 1>::value == 0,
            _idendity <open_index - 1>,
            ENCLOSE <_Paren, open_index - 1>
            >::type,
        _zero >
        ::type::value;
    };

    // エイリアステンプレート．
    template <typename T, size_t index>
    using PARENT = ENCLOSE <T, index>;

    // 括弧表現における最初の子を返すメタ関数．
    template <typename T, size_t index>
    struct FIRSTCHILD;

    template <unsigned long bit_expression, size_t length_, size_t index>
    struct FIRSTCHILD <Paren <bit_expression, length_>, index>
    {
        using _Paren = Paren <bit_expression, length_>;
        using at     = AT <_Paren, index>;
        static constexpr size_t open_index = std::conditional <
            at::value == 0,
            _idendity <index>,
            FINDOPEN <_Paren, index>
            >::type::value;
        // open_index+1の括弧が開のとき，最初の子．
        // open_index+1の括弧が閉のとき，子は存在しない．
        static constexpr size_t value = (AT <_Paren, open_index + 1>::value == 0) ? open_index + 1 : 0;
    };

    // 括弧表現における次の兄弟を返すメタ関数．
    template <typename T, size_t index>
    struct NEXTSIBLING;

    template <unsigned long bit_expression, size_t length_, size_t index>
    struct NEXTSIBLING <Paren <bit_expression, length_>, index>
    {
        using _Paren = Paren <bit_expression, length_>;
        using at     = AT <_Paren, index>;
        static constexpr size_t open_index = std::conditional <
            at::value == 0,
            _idendity <index>,
            FINDOPEN <_Paren, index>
            >::type::value;
        // FINDCLOSE+1の括弧が開のとき，次の兄弟．
        // FINDCLOSE+1の括弧が閉のとき，次の兄弟は存在しない．
        static constexpr size_t _find_close = FINDCLOSE <_Paren, open_index>::value;
        static constexpr size_t value       = (_find_close < length_ && AT <_Paren, _find_close + 1>::value == 0) ? _find_close + 1 : 0;
    };

    void test_paren()
    {
        constexpr auto   str     = sprout::to_string("(()(()()(())))");
        constexpr auto   rev_str = sprout::fixed::reverse(str);
        constexpr size_t len     = str.size();
        constexpr auto   bs      = util::paren_to_bitseq(str);
        std::cout << bs.to_string() << std::endl;
        std::cout << bs.to_ulong() << std::endl;
        constexpr unsigned long compressed = util::paren_to_bitseq(rev_str).to_ulong();

        using paren = Paren <compressed, len>;
        std::cout << "Paren: " << str.c_str() << std::endl;

        // std::cout << util::bit_sum2(compressed) << std::endl;
        //
        std::cout << AT <paren, 1>::value << std::endl;
        std::cout << AT <paren, 2>::value << std::endl;
        std::cout << AT <paren, 3>::value << std::endl;
        std::cout << AT <paren, 4>::value << std::endl;
        std::cout << AT <paren, 5>::value << std::endl;
        std::cout << AT <paren, 6>::value << std::endl;
        std::cout << AT <paren, 7>::value << std::endl;
        std::cout << AT <paren, 8>::value << std::endl;
        std::cout << AT <paren, 9>::value << std::endl;
        std::cout << AT <paren, 10>::value << std::endl;
        std::cout << AT <paren, 11>::value << std::endl;
        std::cout << AT <paren, 12>::value << std::endl;
        std::cout << AT <paren, 13>::value << std::endl;
        std::cout << AT <paren, 14>::value << std::endl;

        constexpr size_t index = 1;
        std::cout << "AT " << index << ": " << AT <paren, index>::value << std::endl;
        std::cout << "PARENT " << index << ": " << PARENT <paren, index>::value << std::endl;
        std::cout << "FIRSTCHILD " << index << ": " << FIRSTCHILD <paren, index>::value << std::endl;
        std::cout << "NEXTSIBLING " << index << ": " << NEXTSIBLING <paren, index>::value << std::endl;
        std::cout << AT <paren, 15>::value << std::endl;

        constexpr unsigned long value = (paren::value >> (paren::length - 1)) & 1;
    }
}

namespace tree {
    // 木の形のみの情報を持つメタデータ．
    namespace shape {
        // 木形のグラフ表現．

        // 木グラフにおける存在しないノードのプレースホルダ．
        struct Node_none
        {
        };

        // 木グラフのノード．
        // indexは括弧表現における開括弧のインデックス．
        template <typename T, size_t index>
        struct Node;

        template <unsigned long bit_expression, size_t length_, size_t index>
        struct Node <paren::Paren <bit_expression, length_>, index>
        {
            using _Paren = paren::Paren <bit_expression, length_>;
            static constexpr size_t id = index;
            struct PARENT
            {
                static constexpr size_t first_child = paren::FIRSTCHILD <_Paren, index>::value;
                using type = typename std::conditional <
                          first_child != 0,
                          // FIRSTCHILDが存在するとき
                          Node <_Paren, first_child>,
                          // 存在しないとき
                          Node_none
                          >::type;
            };

            // DFSの順．
            struct FIRSTCHILD
            {
                static constexpr size_t _first_child = paren::FIRSTCHILD <_Paren, index>::value;
                using type = typename std::conditional <
                          _first_child != 0,
                          // FIRSTCHILDが存在するとき
                          Node <_Paren, _first_child>,
                          // 存在しないとき
                          Node_none
                          >::type;
            };

            struct NEXTSIBLING
            {
                static constexpr size_t _next_sibling = paren::NEXTSIBLING <_Paren, index>::value;
                using type = typename std::conditional <
                          _next_sibling != 0,
                          // NEXTSIBLINGが存在するとき
                          Node <_Paren, _next_sibling>,
                          // 存在しないとき
                          Node_none
                          >::type;
            };
        };

        // 木形のグラフ表現．
        template <typename T>
        struct Graph;

        template <unsigned long bit_expression, size_t length_>
        struct Graph <paren::Paren <bit_expression, length_> >
        {
            using _Paren = paren::Paren <bit_expression, length_>;
            using root   = Node <_Paren, 1>;
        };

        template <class T>
        struct Dot;

        template <class Paren_>
        struct Dot<Graph<Paren_>>
        {
        };

        template <class Graph_>
        std::ostream&operator<<(std::ostream &os, const Dot<Graph_> &dot)
        {
            return os;
        }

        void test_graph()
        {
            std::cout << util::Repeat("-", 20) << std::endl;
            std::cout << "Tree representations test" << std::endl;
            constexpr auto str = sprout::to_string("(()(()()(())))");
            std::cout << str.c_str() << std::endl;
            constexpr auto rev_str = sprout::fixed::reverse(str);
            // Paren型．
            using paren = paren::Paren <util::paren_to_bitseq(rev_str).to_ulong(), str.size()>;
            // Graph型．
            using graph = Graph <paren>;
            std::cout << util::Repeat("-", 20) << std::endl;
        }
    }
}

#endif
