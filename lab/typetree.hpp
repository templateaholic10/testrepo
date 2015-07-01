#ifndef TYPETREE
#define TYPETREE

#include <boost/mpl/vector.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/find.hpp>
#include "treeshape_paren.hpp"

// 木構造に関する名前空間
namespace tree {
    // メタデータ木に関する名前空間
    namespace typetree {

        // メタデータ木．
        template <class Shape_, class Elements_>
        struct Tree
        {
            // 木の形状におけるノード数と要素の数が一致．
            static_assert(Shape_::V_size == boost::mpl::size<Elements_>::value, "shape and element are not consistent");
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
            static constexpr size_t value = boost::mpl::find<typename Tree_::elements, element>::type::pos::value + 1;
        };

        // 直系であるか判定する関数．
        // 直系のとき，距離を返す．
        // 直系でないとき，0を返す．
        template <class Now_node, class Descendent_node, size_t distance, size_t Now_node_id = Now_node::id, size_t Descendent_node_id = Descendent_node::id>
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

        template <class Now_node, class Descendent_node, size_t distance, size_t Descendent_node_id>
        struct _DIRECTLINE<Now_node, Descendent_node, distance, 0, Descendent_node_id>
        {
            static constexpr size_t value = 0;
        };

        template <class Tree_, typename Ancestor, typename Descendent>
        struct DIRECTLINE
        {
            static constexpr size_t value =
        };
    }
}

#endif
