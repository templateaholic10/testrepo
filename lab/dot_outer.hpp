#ifndef DOT_OUTER
#define DOT_OUTER

#include <iostream>
#include <list>
#include <array>
#include <cstddef>
#include <cassert>
#include "graph_config.hpp"
#include "util.hpp"

namespace dot {
    std::string quote(const std::string& str)
    {
        return "\"" + str + "\"";
    }

    template <typename T, size_t num>
    std::string expand(const std::array<T, num>& a)
    {
        using CIT = typename std::array<T, num>::const_iterator;
        std::string result = std::to_string(a[0]);
        for (CIT it = a.begin()+1; it != a.end(); it++) {
            result += ", " + std::to_string(*it);
        }
        return result;
    }

    using RGB = std::array <unsigned char, 3>;
    constexpr RGB RGB_defo = RGB({ 255, 255, 255 });
    std::string to_string(const RGB& rgb)
    {
        return quote(util::color_encode(rgb));
    }

    using Rect = std::array <double, 2>;
    constexpr Rect Rect_defo = Rect({ 0, 0 });
    std::string to_string(const Rect& rect)
    {
        return quote(expand(rect));
    }

    std::string to_string(const std::string& str)
    {
        return quote(str);
    }

    std::string to_string(const bool flag)
    {
        return flag ? "true" : "false";
    }

    named_enum(Direction, graph, digraph);

    // サイズの単位はインチ．

    class Graph
    {
    public:
        named_enum(Loc, defo, b, t);

        named_enum(Just, defo, l, r);

        named_enum(Level_direction, defo, LR);

    public:
        // デフォルト値の設定
        Graph()
            : size(Rect_defo), label(""), labelloc(Loc::defo), labeljust(Just::defo), bgcolor(RGB_defo), rankdir(Level_direction::defo)
        {
        }

        friend std::ostream&operator<<(std::ostream &os, const Graph &graph);

    public:
        Rect                 size;
        std::string          label;
        Loc::Tag             labelloc;
        Just::Tag            labeljust;
        RGB                  bgcolor;
        Level_direction::Tag rankdir;
    };

    std::ostream&operator<<(std::ostream &os, const Graph &graph)
    {
        // 変数名を文字列リテラル化するには#演算子を使う．
        const std::string delim = ",";
#define ATTR(T, attr) (std::string(#attr " = ") + T::to_string(graph.attr) + delim)

        os << "graph [" << std::endl;
        if (graph.size != Rect_defo) {
            os << ATTR(dot, size) << std::endl;
        }
        if (graph.label != "") {
            os << ATTR(dot, label) << std::endl;
        }
        if (graph.labelloc != Graph::Loc::defo) {
            os << ATTR(Graph::Loc, labelloc) << std::endl;
        }
        if (graph.labeljust != Graph::Just::defo) {
            os << ATTR(Graph::Just, labeljust) << std::endl;
        }
        if (graph.rankdir != Graph::Level_direction::defo) {
            os << ATTR(Graph::Level_direction, rankdir) << std::endl;
        }
        os << "];";

        #undef ATTR

        return os;
    }

    // id = 0はマスターノード．
    // マスターノードの属性はすべてのノードに反映される．
    class Node
    {
    public:
        named_enum(Shape, defo, ellipse, box, circle, egg, triangle, diamond, trapezium, parallelogram, house, pentagon, hexagon, septagon, octagon, invtriangle, invtrapezium, invhouse, doublecircle, doubleoctagon, tripleoctagon, Mdiamonc, Msquare, Mcircle, rect, rectangle, plaintext, point);

        named_enum(Style, defo, solid, dotted, bold, invis, filled, diagonals, rounded, dashed);

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
            num = id_ + 1;
        }

        friend std::ostream&operator<<(std::ostream &os, const Node &node);

    public:
        // 基本情報
        size_t      id;
        std::string label;

