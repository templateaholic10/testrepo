#ifndef GRAPH_CONFIG
#define GRAPH_CONFIG

namespace graph {
    enum class Expression {
        adjacency_list,  // 隣接リスト表現
        adjacency_matrix,  // 隣接行列表現
    };
}

#endif
