#pragma once
#include "FIFO&LIFO.h"
#include "graph.h"
#include <set>
#include <map>
template <typename T, typename M>
undirected_graph<T> graph_visit(const undirected_graph<T>& grafo, const T& nodo_partenza, M& contenitore){
    std::set<T> nodi_visitati;
    std::map<T,T> figlio_padre;
    undirected_graph<T> output;
    output.map[nodo_partenza];
    contenitore.put(nodo_partenza);
    while (!contenitore.empty()){
       T nodo_corrente = contenitore.get();

       
        if (nodi_visitati.find(nodo_corrente)==nodi_visitati.end()){
            nodi_visitati.insert(nodo_corrente);
            if (nodo_corrente != nodo_partenza){
                output.add_edge(figlio_padre[nodo_corrente],nodo_corrente);
            }
            for (const T& vicino : grafo.neighbours(nodo_corrente)){
                if (nodi_visitati.find(vicino)==nodi_visitati.end()){
                    contenitore.put(vicino);
                    figlio_padre[vicino]=nodo_corrente;
                }
        }
       }
    }
    return output;
}
