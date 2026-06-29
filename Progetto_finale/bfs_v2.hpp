#pragma once

#include "un_graph.hpp"
#include <queue>
#include <vector>
#include <algorithm>   // per std::reverse
#include <map>

template<typename T>
struct outputBFS {
    T inizio;
    std::map<T, int> distanze;       // distanza dalla partenza, misurata in archi
    std::map<T, T>   predecessori;   // predecessore di ogni nodo nell'albero BFS

    // ricostruisce il cammino inizio -> destinazione risalendo i predecessori
    std::vector<T> cammino(const T& destinazione) {
        std::vector<T> cammino;

        // se la destinazione non ha distanza assegnata non e stata raggiunta
        if (distanze.find(destinazione) == distanze.end()) {
            return cammino; // vuoto: nessun percorso
        }

        T nodo_corrente = destinazione;
        while (nodo_corrente != inizio) {
            cammino.push_back(nodo_corrente);
            nodo_corrente = predecessori.at(nodo_corrente);
        }
        cammino.push_back(inizio);
        std::reverse(cammino.begin(), cammino.end());
        return cammino;
    }
};


template<typename T, typename W>
outputBFS<T> BFS(const un_graph<T, W>& grafo, const T nodo_partenza) {
    std::map<T, int> distanze;
    std::map<T, T>   predecessori;
    std::queue<T>    coda;

    distanze[nodo_partenza] = 0;
    coda.push(nodo_partenza);

    while (!coda.empty()) {
        T nodo_corrente = coda.front();
        coda.pop();

        for (const auto& nodo_vicino : grafo.neighbours(nodo_corrente)) {
            if (distanze.find(nodo_vicino) == distanze.end()) {
                distanze[nodo_vicino]     = distanze[nodo_corrente] + 1;
                predecessori[nodo_vicino] = nodo_corrente;
                coda.push(nodo_vicino);
            }
        }
    }

    return {nodo_partenza, distanze, predecessori};
}
