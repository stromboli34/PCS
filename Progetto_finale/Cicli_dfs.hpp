#pragma once

#include "un_graph.hpp"
#include "recursive_dfs.hpp"
#include "bfs_v2.hpp"

#include <vector>
#include <set>

//Cicli_dfs

template<typename N, typename W>
std::vector<std::vector<N>> Cicli_dfs(const un_graph<N, W>& g) {
    //come nodo di partenza per dfs posso sceglierne uno casuale, i cicli non sono minimi e non saranno gli stessi, ma va bene lo stesso --> appunti

    std::vector<std::vector<N>> result;
    const auto nodes = g.all_nodes();
    if (nodes.empty()) return result; //in caso il grafo sia vuoto

    un_graph<N, W> t;
    std::set<N> visited;
    for (const auto& s : nodes)
        if (!visited.count(s)) {              // s e' di una componente non ancora vista
            visited.insert(s);
            recursive_dfs(g, s, visited, t);
        }  // faccio partire la dfs da qui
    //in questo modo ho gestuito anche il caso in cui il grafo sia non connesso

    un_graph<N, W> c = g - t;

    for (const auto& e : c.all_edges()) {
        outputBFS<N> bfs_tree = BFS(t, e.from());
        std::vector<N> ciclo = bfs_tree.cammino(e.to());
        ciclo.push_back(e.from());
        //std::move converte 'ciclo' da lvalue a rvalue, così push_back invoca l'overload che "ruba" il buffer interno (move) invece di copiarlo elemento per elemento. Dopo il move, 'ciclo' è in stato valido ma vuoto/non specificato: non va più letto. Qui esce comunque dallo scope all'iterazione successiva, quindi è sicuro.
        result.push_back(std::move(ciclo)); //valutare se tenere std::move()
    }

    return result;
}
