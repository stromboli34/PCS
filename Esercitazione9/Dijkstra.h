#pragma once
#include <iostream>
#include "graph.h"
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <limits>

const int inf= std::numeric_limits<int>::max();

template<typename T>
struct outputDijkstra{
    T inizio;
    std::map<T,int> distanze;
    std::map<T,T> predecessori;


void stampa_distanze() {
    std::cout << "Risultati Dijkstra: Partenza da " << inizio << "\n";
    
    for (const auto& [nodo, distanza] : distanze) {
        std::cout << "Nodo: " << nodo << " -> Distanza: ";
        if (distanza == std::numeric_limits<int>::max())  { //per comparare l'infinito
            std::cout << "non raggiungibile";
        } else {
            std::cout << distanza << "\n";
        }
        
        
}
}



void stampa_cammino(const T& destinazione) {

   
    undirected_graph<T> cammino;
    
    T nodo_corrente = destinazione;

     if (inizio == destinazione) {
        std::cout << inizio << "\n"; //non posso aggiungere al grafo un singolo nodo
       return; //metto return per uscire dalla funzione
    }

    
    if (predecessori.find(destinazione) == predecessori.end()) {
        std::cout << "Nessun percorso per: " << destinazione << "\n";
        return;                                                         
    }
   
    while (nodo_corrente != inizio) {
        cammino.add_edge(nodo_corrente,predecessori.at(nodo_corrente));
        nodo_corrente = predecessori.at(nodo_corrente); 
    }
     
    std::cout << "Cammino più veloce da: " << inizio << " a: " << destinazione << "\n";
    stampa_grafo(cammino);
   
    
}
};


template<typename T>

outputDijkstra<T> Dijkstra(const undirected_graph<T>& grafo,const T nodo_partenza){
    std::set<std::pair<int,T>> non_visitati;
    std::map<T,int> distanze;
    std::map<T,T> predecessori;
    for (T nodo : grafo.all_nodes()){
        
        if (nodo == nodo_partenza){
            distanze[nodo] = 0;
            non_visitati.insert({0,nodo});
        }
        else{
            distanze[nodo]=inf;
            non_visitati.insert({inf,nodo});
        }
    }

while (!non_visitati.empty()){
    auto [distanza_corrente, nodo_corrente]= *non_visitati.begin();
    non_visitati.erase(non_visitati.begin());

    if (distanza_corrente==inf){
        break;
    }
    for (auto nodo_vicino : grafo.neighbours(nodo_corrente)){
        
        
        int peso = grafo.get_weight(nodo_corrente,nodo_vicino);

        int nuova_distanza= distanza_corrente + peso;
        

        if (nuova_distanza < distanze[nodo_vicino]){
            non_visitati.erase({distanze[nodo_vicino],nodo_vicino});
            distanze[nodo_vicino]=nuova_distanza;
           
            predecessori[nodo_vicino]=nodo_corrente;

            non_visitati.insert({nuova_distanza,nodo_vicino});
        }


        }


    }

return{nodo_partenza,distanze,predecessori};
}