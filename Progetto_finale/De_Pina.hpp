#pragma once

#include "un_graph.hpp"
#include "bfs_v2.hpp"

#include <vector>
#include <limits>
#include <algorithm>

// Prodotto scalare modulo 2 (GF(2)) per l'algoritmo di De Pina
inline int dot(const std::vector<int>& a, const std::vector<int>& b) {
    int risultato = 0;
    for (std::size_t i = 0; i < a.size(); ++i) {
        risultato ^= (a[i] & b[i]);   // prodotto, sommato mod 2
    }
    return risultato;
}


// Differenza Simmetrica (XOR)
template <typename N>
std::vector<N> diff_simm(const std::vector<N>& a, const std::vector<N>& b) {
    std::vector<N> output(a.size());
    for (size_t i = 0; i < a.size(); i++) {
        output[i] = a[i] ^ b[i];
    }
    return  output;
}

// Find minimal cycles such that
template<typename N, typename W>
std::vector<int> f_m_c_s_t(const un_graph<N,W>& grafo, const std::vector<N>& S_i){
    std::vector<un_edge<N,W>> archi = grafo.all_edges();
    std::vector<N> nodi = grafo.all_nodes();

    un_graph<N,W> grafo_1;

    N offset = nodi.back() + 1;

    for (un_edge<N,W> arco : archi){
        N u = arco.from();
        N v = arco.to();
        N u_m = u + offset;
        N v_m = v + offset;

        if (S_i[grafo.edge_number(arco)] == 1) {
            grafo_1.add_edge(u, v_m, arco.weight());
            grafo_1.add_edge(u_m, v, arco.weight());
        }
        else {
            grafo_1.add_edge(u, v, arco.weight());
            grafo_1.add_edge(u_m, v_m, arco.weight());
        }
    }
    std::vector<int> miglior_C_mu(archi.size(), 0);
    int min_elementi_uno = std::numeric_limits<int>::max();

    for (N nodo : nodi) {
        outputBFS<N> bfs_result = BFS(grafo_1, nodo);
        std::vector<N> cammino_nodo = bfs_result.cammino(nodo + offset);

        if (cammino_nodo.empty()) continue;

        std::vector<int> C_mu_i(archi.size(), 0);

        for (size_t i = 0; i < cammino_nodo.size() - 1; i++) {
            N a = cammino_nodo[i];
            N b = cammino_nodo[i+1];

            N u_orig = a;
            if (a >= offset) u_orig = a - offset;
            N v_orig = b;
            if (b >= offset) v_orig = b - offset;

            if (u_orig == v_orig) continue;

            int edge_idx = grafo.edge_number(u_orig, v_orig);
            C_mu_i[edge_idx] ^= 1;
        }

        int conteggio_uni = std::count(C_mu_i.begin(), C_mu_i.end(), 1);

        if (conteggio_uni > 0 && conteggio_uni < min_elementi_uno) {
            min_elementi_uno = conteggio_uni;
            miglior_C_mu = C_mu_i;
        }
    }

    return miglior_C_mu;
}
// De Pina
template <typename N, typename W>
std::vector<std::vector<int>> De_Pina(un_graph<N,W> grafo, std::vector<std::vector<int>> S) {
    std::vector<std::vector<int>> output;
    for (std::size_t i = 0; i < S.size(); i++) {
        std::vector<int> C_i = f_m_c_s_t(grafo, S[i]);
        output.push_back(C_i);
        for (std::size_t j = i + 1; j < S.size(); j++) {
            if (dot(C_i, S[j]) == 1) {
                S[j] = diff_simm(S[j], S[i]);
            }
        }
    }
    return output;
}
