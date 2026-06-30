#pragma once

#include "un_graph.hpp"
#include "recursive_dfs.hpp"
#include "De_Pina.hpp"

#include <vector>
#include <map>
#include <set>

//Cicli_depina

template <typename N, typename W>
std::vector<std::vector<N>> Cicli_depina(const un_graph<N,W>& G) {
    // Calcolo la dimensione dello spazio dei cicli trovando il numero di cicli fondamentali
    auto nodi = G.all_nodes();
    un_graph<N,W> T = recursive_dfs(G, nodi[0]);

    // co-albero = G − T e numero di archi del co-albero (numero di cicli fondamentali)
    const auto& archi = G.all_edges();
    int m = archi.size();
    un_graph<N,W> Coalbero = G - T;
    int nu = Coalbero.all_edges().size();

    // vettori testimone: un versore e_i per ogni arco del co-albero
    std::vector<std::vector<int>> S(nu, std::vector<int>(m, 0));
    int k = 0;
    for (const auto& e : Coalbero.all_edges()) {
        S[k][G.edge_number(e)] = 1;
        ++k;
    }

    // De Pina -> indicatori sugli archi
    auto indicatori = De_Pina(G, S);

    // conversione indicatori -> sequenze di nodi
    std::vector<std::vector<N>> cicli;
    for (const auto& ind : indicatori) {
        std::map<N, std::vector<N>> adj;
        for (int e = 0; e < m; ++e) {
            if (ind[e]) {
                N u = archi[e].from();
                N v = archi[e].to();
                adj[u].push_back(v);
                adj[v].push_back(u);
            }
        }
        if (adj.empty()){continue;}
        std::vector<N> ciclo;
        std::set<N> visitati;
        N curr = adj.begin()->first;
        while (visitati.find(curr) == visitati.end()) {
            visitati.insert(curr);
            ciclo.push_back(curr);
            for (const N& next : adj[curr]) {
                if (visitati.find(next) == visitati.end()) {
                    curr = next;
                    break;
                }
            }
        }
        ciclo.push_back(ciclo.front());
        cicli.push_back(ciclo);
    }
    return cicli;
}
