#pragma once
#include "un_graph.hpp"
#include <set>

template <typename N, typename W>
void parte_ricorsiva(const un_graph<N,W>& grafo, un_graph<N,W>& output, const N& nodo_partenza, std::set<N>& nodi_visti) {
    nodi_visti.insert(nodo_partenza);

    for (const N& nodo_vicino : grafo.neighbours(nodo_partenza)) {
        if (nodi_visti.find(nodo_vicino) == nodi_visti.end()) {
            output.add_edge(nodo_partenza, nodo_vicino);
            parte_ricorsiva(grafo, output, nodo_vicino, nodi_visti);
        }
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
