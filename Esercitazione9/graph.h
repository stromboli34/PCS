#pragma once
#include<vector>
#include<string>
#include<iostream>
#include<map>
#include<set>
#include<algorithm>
#include<iterator>

template<typename N> 
class undirected_edge {
    private:
        N from_;
        N to_;
    public:
  
        undirected_edge(const N& a, const N& b) {
            if (a < b) {
                from_ = a;
                to_ = b;
            }
            else { 
                from_ = b;
                to_ = a;
            }
        }

       
        const N& from() const {return from_; } 
        const N& to() const {return to_; }

       
        bool operator<(const undirected_edge& other) const { 
            if (from_ != other.from()) return from_ < other.from(); 
            else return to_ < other.to(); 
        }

        
        bool operator==(const undirected_edge& other) const {
            return (from_ == other.from() && to_ == other.to());
        }
};


template<typename N>
std::ostream& operator<<(std::ostream& os, const undirected_edge<N>& e) {
    os << "(" << e.from() << "," << e.to() << ")";
    return os;
}
template <typename T>

struct undirected_graph{

    std::map<T,std::set<T>> map;
    std::map<undirected_edge<T>, int> pesi_archi;

    undirected_graph(){};
    undirected_graph(const std::vector<T>& nodi, const std::vector<std::set<T>>& archi ){
      for (size_t i = 0; i < nodi.size(); ++i) {

            map[nodi[i]] = archi[i];
            for(const T& vicino : archi[i]) {
                pesi_archi[undirected_edge<T>(nodi[i], vicino)] = 1;
            }
        }

    }
    

    const std::set<T>& neighbours(const T& nodo_partenza) const{
        return(map.at(nodo_partenza));

    }
    void add_edge(const T& part, const T& dest, int peso = 1)  { //questo =1 è un default
        map[part].insert(dest);
        map[dest].insert(part);
        pesi_archi[undirected_edge<T>(part, dest)] = peso;
    }

    

   
    std::set<undirected_edge<T>> all_edges() const{
        std::set<undirected_edge<T>> archi;
       for (const auto& [nodo_partenza, vicini] : map){
            for(const T& nodo_arrivo : vicini){
                if(nodo_partenza<nodo_arrivo){
                
                archi.insert(undirected_edge<T> (nodo_partenza,nodo_arrivo));
                
                }
            }
        }
        return archi;
    }

    std::vector<T> all_nodes() const{
        std::vector<T> nodi;
        for (const auto& [nodo_partenza, vicini] : map){
            nodi.push_back(nodo_partenza);   
        }
        return nodi;
    }

    int edge_number(const undirected_edge<T>& arco) const {
       std::set<undirected_edge<T>> archi=all_edges();
       auto it = archi.find(arco);
        int posizione = std::distance(archi.begin(), it);


        return posizione+1;      


    }
    undirected_edge<T> edge_at(const int& posizione) const{

        std::set<undirected_edge<T>> tutti_archi=all_edges();
        auto it= std::next(tutti_archi.begin(),posizione-1);
        undirected_edge<T> arco = *it;

        return arco;
    }

    void set_weight(const T& part, const T& dest, int nuovo_peso) {
        pesi_archi[undirected_edge<T>(part, dest)] = nuovo_peso;
    }

    
    int get_weight(const T& part, const T& dest) const {
        undirected_edge<T> arco(part, dest);

        return pesi_archi.at(arco);
        }
     

    };
   


template<typename T>
undirected_graph<T> operator-(const undirected_graph<T>& grafo_1, const undirected_graph<T>& grafo_2) {
    std::set<undirected_edge<T>> differenza;
    std::set<undirected_edge<T>> archi_1=grafo_1.all_edges();
    std::set<undirected_edge<T>> archi_2=grafo_2.all_edges();
    std::set_difference(
        archi_1.begin(), archi_1.end(), 
        archi_2.begin(), archi_2.end(), 
        std::inserter(differenza, differenza.begin())
    );
    std::vector<T> nodi = grafo_1.all_nodes();
    std::vector<std::set<T>> archi_vuoti(nodi.size());
    undirected_graph<T> grafo_diff = undirected_graph(nodi,archi_vuoti);
    
    
    for (const auto& arco : differenza) {
        
        grafo_diff.add_edge(arco.from(), arco.to());
    }

        return grafo_diff;
    

    }

template<typename T> 
std::ostream& operator<<(std::ostream& os, const undirected_graph<T>& g) {  //mi serve per il .dot
    os << "graph G {\n"; // "graph" indica un grafo non orientato
    
    // Dichiaro tutti i nodi
    for (const auto& node : g.all_nodes()) {
        os << "    \"" << node << "\";\n"; 
    }
    
    // Archi SENZA pesi: stampiamo semplicemente "from -- to;"
    for (const auto& e : g.all_edges()) {
        os << "    \"" << e.from() << "\" -- \"" << e.to() << "\";\n";
    }
    
    os << "}\n";
    return os;
}

template <typename T>
void stampa_grafo(const undirected_graph<T>& grafo) {
    
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