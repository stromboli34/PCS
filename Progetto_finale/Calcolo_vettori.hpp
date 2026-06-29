#pragma once

#include "un_graph.hpp"
#include "recursive_dfs.hpp"

#include <vector>

//Calcolo_vettori

// Genera i testimoni S: per ogni arco del coalbero un versore 0/1 sugli archi.
// Uniformato a vector<int> per essere coerente con dot e con diff_simm.
template <typename N, typename W = double>
std::vector<std::vector<int>> Calcolo_vettori(const un_graph<N, W>& G)
{
    const int m = G.all_edges().size();               // |E|
    std::vector<std::vector<int>> S;

    un_graph<N, W> T        = recursive_dfs(G, G.all_nodes()[0]);
    un_graph<N, W> Coalbero = G - T;

    S.reserve(Coalbero.all_edges().size());
    for (const auto& e : Coalbero.all_edges()) {
        std::vector<int> s(m, 0);
        s[G.edge_number(e)] = 1;
        S.push_back(std::move(s));
    }
    return S;
}
