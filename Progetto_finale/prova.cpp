#pragma once
#include "un_edge.hpp"
#include <stdexcept>

template<typename N, typename W = double>
class circuit_edge : public un_edge<N, W> {
public:
    enum class Kind { Resistor, Generator };

private:
    Kind kind_;
    bool positive_at_to_;   // significativo solo se kind_ == Generator
                            // true  → polo "+" su to_  (nodo di indice maggiore)
                            // false → polo "+" su from_ (nodo di indice minore)

public:
    // Costruttore "resistore": polarità non serve
    circuit_edge(const N& a, const N& b, const W& resistance):
          un_edge<N, W>(a, b, resistance),
          kind_(Kind::Resistor),
          positive_at_to_(false) {}           // valore neutro, non viene mai letto
    

    // Costruttore "generatore": pos_node deve essere uno dei due estremi
    circuit_edge(const N& a, const N& b, const W& voltage, const N& pos_node)
        : un_edge<N, W>(a, b, voltage),
          kind_(Kind::Generator)
    {
        if      (pos_node == this->to())   positive_at_to_ = true;
        else if (pos_node == this->from()) positive_at_to_ = false;
        else throw std::invalid_argument("positive node must be an endpoint");
    }

    // Accessor
    Kind kind()           const { return kind_; }
    bool is_resistor()    const { return kind_ == Kind::Resistor; }
    bool is_generator()   const { return kind_ == Kind::Generator; }

    // Polo "+" come nodo (utile per stampe/debug)
    const N& positive_node() const {
        return positive_at_to_ ? this->to() : this->from();
    }

    // Segno del contributo se la maglia percorre l'arco nel verso canonico
    // (from_ → to_):
    //   to_ è "+" → la maglia entra da "−" ed esce su "+" → +1
    //   from_ è "+" → la maglia entra da "+" ed esce su "−" → −1
    int canonical_sign() const {
        return positive_at_to_ ? +1 : -1;
    }
};

