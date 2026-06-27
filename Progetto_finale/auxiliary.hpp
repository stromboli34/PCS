#pragma once

#include "un_graph.hpp"
#include "recursive_dfs.hpp"
#include "bfs_v2.hpp"
#include "selectionSort.hpp"

#include <string>
#include <sstream>
#include <fstream>
#include <cassert>
#include <limits>
#include <cctype>

//component

enum class Kind {
    R, V
};

template<typename N, typename W = double>
class component {
    private:
    Kind type_of;
    un_edge<N,W> edge;
    bool oriented;
    int number_;

    public:
    component(const N& from, const N& to, const W& weight, const Kind& type, int number = 0)
        : type_of(type),
          edge(from, to, weight),
          oriented(type == Kind::V ? (from > to) : true),
          number_(number)
		  //Il valore booleano serve a capire l'orientamento delle mie componenti.
		  //Se è un resistore mettiamo sempre true, così dipenderà unicamente dall'orientazione della maglia, che sarà true se la maglia va in ordine crescente e false se va in ordine decrescente, così da contribuire con un +1 o -1 in B.
		  //Se invece la componente è un generatore mettiamo true se from > to, ossia se il primo nodo che ci viene dato (quello dove sta la carica positiva) è più grande del secondo, in questo modo se la maglia va in ordine crescente (true) otteniamo un true complessivo se percorre la maglia dal negativo al positivo, contribuendo con un positivo nella costruzione di v
    {}

    const un_edge<N,W>& get_edge() const {
        return edge;
    }

    Kind kind() const {
        return type_of;
    }

    bool is_oriented() const {
        return oriented;
    }

    int number() const {
        return number_;
    }

    bool operator<(const component& other) const {
        return edge < other.edge;
    }
};

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

// Prodotto scalare modulo 2 (GF(2)) per l'algoritmo di De Pina
inline int dot(const std::vector<int>& a, const std::vector<int>& b) {
    int risultato = 0;
    for (std::size_t i = 0; i < a.size(); ++i) {
        risultato ^= (a[i] & b[i]);   // prodotto, sommato mod 2
    }
    return risultato;
}

//De_Pina

