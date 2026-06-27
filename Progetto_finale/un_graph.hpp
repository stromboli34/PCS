#pragma once

#include "un_edge.hpp"

#include<map>
#include<set>
#include<vector>
#include<algorithm> //per lower_bound
#include<stdexcept> //per out_of_range
#include<iterator> //per back_inserter

//grafo semplice: non più di 1 arco per stessi nodi, archi pesati (peso default 1 se non specificato)

template<typename N, typename W = double>
class un_graph {
    private:
        std::map<N, std::map<N, W>> adj_; //matrice di adiacenza pesata: per ogni nodo, una mappa che associa ogni vicino al peso dell'arco
        std::vector<un_edge<N, W>> edges_;   //contenitore per la numerazione degli archi secondo la relazione < definita su un_edge, il vector è veloce per le query ma meno veloce di un set per gli insert
        //inoltre sarebbe ancora più veloce usare una numerazione per ordine di inserimento
    public:
        //costruttore di default: inizializza tutti i membri con a loro volta il costruttore di default di ciascuno, in particolare li inizializza vuoti
        un_graph() = default; //altrimenti posso esplicitare i membri con un_graph() : adj_(), edges_() {} (utile se volessi inizializzare ciasuno in modo particolare)

        //costruttore copia
        un_graph(const un_graph& other) //qua uso l'altra sintassi perchè è più chiaro cosa viene fatto, ma potrei anche qua usare = default;
            : adj_(other.adj_),  //posso usare l'accesso diretto ai membri di other
            edges_(other.edges_)
        {}

        //metodo neighbours: ritorna solo l'identità dei vicini (senza pesi). Lasciato così per non rompere BFS/DFS che non hanno bisogno dei pesi
        std::set<N> neighbours(const N& n) const {
            auto it = adj_.find(n); //find è un metodo di map che mi restituisce un iteratore che può puntare o a un pair (chiave=n, valore) oppure a adj_.end(); a differenza di altri metodi su map se gli passo un nodo non esistente non mi da errori
            if (it == adj_.end()) { //verifico se l'iteratore it punta al valore sentinella
                return {}; //mi ritorna un oggetto del tipo dichiarato all'inizio ma vuoto
            }
            std::set<N> result;
            for (const auto& [v, w] : it->second) { //estraggo solo le chiavi della mappa interna (i vicini), il peso w non lo uso qui
                result.insert(v);
            }
            return result;
        }

        //metodo weighted_neighbours: ritorna la mappa vicino → peso, utile per Dijkstra
        std::map<N, W> weighted_neighbours(const N& n) const {
            auto it = adj_.find(n);
            if (it == adj_.end()) return {}; //stessa logica di neighbours: nodo inesistente → mappa vuota
            return it->second;
        }

        //metodo weight: peso di un singolo arco (a, b)
        W weight(const N& a, const N& b) const {
            return adj_.at(a).at(b); //due at successivi, lancia out_of_range se l'arco o uno dei nodi non esiste
        }

        //metodo add_edge: peso opzionale, default 1
        bool add_edge(const N& a, const N& b, const W& w = W{1}) {
            un_edge<N, W> e(a, b, w); //il costruttore di un_edge controlla che w >= 0
            //aggiungo l'arco al vector che li enumera: uso il metodo lower_bound che mi da l'iteratore i al primo elemento t.c. *i < e is false (ovviamente il vettore deve già essere ordinato)
            auto it = std::lower_bound(edges_.begin(), edges_.end(), e);
            if (it != edges_.end() && *it == e) return false; //già presente (operator== su un_edge ignora il peso, identità è (from, to))
            else edges_.insert(it, e); //inserisce e nella posizione precedente a it

            //aggiungo i nodi e il peso alla matrice di adiacenza pesata
            //uso l'operatore [] sulla mappa esterna che mi da la mappa interna associata, oppure se la chiave è nuova crea la chiave con una mappa vuota
            adj_[a][b] = w;
            adj_[b][a] = w; //grafo non orientato: simmetrico
            return true;
        }

        //add_edge se passi direttamente un arco
        bool add_edge(const un_edge<N, W>& e) {
            auto a = e.from();
            auto b = e.to();
            auto w = e.weight();
            return add_edge(a, b, w);
        }

        //metodo all_edges
        const std::vector<un_edge<N, W>>& all_edges() const { //ritorno un riferimento costante, una copia sarebbe costosa
            return edges_; //già ordinato
        }

        //metodo all_nodes
        std::vector<N> all_nodes() const {
            std::vector<N> result;
            result.reserve(adj_.size()); //questo mi risparmia il dover riallocare memoria ogni volta che aggiungo qualcosa
            for (const auto& [node, neighbours] : adj_) { //prendo da adj_ nell'ordine che ha automaticamente imposto sulle chiavi
                result.push_back(node);
            }
            return result;
        }

        //metodo edge_number
        int edge_number(const un_edge<N, W>& e) const {
            auto it = std::lower_bound(edges_.begin(), edges_.end(), e);
            if (it == edges_.end() || !(*it == e)) {
                throw std::out_of_range("edge not in graph"); //errore se l'arco non esiste
            }
            else return std::distance(edges_.begin(), it);
        }

        //edge_number se passi una coppia di nodi: il peso non entra nel confronto, posso usare il default 1
        int edge_number(const N& a, const N& b) const {
            return edge_number(un_edge<N, W>(a, b));
        }

        //metodo edge_at
        un_edge<N, W> edge_at(int n) const {
            return edges_.at(n); //il metodo at mi da errore se n è aut of range come per edge_number
        }

};

//ho usato Claude
//funzione libera per simmetria dell'operatore ma si poteva scrivere anche dentro alla classe
template<typename N, typename W>
un_graph<N, W> operator-(const un_graph<N, W>& g1, const un_graph<N, W>& g2) {
    const auto& edges_1 = g1.all_edges();
    const auto& edges_2 = g2.all_edges();

    std::vector<un_edge<N, W>> diff;
    std::set_difference( //dati due range ordinati calcola la differenza insiemistica e fornisce un output iterator; usa operator< su un_edge che confronta solo (from, to), il peso non incide
        edges_1.begin(), edges_1.end(),
        edges_2.begin(), edges_2.end(),
        std::back_inserter(diff) //a ogni dereferenziazione dell'output iterator dentro set_difference esegue un push_back su diff
    );

    un_graph<N, W> result;
    for (const auto& e : diff) {
        result.add_edge(e);
    }
    return result;
};

//ho usato Claude
//export in formato DOT per GraphViz: produce un grafo non orientato con i pesi come label degli archi
//è un operator<< libero perchè il membro di sinistra è uno std::ostream (classe std), come per un_edge::operator<<
template<typename N, typename W>
std::ostream& operator<<(std::ostream& os, const un_graph<N, W>& g) {
    os << "graph G {\n"; //"graph" indica grafo non orientato (sarebbe "digraph" per orientati); "G" è solo un nome convenzionale
    //dichiaro tutti i nodi: utile anche solo per vederli stampati in ordine, e difesa preventiva nel caso ci fossero nodi senza archi (oggi un_graph non li ammette ma è gratuito)
    for (const auto& node : g.all_nodes()) {
        os << "    \"" << node << "\";\n"; //metto i nodi tra virgolette per gestire eventuali caratteri speciali nei nomi
    }
    //archi: from -- to [label="peso"]
    for (const auto& e : g.all_edges()) {
        os << "    \"" << e.from() << "\" -- \"" << e.to() << "\" [label=\"" << e.weight() << "\"];\n";
    }
    os << "}\n";
    return os;
}
