#pragma once
#include<vector>
#include<string>
#include<iostream>
#include<map>
#include<set>
#include<algorithm>
#include<iterator>

template<typename N> //N stands for node
class undirected_edge {
    private:
        N from_;
        N to_;
    public:
        //costruttore di default, serve?
        //un_edge() {}

        //user defined
        undirected_edge(const N& a, const N& b) {
            if (a < b) {
                from_ = a;
                to_ = b;
            }
            else { //riordino
                from_ = b;
                to_ = a;
            }
        }

        //metodi per ottenedere i nodi dell'arco
        const N& from() const {return from_; } //prendo un riferimento costante non una copia (meglio per tipi costosi)
        const N& to() const {return to_; }

        //operatore di confronto stretto (lessicografico) tra archi
        bool operator<(const undirected_edge& other) const { //non serve controllo dei tipi perchè in C++ avviene durante la compilazione, non a runtime (che è quando il controllo scritto qui mi darebbe errore), se i tipi sono diversi già in compilazione me ne accorgo
            if (from_ != other.from()) return from_ < other.from(); //potrei anche usare other.from_ che dall'oggetto other mi prende direttamente la sua variabile from_ (questo non è possibile nel main perchè le variabili from_ e to_ sono private, ma nella definizione di un metodo della classe posso accedervi) 
            else return to_ < other.to(); //other.from_ è un accesso più diretto mentre pther.from() deve andare a trovare il codice della classe dove ho definito il metodo from()
        }

        //operatore di confronto ==
        bool operator==(const undirected_edge& other) const {
            return (from_ == other.from() && to_ == other.to());
        }
};

//funzione libera ossia, operatore definito con membro di sinistra non *this ma qualche membro di una classe non mia (in questo caso classe della libreria standard: std::ostream) dunque non posso tenerlo dentro alla mia classe dove i membri di sinistra sono sempre *this
//qua quindi devo esplicitare due parametri
template<typename N>
std::ostream& operator<<(std::ostream& os, const undirected_edge<N>& e) {
    os << "(" << e.from() << "," << e.to() << ")";
    return os;
}
template <typename T>

struct undirected_graph{

    std::map<T,std::set<T>> map;

    undirected_graph(){};
    undirected_graph(const std::vector<T>& nodi, const std::vector<std::set<T>>& archi ){
      for (size_t i = 0; i < nodi.size(); ++i) {

            map[nodi[i]] = archi[i];
        }


    }

    const std::set<T>& neighbours(const T& nodo_partenza) const{
        return(map.at(nodo_partenza));

    }
    void add_edge(const T& part, const T& dest){
        map[part].insert(dest);
        map[dest].insert(part);
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