// Differenza Simmetrica
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
    std::vector<N> nodi=grafo.all_nodes();

    un_graph<N,W> grafo_1;

    N offset = nodi.back() + 1;

    for (un_edge<N,W> arco : archi){
        N u = arco.from();
        N v= arco.to();
        N u_m= u+offset;
        N v_m= v+offset;

        if (S_i[grafo.edge_number(arco)]==1){
            grafo_1.add_edge(u,v_m,arco.weight());
            grafo_1.add_edge(u_m,v,arco.weight());
        }
        else{
            grafo_1.add_edge(u,v,arco.weight());
            grafo_1.add_edge(u_m,v_m,arco.weight());
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

            N u_orig = (a >= offset) ? (a - offset) : a;
            N v_orig = (b >= offset) ? (b - offset) : b;

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

template <typename N, typename W>
std::vector<std::vector<int>> De_Pina(un_graph<N,W> grafo, std::vector<std::vector<int>> S){
    std::vector<std::vector<int>> output;
    for (std::size_t i=0; i<S.size(); i++){
       std::vector<int> C_i = f_m_c_s_t(grafo, S[i]);
        output.push_back(C_i);
        for (std::size_t j=i+1;j<S.size();j++){
            if (dot(C_i,S[j])==1){
                S[j]=diff_simm(S[j],S[i]);
            }
        }
    }
    return output;
}

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

//Cicli_depina

template <typename N, typename W>
std::vector<std::vector<N>> Cicli_depina(const un_graph<N,W>& G) {
    // spanning tree via BFS
    auto nodi = G.all_nodes();
    outputBFS<N> bfs_result = BFS(G, nodi[0]);
    un_graph<N,W> T;
    for (const auto& [figlio, padre] : bfs_result.predecessori) {
        T.add_edge(padre, figlio, G.weight(padre, figlio));
    }

    // vettori testimone: un e_i per ogni arco del co-albero
    const auto& archi = G.all_edges();
    int m = static_cast<int>(archi.size());
    std::vector<std::vector<int>> S;
    for (int i = 0; i < m; ++i) {
        try {
            T.edge_number(archi[i]);
        } catch (const std::out_of_range&) {
            std::vector<int> s_i(m, 0);
            s_i[i] = 1;
            S.push_back(s_i);
        }
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
        if (adj.empty()) continue;
        std::vector<N> ciclo;
        N start = adj.begin()->first;
        N prev = N(-1), curr = start;
        do {
            ciclo.push_back(curr);
            for (const N& next : adj[curr]) {
                if (next != prev) { prev = curr; curr = next; break; }
            }
        } while (curr != start);
        cicli.push_back(ciclo);
    }
    return cicli;
}

//input_read

// Risultato della lettura della netlist.
//   - components      : tutti i componenti (resistori + generatori), in ordine di file
//   - resistor_vector : i soli resistori; la POSIZIONE i è la riga i di B e R.
//                       resistor_vector.size() == m (numero di resistori).
template <typename N, typename W>
struct netlist {
    std::vector<component<N, W>> components;
    std::size_t resistor_count = 0;
};

// Legge una riga di netlist e, se contiene un componente valido:
//   - aggiunge il componente a 'components'
//   - aggiunge il corrispondente arco (comp.get_edge()) al grafo G
//   - se è un resistore, lo aggiunge anche a 'resistor_vector'
// Ritorna true se è stato aggiunto un componente, false se la riga va
// ignorata (riga vuota o di soli spazi / incompleta).
template <typename N, typename W>
bool add_component_from_line(const std::string& line,
                             un_graph<N, W>& G,
                             std::vector<component<N, W>>& components,
                             std::size_t& resistor_count)
{
    std::istringstream iss(line);

    std::string label;   // es. "R1", "V2"
    W           value;   // resistenza (Ohm) o tensione (Volt)
    N           a, b;    // i due nodi a cui è collegato il componente

    // Se non si leggono i 4 campi la riga è vuota/incompleta: si salta.
    if (!(iss >> label >> value >> a >> b))
        return false;

    const char type = label[0];                      // 'R'/'r' oppure 'V'/'v'
    if (type != 'R' && type != 'r' && type != 'V' && type != 'v')
        return false;                                // etichetta non riconosciuta: salto la riga

    const std::string num_str = label.substr(1);     // la parte numerica, es. "1" in "R1"
    if (num_str.empty() ||
        !std::all_of(num_str.begin(), num_str.end(),
                    [](unsigned char c){ return std::isdigit(c); }))
        return false;                                // numero mancante o non numerico: salto

    const Kind kind   = (type == 'R' || type == 'r') ? Kind::R : Kind::V;
    const int  number = std::stoi(num_str) - 1;
	// Netlist 1-based ("R1" = primo resistore) → internamente 0-based, così
	// number_ può essere usato direttamente come indice di riga in B/R senza
	// alcuno shift in system_constructor. L'output finale ricostruisce
	// l'etichetta umana con (number_ + 1).

    component<N, W> comp(a, b, value, kind, number);     // costruzione diretta
    components.push_back(comp);
    G.add_edge(comp.get_edge());

    if (kind == Kind::R)              // aggiorno il vettore di resistori
        ++resistor_count;

    return true;
}

template <typename N, typename W>
netlist<N, W> read_netlist(const std::string& path, un_graph<N, W>& G)
{
    std::ifstream in(path);
    if (!in)
        throw std::runtime_error("impossibile aprire il file: " + path);

    netlist<N, W> result;

    std::string line;
    while (std::getline(in, line))
        add_component_from_line<N, W>(line, G, result.components, result.resistor_count);

    // Ordino lex per (from, to): così components[i] == G.all_edges()[i],
    // e dato un arco mi basta G.edge_number(u, v) per accedere al componente.
    selectionSort(result.components);

    // Verifica invarianti (gli assert spariscono in Release con -DNDEBUG):
    // 1. components allineato a G.all_edges() per (from, to)
    assert(result.components.size() == G.all_edges().size());
    for (std::size_t i = 0; i < result.components.size(); ++i) {
        assert(result.components[i].get_edge() == G.all_edges()[i]);
    }
    // 2. i number_ dei resistori formano l'insieme contiguo {0, 1, ..., m-1}.
    //    Senza questa proprietà, l'uso di component::number() come indice di
    //    riga in B/R produrrebbe accessi fuori range o "buchi" nelle matrici.
    std::set<int> numeri_R;
    for (const auto& c : result.components) {
        if (c.kind() == Kind::R) numeri_R.insert(c.number());
    }
    assert(numeri_R.size() == result.resistor_count);
    if (!numeri_R.empty()) {
        assert(*numeri_R.begin() == 0);
        assert(*numeri_R.rbegin() == static_cast<int>(result.resistor_count) - 1);
    }

    return result;
}
