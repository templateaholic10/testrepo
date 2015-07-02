#ifndef TYPETREE
#define TYPETREE

#include <boost/mpl/vector.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/find.hpp>
#include "treeshape.hpp"

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
            using ancestor_node   =  tree::shape::AT_AS_GRAPH <typename Tree_::Shape, ancestor_id>;
            using descendent_node =  tree::shape::AT_AS_GRAPH <typename Tree_::Shape, descendent_id>;
            static constexpr size_t value = std::conditional <
                ancestor_id != 0 && descendent_id != 0,
                _DIRECTLINE <typename ancestor_node::type, typename descendent_node::type, 0>,
                std::integral_constant <size_t, 0>
                >::type::value;
        };

        // ノードに対応するメタデータの出力専用のオブジェクト．
        template <class Node_, class Elements_, class Ignored = void>
        class Out;

        // SFINAEによりElementsはランダムアクセス可能なメタシーケンスに限定．
        template <class Paren_, size_t index, class Elements_>
        class Out <tree::shape::Node <Paren_, index>, Elements_, typename boost::mpl::at <Elements_, std::integral_constant <size_t, 0> >::type>
        {
        private:
            using _Node = tree::shape::Node <Paren_, index>;
        public:
            constexpr Out()
            {
            }

            static std::string to_string()
            {
                return util::typename_of <boost::mpl::at <Elements_, std::integral_constant <size_t, _Node::graph_id> > >();
            }
        };

        template <class Node_, class Elements_>
        std::ostream&operator<<(std::ostream &os, const Out <Node_, Elements_> &out)
        {
            os << out.to_string();

            return os;
        }

        // 兄弟の出力専用のオブジェクト．
        template <class Node_, class Elements_, class Ignored = void>
        class Sibling_Out;

        // SFINAEによりElementsはランダムアクセス可能なメタシーケンスに限定．
        template <class Paren_, size_t index, class Elements_>
        class Sibling_Out <tree::shape::Node <Paren_, index>, Elements_, typename boost::mpl::at <Elements_, std::integral_constant <size_t, 0> >::type>
        {
        private:
            using _Node = tree::shape::Node <Paren_, index>;
        public:
            constexpr Sibling_Out(const char delim=',')
                : _delim(delim)
            {
            }

            static std::string to_string(const char delim=' ')
            {
                using next_sibling = typename _Node::NEXTSIBLING;
                std::string result = "";
                if (next_sibling::type::graph_id != 0) {
                    result += Out <typename next_sibling::type, Elements_>::to_string() + delim + Sibling_Out <typename next_sibling::type, Elements_>::to_string();
                }

                return std::move(result);
            }

            const char _delim;
        };

        template <class Node_, class Elements_>
        std::ostream&operator<<(std::ostream &os, const Sibling_Out <Node_, Elements_> &out)
        {
            os << out.to_string(out._delim);

            return os;
        }

        // 子の出力専用のオブジェクト．
        template <class Node_, class Elements_, class Ignored = void>
        class Children_Out;

        // SFINAEによりElementsはランダムアクセス可能なメタシーケンスに限定．
        template <class Paren_, size_t index, class Elements_>
        class Children_Out <tree::shape::Node <Paren_, index>, Elements_, typename boost::mpl::at <Elements_, std::integral_constant <size_t, 0> >::type>
        {
        private:
            using _Node = tree::shape::Node <Paren_, index>;
        public:
            constexpr Children_Out(const char delim=',')
                : _delim(delim)
            {
            }

            static std::string to_string(const char delim=' ')
            {
                using first_child = typename _Node::FIRSTCHILD;
                std::string result = "";
                if (first_child::type::graph_id != 0) {
                    result += Out <typename first_child::type, Elements_>::to_string() + delim + Sibling_Out <typename first_child::type, Elements_>::to_string();
                }

                return std::move(result);
            }

            const char _delim;
        };

        template <class Node_, class Elements_>
        std::ostream&operator<<(std::ostream &os, const Children_Out <Node_, Elements_> &out)
        {
            os << out.to_string(out._delim);

            return os;
        }

        // 兄弟の出力専用のオブジェクト．
        template <class Node_, class Elements_, class Ignored = int>
        class List_Out;

        // SFINAEによりElementsはランダムアクセス可能なメタシーケンスに限定．
        template <class Paren_, size_t index, class Elements_>
        class List_Out <tree::shape::Node <Paren_, index>, Elements_, typename boost::mpl::at <Elements_, std::integral_constant <size_t, 0> >::type>
        {
        private:
            using _Node = tree::shape::Node <Paren_, index>;
        public:
            List_Out(const std::string &edge_marker=" -> ", const char delim=',')
                : _edge_marker(edge_marker), _delim(delim)
            {
            }

            static std::string to_string(const std::string &edge_marker=" -> ", const char delim=' ')
            {
                using first_child = typename _Node::FIRSTCHILD;
                std::string result = Out <_Node, Elements_>::to_string() + edge_marker + Children_Out <typename first_child::type, Elements_>::to_string();

                return std::move(result);
            }

            const std::string _edge_marker;
            const char        _delim;
        };

        template <class Node_, class Elements_>
        std::ostream&operator<<(std::ostream &os, const List_Out <Node_, Elements_> &out)
        {
            os << out.to_string(out._edge_marker, out._delim);

            return os;
        }

        // 兄弟の出力専用のオブジェクト．
        template <class Tree_, size_t index>
        class _Tree_Out;

        // 再帰を用いるので停止条件に注意．
        // SFINAEによりElementsはランダムアクセス可能なメタシーケンスに限定．
        template <class Shape_, class Elements_, size_t index>
        class _Tree_Out <Tree <Shape_, Elements_>, index >
        {
        private:
            using _Tree = Tree <Shape_, Elements_>;
        public:
            constexpr _Tree_Out(const std::string &edge_marker=" -> ", const char delim=',')
                : _edge_marker(edge_marker), _delim(delim)
            {
            }

            static std::string to_string(const std::string &edge_marker=" -> ", const char delim=',')
            {
                using this_node = typename tree::shape::AT_AS_GRAPH<typename _Tree::Shape, index>;
                std::string result = "";
                if (this_node::type::graph_id <= _Tree::v_size) {
                    result += List_Out <typename this_node::type, Elements_>::to_string(edge_marker, delim) + '\n' + _Tree_Out<_Tree, index+1>::to_string(edge_marker, delim);
                }

                return std::move(result);
            }

            const std::string _edge_marker;
            const char        _delim;
        };

        // 兄弟の出力専用のオブジェクト．
        template <class Tree_>
        class Tree_Out;

        // SFINAEによりElementsはランダムアクセス可能なメタシーケンスに限定．
        template <class Shape_, class Elements_>
        class Tree_Out <Tree <Shape_, Elements_> >
        {
        private:
            using _Tree = Tree <Shape_, Elements_>;
        public:
            constexpr Tree_Out(const std::string &edge_marker=" -> ", const char delim=',')
                : _edge_marker(edge_marker), _delim(delim)
            {
            }

            static std::string to_string(const std::string &edge_marker=" -> ", const char delim=',')
            {
                std::string result = _Tree_Out<_Tree, 1>::to_string(edge_marker, delim);

                return std::move(result);
            }

            const std::string _edge_marker;
            const char        _delim;
        };

        template <class Shape_, class Elements_>
        std::ostream&operator<<(std::ostream &os, const Tree_Out <Tree <Shape_, Elements_> > &out)
        {
            os << out.to_string(out._edge_marker, out._delim);

            return os;
        }

        void test_typetree()
        {
            struct Human { };

            struct My_daughter { };

            struct Akari { };

            struct Other { };

            struct Normal { };

            struct Otaku { };

            std::cout << util::Repeat("-", 20) << std::endl;
            std::cout << "Typetree test" << std::endl;
            constexpr auto str = sprout::to_string("((())(()()))");
            std::cout << str.c_str() << std::endl;
            constexpr auto rev_str = sprout::fixed::reverse(str);
            // Paren型．
            using _Paren = tree::shape::paren::Paren <util::paren_to_bitseq(rev_str).to_ulong(), str.size()>;
            // Tree型．
            using _Tree = tree::shape::Tree <_Paren>;
            // メタシーケンス．
            using _Elements = boost::mpl::vector <Human, My_daughter, Akari, Other, Normal, Otaku>;
            // メタデータ木．
            using _Meta_Tree = Tree <_Tree, _Elements>;

            std::cout << "[adjucency list expression]" << std::endl;
            std::cout << Tree_Out<_Meta_Tree>(" | ", ':') << std::endl;
            using hoge = boost::mpl::at <_Elements, std::integral_constant <size_t, 0> >::type;

            std::cout << "FIND Akari: " << FIND <_Meta_Tree, Akari>::value << std::endl;
            std::cout << "FIND int: " << FIND <_Meta_Tree, int>::value << std::endl;
            std::cout << boost::mpl::find <_Elements, Akari>::type::pos::value << std::endl;
            std::cout << boost::mpl::find <_Elements, int>::type::pos::value << std::endl;
            std::cout << boost::mpl::size <_Elements>::value << std::endl;
            std::cout << "DIRECTLINE Human, Otaku: " << DIRECTLINE <_Meta_Tree, Human, Otaku>::value << std::endl;
            std::cout << "DIRECTLINE Akari, Otaku: " << DIRECTLINE <_Meta_Tree, Akari, Otaku>::value << std::endl;
            std::cout << "DIRECTLINE int, Otaku: " << DIRECTLINE <_Meta_Tree, Akari, Otaku>::value << std::endl;

            std::cout << util::Repeat("-", 20) << std::endl;
        }
    }
}

#endif
