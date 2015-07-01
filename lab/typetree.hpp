#ifndef TYPETREE
#define TYPETREE

#include <boost/mpl/vector.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/find.hpp>
#include "treeshape.hpp"

// 木構造に関する名前空間
namespace tree {
    // メタデータ木に関する名前空間
    namespace typetree {

        // メタデータ木．
        template <class Shape_, class Elements_>
        struct Tree
        {
            // 木の形状におけるノード数と要素の数が一致．
            static_assert(tree::shape::V_SIZE<Shape_>::value == boost::mpl::size<Elements_>::value, "shape and element are not consistent");
            using shape = Shape_;
            using elements = Elements_;
        };

        // 基本クエリ

        // 要素アクセス
        template <class Tree_, size_t index>
        struct AT
        : boost::mpl::at<typename Tree_::element, std::integral_constant<size_t, index>>
        {
        };

        // 検索
        template <class Tree_, typename element>
        struct FIND
        {
            static constexpr size_t _pre_value = boost::mpl::find<typename Tree_::elements, element>::type::pos::value;
            static constexpr size_t value = (_pre_value != boost::mpl::size<typename Tree_::elements>::value) ? _pre_value+1 : 0;
        };

        // 直系であるか判定する関数．
        // 直系のとき，距離を返す．
        // 直系でないとき，0を返す．
        template <class Now_node, class Descendent_node, size_t distance, size_t Now_node_id = Now_node::id>
        struct _DIRECTLINE
        {
            // 遅延評価，最高．
            using CHILDLINE = _DIRECTLINE<typename Now_node::FIRSTCHILD::type, Descendent_node, distance+1>;
            using SIBLINGLINE = _DIRECTLINE<typename Now_node::NEXTSIBLING::type, Descendent_node, distance>;
            static constexpr size_t value = std::conditional<
                Now_node::id == Descendent_node::id,
                std::integral_constant<size_t, distance>,
                typename std::conditional<
                    CHILDLINE::value != 0,
                    CHILDLINE,
                    SIBLINGLINE
                    >::type
                >::type::value;
        };

        template <class Now_node, class Descendent_node, size_t distance>
        struct _DIRECTLINE<Now_node, Descendent_node, distance, 0>
        {
            static constexpr size_t value = 0;
        };

        template <class Tree_, typename Ancestor, typename Descendent>
        struct DIRECTLINE
        {
            // 遅延評価最高．
            static constexpr size_t ancestor_id = FIND<Tree_, Ancestor>::value;
            static constexpr size_t descendent_id = FIND<Tree_, Descendent>::value;
            using ancestor_node =  tree::shape::AT_AS_GRAPH<Tree_, ancestor_id>;
            using descendent_node =  tree::shape::AT_AS_GRAPH<Tree_, descendent_id>;
            static constexpr size_t value = std::conditional<
                ancestor_id != 0 && descendent_id != 0,
                _DIRECTLINE<typename ancestor_node::type, typename descendent_node::type, 0>,
                std::integral_constant<size_t, 0>
                >::type::value;
        };

        void test_typetree()
        {
            struct Human {};
            struct My_daughter {};
            struct Akari {};
            struct Other {};
            struct Normal {};
            struct Otaku {};

            std::cout << util::Repeat("-", 20) << std::endl;
            std::cout << "Typetree test" << std::endl;
            constexpr auto str = sprout::to_string("((())(()()))");
            std::cout << str.c_str() << std::endl;
            constexpr auto rev_str = sprout::fixed::reverse(str);
            // Paren型．
            using paren = tree::shape::paren::Paren <util::paren_to_bitseq(rev_str).to_ulong(), str.size()>;
            // Tree型．
            using tree = tree::shape::Tree <paren>;
            // メタシーケンス．
            using elements = boost::mpl::vector<Human, My_daughter, Akari, Other, Normal, Otaku>;
            // メタデータ木．
            using meta_tree = Tree<tree, elements>;

            std::cout << "FIND Akari: " << FIND<meta_tree, Akari>::value << std::endl;
            std::cout << "FIND int: " << FIND<meta_tree, int>::value << std::endl;
            std::cout << boost::mpl::find<elements, Akari>::type::pos::value << std::endl;
            std::cout << boost::mpl::find<elements, int>::type::pos::value << std::endl;
            std::cout << boost::mpl::size<elements>::value << std::endl;
            std::cout << "DIRECTLINE Human, Otaku: " << DIRECTLINE<meta_tree, Human, Otaku>::value << std::endl;
            std::cout << "DIRECTLINE Akari, Otaku: " << DIRECTLINE<meta_tree, Akari, Otaku>::value << std::endl;
            std::cout << "DIRECTLINE int, Otaku: " << DIRECTLINE<meta_tree, Akari, Otaku>::value << std::endl;

            std::cout << util::Repeat("-", 20) << std::endl;
        }
    }
}

#endif
