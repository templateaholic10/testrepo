// Copyright (C) 2015, Akari Ozora
#include <iostream>
#include <vector>
#include "symbol.hpp"
#include "nodetype.hpp"

namespace functree {
    template <Nodetype ntype>
    class Node;

    template <Nodetype ntype>
    class Node {
    public:
        Node();  // コンストラクタ
        ~Node();  // デストラクタ
        Node(Node const&);  // コピーコンストラクタ
        Node& operator =(Node const&);  // コピー代入演算子
        Node(Node&&);  // ムーブコンストラクタ
        Node& operator =(Node&&);  // ムーブ代入演算子

        // 評価
        template <typename ttype>
        ttype eval(const Symbol& s, ttype t) const;
    private:
        std::vector <Node> _operand;
    };
}
