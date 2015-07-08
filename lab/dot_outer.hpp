#ifndef DOT_OUTER
#define DOT_OUTER

#include <iostream>
#include <queue>
#include <array>
#include <cstddef>
#include <cassert>
#include "graph_config.hpp"
#include "util.hpp"

namespace dot {
    using RGB = std::array <unsigned char, 3>;
    constexpr RGB RGB_defo = RGB({ 255, 255, 255 });

    using Rect = std::array <double, 2>;
    constexpr Rect Rect_defo = Rect({ 0, 0 });

    named_enum(Direction, graph, digraph);

    enum class Loc { defo, b, t };

    enum class Just { defo, l, r };

    enum class Style { defo, solid, dotted, bold, invis, filled, diagonals, rounded, dashed };

    // サイズの単位はインチ．

    class Graph
    {
    private:
        enum class Level_direction { defo, LR };

        friend std::ostream&operator<<(std::ostream &os, const Graph &graph);

    public:
        // デフォルト値の設定
        Graph()
            : size(Rect_defo), label(""), labelloc(Loc::defo), labeljust(Just::defo), bgcolor(RGB_defo), rankdir(Level_direction::defo)
        {
        }

    public:
        Rect            size;
        std::string     label;
        Loc             labelloc;
        Just            labeljust;
        RGB             bgcolor;
        Level_direction rankdir;
    };

    std::ostream&operator<<(std::ostream &os, const Graph &graph)
    {
#define ATTR_ENUM(attr) (std::string(#attr " = ") + std::to_string(attr))

        os << "graph [" << std::endl;

        os << "];" << std::endl;

        #undef ATTR_ENUM

        return os;
    }

    // id = 0はマスターノード．
    // マスターノードの属性はすべてのノードに反映される．
    class Node
    {
    private:
        enum class Shape { defo, ellipse, box, circle, egg, triangle, diamond, trapezium, parallelogram, house, pentagon, hexagon, septagon, octagon, invtriangle, invtrapezium, invhouse, doublecircle, doubleoctagon, tripleoctagon, Mdiamonc,
                           Msquare, Mcircle, rect, rectangle, plaintext, point };

        static constexpr unsigned char peri_defo = 0;
    public:
        // デフォルト値の設定
        Node()
            : id(num), label(""), size(Rect_defo), shape(Shape::defo), style(Style::defo), peripheries(peri_defo), color(RGB_defo), fillcolor(RGB_defo)
        {
            num++;
        }
        Node(size_t id_)
            : id(id_), label(""), size(Rect_defo), shape(Shape::defo), style(Style::defo), peripheries(peri_defo), color(RGB_defo), fillcolor(RGB_defo)
        {
            assert(id_ >= num);
            num = id_+1;
        }

    public:
        // 基本情報
        size_t      id;
        std::string label;

        // 形
        // 勝手にfixedsizeが設定される．
        Rect          size;
        Shape         shape;
        Style         style;
        unsigned char peripheries;

        // 色
        RGB color;
        RGB fillcolor;

        static size_t num;
    };

    size_t Node::num = 1;

    // id = 0はマスターエッジ．
    // マスターエッジの属性はすべてのエッジに反映される．
    class Edge
    {
    private:
        enum class Direction { defo, back, both, none };

        enum class Arrow { defo, normal, box, crow, diamond, dot, inv, none, tee, vee, lbox, rbox, obox, olbox, orbox, lcrow, rcrow, ldiamond, rdiamond, odiamond, oldiamond, ordiamond, odot, linv, rinv, oinv, olinv, orinv, lnormal, rnormal,
                           onormal, olnormal, ornormal, ltee, rtee, lvee, rvee };

    public:
        // デフォルト値の設定
        Edge()
            : id(num), label(""), headlabel(""), taillabel(""), labeldistance(0), labelangle(0), labelfloat(false), style(Style::defo), dir(Edge::Direction::defo), arrowhead(Arrow::defo), arrowtail(Arrow::defo), arrowsize(0), color(
                RGB_defo)
        {
            num++;
        }
        Edge(size_t id_)
            : id(id_), label(""), headlabel(""), taillabel(""), labeldistance(0), labelangle(0), labelfloat(false), style(Style::defo), dir(Edge::Direction::defo), arrowhead(Arrow::defo), arrowtail(Arrow::defo), arrowsize(0), color(
                RGB_defo)
        {
            assert(id_ >= num);
            num = id_+1;
        }

    public:
        // 基本情報
        size_t      id;
        std::string label;
        std::string headlabel;
        std::string taillabel;

        // ラベル位置
        unsigned char labeldistance;
        unsigned char labelangle;
        bool          labelfloat;

        // 形
        Style           style;
        Edge::Direction dir;
        Arrow           arrowhead;
        Arrow           arrowtail;
        unsigned char   arrowsize;

        // 色
        RGB color;

        static size_t num;
    };

    size_t Edge::num = 0;

    class Dot
    {
    public:
        Dot(Direction direction=Direction::digraph)
            : _direction(direction)
        {
        }

        template <class Out_, typename std::enable_if <Out_::expression == graph::Expression::adjacency_list>::type * = nullptr>
        Dot&operator<<(const Out_ &out);

        template <class Out_, typename std::enable_if <Out_::expression == graph::Expression::adjacency_matrix>::type * = nullptr>
        Dot                &operator<<(const Out_ &out);

        friend std::ostream&operator<<(std::ostream &os, const Dot &dot);

    private:
        Direction         _direction;
        Graph             _graph;
        Node              _node_master;
        std::queue <Node> _node;
        Edge              _edge_master;
        std::queue <Edge> _edge;
    };

    template <class Out_, typename std::enable_if <Out_::expression == graph::Expression::adjacency_list>::type *>
    Dot&Dot::operator<<(const Out_ &out)
    {
        return *this;
    }

    template <class Out_, typename std::enable_if <Out_::expression == graph::Expression::adjacency_matrix>::type *>
    Dot&Dot::operator<<(const Out_ &out)
    {
        return *this;
    }

    std::ostream&operator<<(std::ostream &os, const Dot &dot)
    {
        os << "# dot file" << std::endl;

#define ATTR(attr) (std::string(#attr " = ") + std::to_string(attr))

        #undef ATTR

        return os;
    }
}

#endif
