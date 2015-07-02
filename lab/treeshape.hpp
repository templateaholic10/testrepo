#ifndef TREE_SHAPE
#define TREE_SHAPE

#include <iostream>
#include <sprout/numeric/iota.hpp>
#include <sprout/algorithm/fixed/reverse.hpp>
#include "util.hpp"
#include "treeshape_paren.hpp"

// 木構造に関する空間
namespace tree {
    // 木構造の形状に関する名前空間
    namespace shape {
        // 木構造のノード．
        // Nodeはメタデータであり，複数のメタ関数をローカルな構造体として含む．
        template <class Paren_, size_t index, class Ignored = void>
        struct Node
        {
            static constexpr size_t id       = index;
            static constexpr size_t graph_id = tree::shape::paren::RANK <Paren_, index>::value;
            struct PARENT
            {
                using type = Node <Paren_, paren::PARENT <Paren_, index>::value>;
            };

            struct FIRSTCHILD
            {
                using type = Node <Paren_, paren::FIRSTCHILD <Paren_, index>::value>;
            };

            struct LASTCHILD
            {
                using type = Node <Paren_, paren::LASTCHILD <Paren_, index>::value>;
            };

            struct NEXTSIBLING
            {
                using type = Node <Paren_, paren::NEXTSIBLING <Paren_, index>::value>;
            };

            struct PREVSIBLING
            {
                using type = Node <Paren_, paren::PREVSIBLING <Paren_, index>::value>;
            };

            struct CHILDRENNUM
            {
                static constexpr size_t value = paren::CHILDRENNUM <Paren_, index>::value;
            };
        };

        // indexがアクセス範囲外の時の特殊化．
        template <class Paren_, size_t index>
        struct Node < Paren_, index, typename std::enable_if <index <= 0 || Paren_::length < index >::type>
        {
            static constexpr size_t id       = 0;
            static constexpr size_t graph_id = 0;
            struct PARENT
            {
                using type = Node <Paren_, index>;
            };

            struct FIRSTCHILD
            {
                using type = Node <Paren_, index>;
            };

            struct LASTCHILD
            {
                using type = Node <Paren_, index>;
            };

            struct NEXTSIBLING
            {
                using type = Node <Paren_, index>;
            };

            struct PREVSIBLING
            {
                using type = Node <Paren_, index>;
            };

            struct CHILDRENNUM
            {
                static constexpr size_t value = 0;
            };
        };

        // 木形のグラフ表現．
        template <class Paren_>
        struct Tree
        {
            using paren = Paren_;
        };

        // 括弧インデックスに対応するノードを返すメタ関数．
        template <class Tree_, size_t index>
        struct AT_AS_PAREN
        {
            using type = Node <typename Tree_::paren, index>;
        };

        // 根ノードを返すメタ関数．
        template <class Tree_>
        struct ROOT
            : AT_AS_PAREN <Tree_, 1>
        {
        };

        // グラフインデックスに対応するノードを返すメタ関数．
        template <class Tree_, size_t index>
        struct AT_AS_GRAPH
        {
            using type = Node <typename Tree_::paren, tree::shape::paren::SELECT <typename Tree_::paren, index>::value>;
        };

        template <class Tree_>
        struct V_SIZE
        {
            static constexpr size_t value = Tree_::paren::length / 2;
        };

        void test_treeshape()
        {
            // std::cout << util::Repeat("-", 20) << std::endl;
            // std::cout << "Tree representations test" << std::endl;
            // constexpr auto str = sprout::to_string("(()(()()(())))");
            // std::cout << str.c_str() << std::endl;
            // constexpr auto rev_str = sprout::fixed::reverse(str);
            // // Paren型．
            // using paren = paren::Paren <util::paren_to_bitseq(rev_str).to_ulong(), str.size()>;
            // // Tree型．
            // using graph = Tree <paren>;
            // constexpr size_t root             = graph::ROOT::type::id;
            // constexpr size_t root_firstchild  = graph::ROOT::type::FIRSTCHILD::type::id;
            // constexpr size_t root_childrennum = graph::ROOT::type::CHILDRENNUM::value;
            // constexpr size_t _4_as_paren  = graph::AT_AS_PAREN<4>::type::id;
            // constexpr size_t _4_as_graph = graph::AT_AS_GRAPH<4>::type::id;
            // std::cout << "root: " << root << std::endl;
            // std::cout << "root_firstchild: " << root_firstchild << std::endl;
            // std::cout << "root_childrennum: " << root_childrennum << std::endl;
            // std::cout << "4 as paren: " << _4_as_paren << std::endl;
            // std::cout << "4 as graph: " << _4_as_graph << std::endl;
            // std::cout << util::Repeat("-", 20) << std::endl;
        }
    }

    // 木形のグラフ表現．
    // インスタンス化して使うのを目的とする．
    // 部分特殊化テンプレートがデフォルトパラメータを持てなかったので，しかたなくshape::Node以外の第一テンプレートパラメータでも特殊化可能にしてある．
    template <class Node_, size_t index_in_paren = Node_::id, size_t children_num_ = Node_::CHILDRENNUM::value>
    class Node
    {
    public:
        // 以下コンパイル時に決まるメンバ．
        static constexpr size_t children_num = children_num_;
    public:
        constexpr Node(size_t id);
    public:
        // 以下インスタンス化時に決まるメンバ．
        const size_t                               id;
        const size_t                               parent;
        const sprout::array <size_t, children_num> children;
    };

    // 親がいない場合．
    template <class Node_, size_t children_num_>
    class Node <Node_, 0, children_num_>
    {
    public:
        // 以下コンパイル時に決まるメンバ．
        static constexpr size_t children_num = children_num_;
    public:
        constexpr Node(size_t id);
    public:
        // 以下インスタンス化時に決まるメンバ．
        const size_t                               id;
        const sprout::array <size_t, children_num> children;
    };

    // 子がいない場合．
    template <class Node_, size_t index_in_paren>
    class Node <Node_, index_in_paren, 0>
    {
    public:
        // 以下コンパイル時に決まるメンバ．
        static constexpr size_t children_num = 0;
    public:
        constexpr Node(size_t id);
    public:
        // 以下インスタンス化時に決まるメンバ．
        const size_t id;
        const size_t parent;
    };

    template <typename T, typename value_type>
    class Tree;

    template <class Paren_, typename value_type>
    class Tree <shape::Tree <Paren_>, value_type>
    {
    private:
        using Tree_ = shape::Tree <Paren_>;
        static constexpr size_t V_size = Tree_::V_size;
    public:
        // 根のインデックスは常に0．
        constexpr Tree(const sprout::array <value_type, V_size> &values);
    };

    template <class F, class T>
    struct DFS;

    template <class T>
    struct Dot;

    template <class Graph_>
    std::ostream&operator<<(std::ostream &os, const Dot <Graph_> &dot)
    {
        return os;
    }
}

#endif
