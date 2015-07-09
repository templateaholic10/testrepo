#ifndef TYPETREE
#define TYPETREE

#include <boost/mpl/vector.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/find.hpp>
#include "util.hpp"
#include "graph_config.hpp"
#include "treeshape.hpp"
#include "metadata.hpp"
#include "dot_outer.hpp"

// 木構造に関する名前空間
namespace tree {
    // メタデータ木に関する名前空間
    namespace typetree {
        // メタデータ木．
        // 同じメタデータが複数回現れることはない．
        template <class Shape_, class Elements_>
        struct Tree
        {
            // 木の形状におけるノード数と要素の数が一致．
            static_assert(tree::shape::V_SIZE <Shape_>::value == boost::mpl::size <Elements_>::value, "shape and element are not consistent");
            using Shape    = Shape_;
            using Elements = Elements_;
            static constexpr size_t v_size = tree::shape::V_SIZE <Shape_>::value;
        };

        // 基本クエリ

        // 要素アクセス
        template <class Tree_, size_t index>
        struct AT
            : boost::mpl::at <typename Tree_::element, std::integral_constant <size_t, index> >
        {
        };

        // 検索
        template <class Tree_, typename Element>
        struct FIND
        {
            static constexpr size_t _pre_value = boost::mpl::find <typename Tree_::Elements, Element>::type::pos::value;
            static constexpr size_t value      = (_pre_value != boost::mpl::size <typename Tree_::Elements>::value) ? _pre_value + 1 : 0;
        };

        // 直系であるか判定する関数．
        // 直系のとき，距離を返す．
        // 直系でないとき，0を返す．
        // 自分自身の場合は直系と扱わない．
        template <class Now_node, class Descendent_node, size_t distance, size_t Now_node_id = Now_node::id>
        struct _DIRECTLINE
        {
            // 遅延評価，最高．
            using CHILDLINE   = _DIRECTLINE <typename Now_node::FIRSTCHILD::type, Descendent_node, distance + 1>;
            using SIBLINGLINE = _DIRECTLINE <typename Now_node::NEXTSIBLING::type, Descendent_node, distance>;
            static constexpr size_t value = std::conditional <
                Now_node::id == Descendent_node::id,
                std::integral_constant <size_t, distance>,
                typename std::conditional <
                    CHILDLINE::value != 0,
                    CHILDLINE,
                    SIBLINGLINE
                    >::type
                >::type::value;
        };

        template <class Now_node, class Descendent_node, size_t distance>
        struct _DIRECTLINE <Now_node, Descendent_node, distance, 0>
        {
            static constexpr size_t value = 0;
        };

        template <class Tree_, typename Ancestor, typename Descendent>
        struct DIRECTLINE
        {
            // 遅延評価最高．
            static constexpr size_t ancestor_id   = FIND <Tree_, Ancestor>::value;
            static constexpr size_t descendent_id = FIND <Tree_, Descendent>::value;
            using ancestor_node   =  tree::shape::AT <typename Tree_::Shape, ancestor_id>;
            using descendent_node =  tree::shape::AT <typename Tree_::Shape, descendent_id>;
            static constexpr size_t value = std::conditional <
                ancestor_id != 0 && descendent_id != 0,
                _DIRECTLINE <typename ancestor_node::type, typename descendent_node::type, 0>,
                std::integral_constant <size_t, 0>
                >::type::value;
        };

        // ノードに対応するメタデータの出力専用のオブジェクト．
        template <class Node_, class Elements_>
        class Out;

        template <class Paren_, size_t index, class Elements_>
        class Out <tree::shape::Node <Paren_, index>, Elements_>
        {
        private:
            using _Node = tree::shape::Node <Paren_, index>;
        public:
            constexpr Out()
            {
            }

            static std::string to_string()
            {
                return util::typename_of <typename boost::mpl::at <Elements_, std::integral_constant <size_t, _Node::id - 1> >::type>();
            }
        };

        template <class Node_, class Elements_>
        std::ostream&operator<<(std::ostream &os, const Out <Node_, Elements_> &out)
        {
            os << out.to_string();

            return os;
        }

        // 兄弟の出力専用のオブジェクト．
        template <class Node_, class Elements_>
        class Sibling_Out;

        template <class Paren_, size_t index, class Elements_>
        class Sibling_Out <tree::shape::Node <Paren_, index>, Elements_>
        {
        private:
            using _Node = tree::shape::Node <Paren_, index>;
        public:
            constexpr Sibling_Out(const std::string &delim_=", ")
                : delim(delim_)
            {
            }

