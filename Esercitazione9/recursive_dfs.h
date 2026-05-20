#pragma once
#include "graph_visit.h"
#include "graph.h"
#include <set>

template <typename T>
void parte_ricorsiva(const undirected_graph<T>& grafo, undirected_graph<T>& output, const T& nodo_partenza, std::set<T>& nodi_visti) {
    nodi_visti.insert(nodo_partenza);
   
    for (const T& nodo_vicino : grafo.neighbours(nodo_partenza)) {
        if (nodi_visti.find(nodo_vicino) == nodi_visti.end()) {
            output.add_edge(nodo_partenza, nodo_vicino);
            parte_ricorsiva(grafo, output, nodo_vicino, nodi_visti);
        }
    }
}


template <typename T>
undirected_graph<T> recursive_dfs(const undirected_graph<T>& grafo, const T& nodo_partenza) {
    undirected_graph<T> output;
    std::set<T> nodi_visti;

    
    parte_ricorsiva(grafo, output, nodo_partenza, nodi_visti);

    return output;
}