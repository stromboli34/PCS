#include <iostream>
#include <fstream>   
#include <vector>
#include <set>
#include "graph.h"
#include "graph_visit.h"
#include "recursive_dfs.h"
#include "Dijkstra.h"
int main() {
    std::vector<int> nodi = {1, 11, 5, 6, 4, 7, 3};
    std::vector<std::set<int>> archi = {{3, 5, 4}, {5, 6}, {1, 3, 11}, {4, 11}, {1, 6, 7}, {4}, {1, 5}}; //non inserisco pesi perchè li ho segnati di default a 1
    
    undirected_graph<int> grafo(nodi, archi);

    FIFO<int> f;
    LIFO<int> l;

    
    auto dfsG = graph_visit(grafo, 1, l);
    auto bfsG = graph_visit(grafo, 1, f);

   
    std::ofstream file_dfs("albero_dfs.dot");
    if (file_dfs.is_open()) {
        file_dfs << dfsG; 
        file_dfs.close();
        std::cout << "File albero_dfs.dot" << "\n";
    }

    
    std::ofstream file_bfs("albero_bfs.dot");
    if (file_bfs.is_open()) {
        file_bfs << bfsG; 
        file_bfs.close();
        std::cout << "File albero_bfs.dot" << "\n";
    }

auto dfsG_recursive= recursive_dfs(grafo,1);
    std::ofstream file_dfs_rec("albero_dfs_rec.dot");
    if (file_dfs_rec.is_open()) {
        file_dfs_rec << dfsG_recursive; 
        file_dfs_rec.close();
        std::cout << "File albero_dfs_rec.dot" << "\n";
    }

auto Alg_Dij= Dijkstra(grafo,1);

Alg_Dij.stampa_distanze();
Alg_Dij.stampa_cammino(11);




    return 0;
}