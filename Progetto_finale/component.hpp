#pragma once

#include "un_graph.hpp"

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
          //Se è un resistore mettiamo sempre true, così dipenderà unicamente dall'orientazione della maglia,
          //che sarà true se la maglia va in ordine crescente e false se va in ordine decrescente,
          //così da contribuire con un +1 o -1 in B.
          //Se invece la componente è un generatore mettiamo true se from > to, ossia se il primo nodo che ci viene dato
          //(quello dove sta la carica positiva) è più grande del secondo: in questo modo se la maglia è percorsa in ordine crescente (true),
          //essa attraversà il generatore dal - a +, contribuendo positivamente nella costruzione di v
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
