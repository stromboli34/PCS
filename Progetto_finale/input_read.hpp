#pragma once

#include "un_graph.hpp"
#include "component.hpp"
#include "selectionSort.hpp"

#include <string>
#include <sstream>
#include <fstream>
#include <cassert>
#include <vector>
#include <set>
#include <cctype>

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

    // Riga vuota o di soli spazi: si salta.
    if (!(iss >> label))
        return false;

    const char type = label[0];                      // 'R'/'r' oppure 'V'/'v'
    if (type != 'R' && type != 'r' && type != 'V' && type != 'v')
        return false;                                // etichetta non riconosciuta: salto la riga

    // Da qui in poi la riga è intesa come componente:
    // ogni errore di formato deve essere segnalato, non ignorato.

    const std::string num_str = label.substr(1);     // la parte numerica, es. "1" in "R1"
    if (num_str.empty() ||
        !std::all_of(num_str.begin(), num_str.end(),
                    [](unsigned char c){ return std::isdigit(c); }))
        throw std::runtime_error("netlist: etichetta malformata: \"" + label + "\"");

    W value;   // resistenza (Ohm) o tensione (Volt)
    N a, b;    // i due nodi a cui è collegato il componente
    if (!(iss >> value >> a >> b))
        throw std::runtime_error(" riga malformata: \"" + line + "\"");

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
    std::sort(result.components.begin(), result.components.end());

    if (result.components.size() != G.all_edges().size())
        throw std::runtime_error("netlist: numero di componenti diverso dal numero di archi del grafo");

    for (std::size_t i = 0; i < result.components.size(); ++i) {
        if (not(result.components[i].get_edge() == G.all_edges()[i]))
            throw std::runtime_error("netlist: componente " + std::to_string(i) +
                                     " non allineato con l'arco corrispondente del grafo");
    }

    std::set<int> numeri_R;
    for (const auto& c : result.components) {
        if (c.kind() == Kind::R) numeri_R.insert(c.number());
    }
    if (numeri_R.size() != result.resistor_count)
        throw std::runtime_error("netlist: numero di resistori inconsistente");

    if (!numeri_R.empty()) {
        if (*numeri_R.begin() != 0)
            throw std::runtime_error("netlist: la numerazione dei resistori non parte da 0");
        if (*numeri_R.rbegin() != static_cast<int>(result.resistor_count) - 1)
            throw std::runtime_error("netlist: la numerazione dei resistori non e' contigua (0.."
                                     + std::to_string(result.resistor_count - 1) + ")");
    }

    return result;
}
