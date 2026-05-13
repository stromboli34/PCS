#include "graph.h"


void stampa_grafo(const undirected_graph<int>& grafo) {
    
    std::cout << "nodi: ";
    for (const auto& nodo : grafo.all_nodes()) {
        std::cout << nodo << " ";
    }
    std::cout << "\n numero archi: " << grafo.all_edges().size() << "\n";
    for (const auto& arco : grafo.all_edges()) {
        std::cout << "  " << arco.from() << " --- " << arco.to() << "\n";
    }
    std::cout << "\n";
    
}

int main(){
    try{
std::vector<int> nodi= {1,11,5,6,4,7,3};

std::vector<std::set<int>> archi= {{3,5,4},{5,6},{1,3,11},{4,11},{1,7},{4},{1,5}};



undirected_graph<int> grafo= undirected_graph(nodi,archi);

stampa_grafo(grafo);

std::cout << "Vicini del nodo 4: ";
for (const auto& vicino : grafo.neighbours(4)) {
    std::cout << vicino << " ";
}
std::cout << "\n";

grafo.add_edge(11,3);

std::cout << "\n" << "Aggiunto arco 3-11" << "\n";

stampa_grafo(grafo);


undirected_edge<int> arco_35 = undirected_edge(3,5);
std::cout << "Posizione dell'arco 3-5: " << grafo.edge_number(arco_35) << "\n";

std::cout << "\n" << "Arco in posizione 6: " << grafo.edge_at(6) << "\n";


undirected_graph<int> grafo_2 = grafo;
grafo_2.add_edge(7,3);
grafo_2.add_edge(1,11);

undirected_graph<int> grafo_diff = grafo_2 - grafo;


std::cout << "\n" << "Grafo con archi 3-7 e 1-11 in più: " << "\n";
stampa_grafo(grafo_2);

std::cout << "Stampa della differenza con grafo originale: " << "\n";
stampa_grafo(grafo_diff);

return EXIT_SUCCESS;
    }
catch(...){
return EXIT_FAILURE;
    }
}