            static std::string to_string(const std::string &delim_=", ")
            {
                using next_sibling = typename _Node::NEXTSIBLING;
                std::string result = "";
                if (next_sibling::type::id != 0) {
                    result += Out <typename next_sibling::type, Elements_>::to_string() + delim_ + Sibling_Out <typename next_sibling::type, Elements_>::to_string();
                }

                return std::move(result);
            }

            const std::string delim;
        };

        template <class Node_, class Elements_>
        std::ostream&operator<<(std::ostream &os, const Sibling_Out <Node_, Elements_> &out)
        {
            os << out.to_string(out.delim);

            return os;
        }

        // 子の出力専用のオブジェクト．
        template <class Node_, class Elements_>
        class Children_Out;

        template <class Paren_, size_t index, class Elements_>
        class Children_Out <tree::shape::Node <Paren_, index>, Elements_>
        {
        private:
            using _Node = tree::shape::Node <Paren_, index>;
        public:
            constexpr Children_Out(const std::string &delim_=", ")
                : delim(delim_)
            {
            }

            static std::string to_string(const std::string &delim_=", ")
            {
                using first_child = typename _Node::FIRSTCHILD;
                std::string result = "";
                if (first_child::type::id != 0) {
                    result += Out <typename first_child::type, Elements_>::to_string() + delim_ + Sibling_Out <typename first_child::type, Elements_>::to_string(delim_);
                }

                return std::move(result);
            }

            const std::string delim;
        };

        template <class Node_, class Elements_>
        std::ostream&operator<<(std::ostream &os, const Children_Out <Node_, Elements_> &out)
        {
            os << out.to_string(out.delim);

            return os;
        }

        // 兄弟の出力専用のオブジェクト．
        template <class Node_, class Elements_>
        class List_Out;

        template <class Paren_, size_t index, class Elements_>
        class List_Out <tree::shape::Node <Paren_, index>, Elements_>
        {
        private:
            using _Node = tree::shape::Node <Paren_, index>;
        public:
            List_Out(const std::string &edge_marker_=" -> ", const std::string &delim_=", ")
                : edge_marker(edge_marker_), delim(delim_)
            {
            }

            static std::string to_string(const std::string &edge_marker_=" -> ", const std::string &delim_=", ")
            {
                std::string result = Out <_Node, Elements_>::to_string() + edge_marker_ + Children_Out <_Node, Elements_>::to_string(delim_);

                return std::move(result);
            }

            const std::string edge_marker;
            const std::string delim;
        };

        template <class Node_, class Elements_>
        std::ostream&operator<<(std::ostream &os, const List_Out <Node_, Elements_> &out)
        {
            os << out.to_string(out.edge_marker, out.delim);

            return os;
        }

        // 兄弟の出力専用のオブジェクト．
        template <class Tree_, size_t index>
        class _Tree_Out;

        // 再帰を用いるので停止条件に注意．
        template <class Shape_, class Elements_, size_t index>
        class _Tree_Out <Tree <Shape_, Elements_>, index>
        {
        private:
            using _Tree = Tree <Shape_, Elements_>;
            static constexpr size_t next_index = (1 <= index && index < _Tree::v_size) ? index + 1 : 0;
        public:
            constexpr _Tree_Out(const std::string &edge_marker_=" -> ", const std::string &delim_=", ")
                : edge_marker(edge_marker_), delim(delim_)
            {
            }

            static std::string to_string(const std::string &edge_marker_=" -> ", const std::string &delim_=", ")
            {
                using this_node = typename tree::shape::AT <typename _Tree::Shape, index>;
                std::string result = List_Out <typename this_node::type, Elements_>::to_string(edge_marker_, delim_) + '\n' + _Tree_Out <_Tree, next_index>::to_string(edge_marker_, delim_);

                return std::move(result);
            }

            const std::string edge_marker;
            const std::string delim;
        };

        // 再帰を用いるので停止条件に注意．
        template <class Shape_, class Elements_>
        class _Tree_Out <Tree <Shape_, Elements_>, 0>
        {
        private:
            using _Tree = Tree <Shape_, Elements_>;
        public:
            constexpr _Tree_Out(const std::string &edge_marker_=" -> ", const std::string &delim_=", ")
                : edge_marker(edge_marker_), delim(delim_)
            {
            }

            static std::string to_string(const std::string &edge_marker_=" -> ", const std::string &delim_=", ")
            {
                std::string result = "";

                return std::move(result);
            }

            const std::string edge_marker;
            const std::string delim;
        };

        // 兄弟の出力専用のオブジェクト．
        template <class Tree_>
        class Tree_Out;

