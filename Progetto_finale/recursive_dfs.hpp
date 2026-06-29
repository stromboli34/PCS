#pragma once
#include "un_graph.hpp"
#include <set>
#include <vector>

template <typename N, typename W>
void parte_ricorsiva(const un_graph<N,W>& grafo, un_graph<N,W>& output,
                     const N& nodo_partenza, std::set<N>& nodi_visti) {
    struct Frame { N node; std::vector<N> nbrs; std::size_t idx; };
    std::vector<Frame> stack;
    nodi_visti.insert(nodo_partenza);
    { auto s = grafo.neighbours(nodo_partenza);
      stack.push_back({nodo_partenza, std::vector<N>(s.begin(), s.end()), 0}); }
    while (!stack.empty()) {
        Frame& f = stack.back();
        if (f.idx < f.nbrs.size()) {
            N u = f.node;
            N v = f.nbrs[f.idx++];
            if (nodi_visti.find(v) == nodi_visti.end()) {
                output.add_edge(u, v, grafo.weight(u, v)); 
                nodi_visti.insert(v);
                auto s = grafo.neighbours(v);
                stack.push_back({v, std::vector<N>(s.begin(), s.end()), 0});
            }
        } else stack.pop_back();
    }
}
template <typename N, typename W>
void recursive_dfs(const un_graph<N,W>& grafo, const N& nodo_partenza, std::set<N>& nodi_visti, un_graph<N,W>& output) {
    parte_ricorsiva(grafo, output, nodo_partenza, nodi_visti);
}

template <typename N, typename W>
un_graph<N,W> recursive_dfs(const un_graph<N,W>& grafo, const N& nodo_partenza) {
    un_graph<N,W> output;
    std::set<N> nodi_visti;
    parte_ricorsiva(grafo, output, nodo_partenza, nodi_visti);
    return output;
}