        // 形
        // 勝手にfixedsizeが設定される．
        Rect          size;
        Shape::Tag         shape;
        Style::Tag         style;
        unsigned char peripheries;

        // 色
        RGB color;
        RGB fillcolor;

        static size_t num;
    };

    size_t Node::num = 1;

    std::ostream&operator<<(std::ostream &os, const Node &node)
    {
        // 変数名を文字列リテラル化するには#演算子を使う．
        const std::string delim = ",";
#define ATTR(T, attr) (std::string(#attr " = ") + T::to_string(node.attr) + delim)

        os << ((node.id == 0) ? "node" : std::to_string(node.id)) << " [" << std::endl;
        if (node.label != "") {
            os << ATTR(dot, label) << std::endl;
        }
        if (node.size != Rect_defo) {
            os << "fixedsize = true," << std::endl;
            os << "width = " << node.size[0] << "," << std::endl;
            os << "height = " << node.size[1] << "," << std::endl;
        }
        if (node.shape != Node::Shape::defo) {
            os << ATTR(Node::Shape, shape) << std::endl;
        }
        if (node.style != Node::Style::defo) {
            os << ATTR(Node::Style, style) << std::endl;
        }
        if (node.peripheries !=Node::peri_defo) {
            os << ATTR(std, peripheries) << std::endl;
        }
        if (node.color != RGB_defo) {
            os << ATTR(dot, color) << std::endl;
        }
        if (node.fillcolor != RGB_defo) {
            os << ATTR(dot, fillcolor) << std::endl;
        }
        os << "];";

        #undef ATTR

        return os;
    }

    // id = 0はマスターエッジ．
    // マスターエッジの属性はすべてのエッジに反映される．
    class Edge
    {
    public:
        named_enum(Direction, defo, back, both, none);

        named_enum(Arrow, defo, normal, box, crow, diamond, dot, inv, none, tee, vee, lbox, rbox, obox, olbox, orbox, lcrow, rcrow, ldiamond, rdiamond, odiamond, oldiamond, ordiamond, odot, linv, rinv, oinv, olinv, orinv, lnormal, rnormal, onormal, olnormal, ornormal, ltee, rtee, lvee, rvee);

        named_enum(Style, defo, solid, dotted, bold, invis, filled, diagonals, rounded, dashed);

        static constexpr unsigned char labeldistance_defo = 0;

        static constexpr unsigned char labelangle_defo = 0;

        static constexpr bool labelfloat_defo = false;

        static constexpr unsigned char arrowsize_defo = 0;

    public:
        // デフォルト値の設定
        Edge()
            : id(num), from_id(1), to_id(1), label(""), headlabel(""), taillabel(""), labeldistance(labeldistance_defo), labelangle(labelangle_defo), labelfloat(labelfloat_defo), style(Style::defo), dir(Edge::Direction::defo), arrowhead(Arrow::defo), arrowtail(Arrow::defo), arrowsize(arrowsize_defo), color(
                RGB_defo)
        {
            num++;
        }

        Edge(size_t id_)
            : id(id_), from_id(1), to_id(1), label(""), headlabel(""), taillabel(""), labeldistance(labeldistance_defo), labelangle(labelangle_defo), labelfloat(labelfloat_defo), style(Style::defo), dir(Edge::Direction::defo), arrowhead(Arrow::defo), arrowtail(Arrow::defo), arrowsize(arrowsize_defo), color(
                RGB_defo)
        {
            num = id_ + 1;
        }

        Edge(size_t from_id_, size_t to_id_)
            : id(num), from_id(from_id_), to_id(to_id_), label(""), headlabel(""), taillabel(""), labeldistance(labeldistance_defo), labelangle(labelangle_defo), labelfloat(labelfloat_defo), style(Style::defo), dir(Edge::Direction::defo), arrowhead(Arrow::defo), arrowtail(Arrow::defo), arrowsize(arrowsize_defo), color(
                RGB_defo)
        {
            num++;
        }

        friend std::ostream&operator<<(std::ostream &os, const Edge &edge);

    public:
        // 基本情報
        size_t      id;
        size_t from_id;
        size_t to_id;
        std::string label;
        std::string headlabel;
        std::string taillabel;

        // ラベル位置
        unsigned char labeldistance;
        unsigned char labelangle;
        bool          labelfloat;

        // 形
        Style::Tag           style;
        Edge::Direction::Tag dir;
        Arrow::Tag           arrowhead;
        Arrow::Tag           arrowtail;
        unsigned char   arrowsize;

        // 色
        RGB color;

        static size_t num;
    };

    size_t Edge::num = 0;

    std::ostream&operator<<(std::ostream &os, const Edge &edge)
    {
        // 変数名を文字列リテラル化するには#演算子を使う．
        const std::string delim = ",";
#define ATTR(T, attr) (std::string(#attr " = ") + T::to_string(edge.attr) + delim)

        if (edge.id == 0) {
            os << "edge";
        } else {
            os << edge.from_id << " -> " << edge.to_id;
        }
        os << " [" << std::endl;
        if (edge.label != "") {
            os << ATTR(dot, label) << std::endl;
        }
        if (edge.headlabel != "") {
            os << ATTR(dot, headlabel) << std::endl;
        }
        if (edge.taillabel != "") {
            os << ATTR(dot, taillabel) << std::endl;
        }
        if (edge.labeldistance !=Edge::labeldistance_defo) {
            os << ATTR(std, labeldistance) << std::endl;
        }
        if (edge.labelangle !=Edge::labelangle_defo) {
            os << ATTR(std, labelangle) << std::endl;
        }
        if (edge.labelfloat != Edge::labelfloat_defo) {
            os << ATTR(dot, labelfloat) << std::endl;
        }
        if (edge.style != Edge::Style::defo) {
            os << ATTR(Edge::Style, style) << std::endl;
        }
        if (edge.dir != Edge::Direction::defo) {
            os << ATTR(Edge::Direction, dir) << std::endl;
        }
        if (edge.arrowhead != Edge::Arrow::defo) {
            os << ATTR(Edge::Arrow, arrowhead) << std::endl;
        }
        if (edge.arrowtail != Edge::Arrow::defo) {
            os << ATTR(Edge::Arrow, arrowtail) << std::endl;
        }
        if (edge.arrowsize !=Edge::arrowsize_defo) {
            os << ATTR(std, arrowsize) << std::endl;
        }
        if (edge.color != RGB_defo) {
            os << ATTR(dot, color) << std::endl;
        }
        os << "];";

        #undef ATTR

        return os;
    }

    class Dot
    {
    public:
        Dot(const std::string& name_="akari", const Direction::Tag direction_=Direction::digraph)
            : name(name_), direction(direction_), node_master(0), edge_master(0)
        {
        }

        template <class Out_, typename std::enable_if <Out_::expression == graph::Expression::adjacency_list>::type * = nullptr>
        Dot&operator<<(const Out_ &out);

        template <class Out_, typename std::enable_if <Out_::expression == graph::Expression::adjacency_matrix>::type * = nullptr>
        Dot                &operator<<(const Out_ &out);

        friend std::ostream&operator<<(std::ostream &os, const Dot &dot);

    public:
        std::string name;
        Direction::Tag    direction;
        Graph             graph;
        Node              node_master;
        std::list <Node> nodes;
        Edge              edge_master;
        std::list <Edge> edges;
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
        os << Direction::to_string(dot.direction) << " " << dot.name << " {" << std::endl;
        os << dot.graph << std::endl;
        os << dot.node_master << std::endl;
        os << dot.edge_master << std::endl;
        for (auto node : dot.nodes) {
            os << node << std::endl;
        }
        for (auto edge : dot.edges) {
            os << edge << std::endl;
        }
        os << "}";

        return os;
    }
}

#endif