        template <class Shape_, class Elements_>
        class Tree_Out <Tree <Shape_, Elements_> >
        {
        private:
            using _Tree = Tree <Shape_, Elements_>;
        public:
            constexpr Tree_Out(const std::string &edge_marker_=" -> ", const std::string &delim_=", ")
                : edge_marker(edge_marker_), delim(delim_)
            {
            }

            static std::string to_string(const std::string &edge_marker_=" -> ", const std::string &delim_=", ")
            {
                std::string result = _Tree_Out <_Tree, 1>::to_string(edge_marker_, delim_);

                return std::move(result);
            }

            static constexpr graph::Expression expression = graph::Expression::adjacency_list;
            const std::string                  edge_marker;
            const std::string                  delim;
        };

        template <class Shape_, class Elements_>
        std::ostream&operator<<(std::ostream &os, const Tree_Out <Tree <Shape_, Elements_> > &out)
        {
            os << out.to_string(out.edge_marker, out.delim);

            return os;
        }

        void test_typetree()
        {
            std::cout << util::Repeat("-", 20) << std::endl;
            std::cout << "Typetree test" << std::endl;

            std::cout << "ex1. オブジェクト型の従属関係を定義する" << std::endl;
            constexpr auto str1 = animal::p_seq;
            std::cout << str1.c_str() << std::endl;
            // Paren型．
            using _Paren1 = tree::shape::paren::Paren <util::paren_to_bitseq(sprout::fixed::reverse(str1)).to_ulong(), str1.size()>;
            // Tree型．
            using _TreeShape1 = tree::shape::TreeShape <_Paren1>;
            // メタデータ木．
            using _Meta_Tree1 = Tree <_TreeShape1, animal::Elements>;

            std::cout << "[adjucency list expression]" << std::endl;
            std::cout << Tree_Out <_Meta_Tree1>() << std::endl;

            std::cout << "FIND(animal::Akari): " << FIND <_Meta_Tree1, animal::Akari>::value << std::endl;
            std::cout << "FIND(int): " << FIND <_Meta_Tree1, int>::value << std::endl;
            std::cout << "DIRECTLINE(animal::Animal, animal::Man): " << DIRECTLINE <_Meta_Tree1, animal::Animal, animal::Man>::value << std::endl;
            std::cout << "DIRECTLINE(animal::Human, animal::Otaku): " << DIRECTLINE <_Meta_Tree1, animal::Human, animal::Otaku>::value << std::endl;

            std::cout << std::endl;

            std::cout << "ex2. 派生型の関係を参照する" << std::endl;
            constexpr auto str2 = derivation::p_seq;
            std::cout << str2.c_str() << std::endl;
            // Paren型．
            using _Paren2 = tree::shape::paren::Paren <util::paren_to_bitseq(sprout::fixed::reverse(str2)).to_ulong(), str2.size()>;
            // Tree型．
            using _TreeShape2 = tree::shape::TreeShape <_Paren2>;
            // メタデータ木．
            using _Meta_Tree2 = Tree <_TreeShape2, derivation::Elements>;

            std::cout << "[adjucency list expression]" << std::endl;
            std::cout << Tree_Out <_Meta_Tree2>() << std::endl;

            std::cout << "FIND(int * const): " << FIND <_Meta_Tree2, int *const>::value << std::endl;
            std::cout << "FIND(const int * const): " << FIND <_Meta_Tree2, const int *const>::value << std::endl;
            std::cout << "DIRECTLINE(int *, int * const *): " << DIRECTLINE <_Meta_Tree2, int *, int *const *>::value << std::endl;
            std::cout << "DIRECTLINE(int *, int (*)[]): " << DIRECTLINE <_Meta_Tree2, int *, int(*)[]>::value << std::endl;

            std::cout << util::Repeat("-", 20) << std::endl;
        }

        void test_typetree_to_dot()
        {
            constexpr auto str1 = animal::p_seq;
            // Paren型．
            using _Paren1 = tree::shape::paren::Paren <util::paren_to_bitseq(sprout::fixed::reverse(str1)).to_ulong(), str1.size()>;
            // Tree型．
            using _TreeShape1 = tree::shape::TreeShape <_Paren1>;
            // メタデータ木．
            using _Meta_Tree1 = Tree <_TreeShape1, animal::Elements>;

            std::cout << Tree_Out <_Meta_Tree1>() << std::endl;
            std::cout << std::endl;
            std::cout << (dot::Dot("typetree") << Tree_Out <_Meta_Tree1>()) << std::endl;
        }
    }
}

#endif
