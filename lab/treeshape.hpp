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
        // Parenとは異なり，グラフとしてのidで管理する．

        // ポイントは
        // RANK: 括弧列としてのid→グラフとしてのid
        // SELECT: グラフとしてのid→括弧列としてのid

        // 括弧列クエリメタ関数をラップするグラフクエリメタ関数．
        // 2階メタ関数として実装してもよかったかも．
        template <template <class, size_t> class Query_for_Paren, class Paren_, size_t index>
        struct GRAPHCAL
        {
            static constexpr size_t value = tree::shape::paren::RANK <Paren_, Query_for_Paren <Paren_, tree::shape::paren::SELECT <Paren_, index>::value>::value>::value;
        };

        template <class Paren_, size_t index, class Ignored = void>
        struct Node
        {
            static constexpr size_t id          = index;
            static constexpr size_t id_as_paren = tree::shape::paren::SELECT <Paren_, index>::value;
            struct PARENT
            {
                using type = Node <Paren_, GRAPHCAL <tree::shape::paren::PARENT, Paren_, index>::value>;
            };

            struct FIRSTCHILD
            {
                using type = Node <Paren_, GRAPHCAL <tree::shape::paren::FIRSTCHILD, Paren_, index>::value>;
            };

            struct LASTCHILD
            {
                using type = Node <Paren_, GRAPHCAL <tree::shape::paren::LASTCHILD, Paren_, index>::value>;
            };

            struct NEXTSIBLING
            {
                using type = Node <Paren_, GRAPHCAL <tree::shape::paren::NEXTSIBLING, Paren_, index>::value>;
            };

            struct PREVSIBLING
            {
                using type = Node <Paren_, GRAPHCAL <tree::shape::paren::PREVSIBLING, Paren_, index>::value>;
            };

            struct CHILDRENNUM
            {
                static constexpr size_t value = paren::CHILDRENNUM <Paren_, tree::shape::paren::SELECT <Paren_, index>::value>::value;
            };
        };

        // indexがアクセス範囲外の時の特殊化．
        template <class Paren_, size_t index>
        struct Node < Paren_, index, typename std::enable_if <index <= 0 || Paren_::length / 2 < index > ::type>
        {
            static constexpr size_t id          = 0;
            static constexpr size_t id_as_paren = 0;
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
        struct TreeShape
        {
            using paren = Paren_;
        };

        // グラフインデックスに対応するノードを返すメタ関数．
        template <class TreeShape_, size_t index>
        struct AT
        {
            using type = Node <typename TreeShape_::paren, index>;
        };

        // 根ノードを返すメタ関数．
        template <class TreeShape_>
        struct ROOT
            : AT <TreeShape_, 1>
        {
        };

        // 括弧インデックスに対応するノードを返すメタ関数．
        template <class TreeShape_, size_t index>
        struct AT_AS_PAREN
        {
            using type = Node <typename TreeShape_::paren, tree::shape::paren::RANK <typename TreeShape_::paren, index>::value>;
        };

        // 子の数の最大値を返すメタ関数．
        template <class TreeShape_, size_t index>
        struct CHILDRENNUM_SUP
        {
            using type = Node <typename TreeShape_::paren, tree::shape::paren::RANK <typename TreeShape_::paren, index>::value>;
        };

        template <class TreeShape_>
        struct V_SIZE
        {
            static constexpr size_t value = TreeShape_::paren::length / 2;
        };

        void test_treeshape()
        {
            std::cout << util::Repeat("-", 20) << std::endl;
            std::cout << "Tree representations test" << std::endl;
            constexpr auto str = sprout::to_string("(()(()()(())))");
            std::cout << str.c_str() << std::endl;
            constexpr auto rev_str = sprout::fixed::reverse(str);
            // Paren型．
            using Paren = paren::Paren <util::paren_to_bitseq(rev_str).to_ulong(), str.size()>;
            // TreeShape型．
            using Shape = TreeShape <Paren>;
            constexpr size_t root             = tree::shape::ROOT <Shape>::type::id;
            constexpr size_t root_firstchild  = tree::shape::ROOT <Shape>::type::FIRSTCHILD::type::id;
            constexpr size_t root_childrennum = tree::shape::ROOT <Shape>::type::CHILDRENNUM::value;
            constexpr size_t _4_as_paren      = tree::shape::AT_AS_PAREN <Shape, 4>::type::id;
            constexpr size_t _4_as_graph      = tree::shape::AT <Shape, 4>::type::id;
            std::cout << "root: " << root << std::endl;
            std::cout << "root_firstchild: " << root_firstchild << std::endl;
            std::cout << "root_childrennum: " << root_childrennum << std::endl;
            std::cout << "4 as paren: " << _4_as_paren << std::endl;
            std::cout << "4 as graph: " << _4_as_graph << std::endl;
            std::cout << util::Repeat("-", 20) << std::endl;
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
    class TreeShape;

    template <class Paren_, typename value_type>
    class TreeShape <shape::TreeShape <Paren_>, value_type>
    {
    private:
        using TreeShape_ = shape::TreeShape <Paren_>;
        static constexpr size_t V_size = TreeShape_::V_size;
    public:
        // 根のインデックスは常に0．
        constexpr TreeShape(const sprout::array <value_type, V_size> &values);
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
