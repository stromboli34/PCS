#pragma once

#include<iostream>
#include<stdexcept> //per invalid_argument

template<typename N, typename W = double>
class un_edge {
    private:
        N from_;
        N to_;
        W weight_;

    public:
        //costruttore user defined
        un_edge(const N& a, const N& b, const W& w = W{1}) : weight_(w) { //W{1} come default: se non passi un peso l'arco vale 1 (comportamento "grafo non pesato"). Uso la lista di inizializzazione: weight_ viene direttamente costruito copiando w, invece di essere prima default-costruito e poi assegnato. È anche necessario se W non ha un costruttore di default
            if (w < W{0}) throw std::invalid_argument("weight must be non-negative"); //vincolo richiesto da Dijkstra: pesi negativi farebbero risultati silenziosamente sbagliati
            if (a < b) {
                from_ = a;
                to_ = b;
            }
            else { //riordino
                from_ = b;
                to_ = a;
            }
        }

        //metodi per ottenedere i nodi e il peso dell'arco
        const N& from() const {return from_; } //prendo un riferimento costante non una copia (meglio per tipi costosi)
        const N& to() const {return to_; }
        const W& weight() const {return weight_; }

        //operatore di confronto stretto (lessicografico) tra archi: il peso è metadato, non entra nel confronto (grafo semplice: identità dell'arco è (from, to))
        bool operator<(const un_edge& other) const { //non serve controllo dei tipi perchè in C++ avviene durante la compilazione, non a runtime (che è quando il controllo scritto qui mi darebbe errore), se i tipi sono diversi già in compilazione me ne accorgo
            if (from_ != other.from()) return from_ < other.from(); //potrei anche usare other.from_ (questo non è possibile nel main perchè le variabili from_ e to_ sono private, ma nella definizione di un metodo della classe posso accedervi)
            else return to_ < other.to(); //other.from_ è un accesso più diretto mentre other.from() deve andare a trovare il codice della classe dove ho definito il metodo from()
        }

        //operatore di confronto ==
        bool operator==(const un_edge& other) const {
            return (from_ == other.from() && to_ == other.to());
        }
};

//funzione libera ossia operatore definito con membro di sinistra non this ma qualche membro di una classe non mia (in questo caso classe della libreria standard: std::ostream) dunque non posso tenerlo dentro alla mia classe dove i membri di sinistra sono sempre automaticamente  this
//qua quindi devo esplicitare due parametri
template<typename N, typename W>
std::ostream& operator<<(std::ostream& os, const un_edge<N, W>& e) { // prendo un riferimento non costante a os, modifico os e lo returno; invece il riferimento a un_edge deve essere istanziato con <N, W> perchè non essendo più all'interno della classe siamo fuori dallo scope
    os << "(" << e.from() << ", " << e.to() << ", w=" << e.weight() << ")"; //qua mi darebbe errore se usassi e.from_ perchè non vi ho accesso
    return os;
}
