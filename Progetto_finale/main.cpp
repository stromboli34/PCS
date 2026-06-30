#include <iostream>
#include <string>
#include <stdexcept>
#include <Eigen/Dense>
#include "un_graph.hpp"
#include "input_read.hpp"
#include "Cicli_dfs.hpp"
#include "Cicli_depina.hpp"
#include "system_constructor.hpp"
#include "conjugate_gradient.hpp"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <netlist.txt> [dfs|depina]\n";
        return 1;
    }
    if (argc > 3) {
        // Sono previsti al più due argomenti (netlist + metodo): avviso e
        // proseguo usando solo argv[1] e argv[2], gli altri li ignoro.
        std::cerr << "Avviso: ricevuti " << (argc - 1)
                  << " argomenti, uso solo i primi due e ignoro gli altri.\n";
    }
    std::string metodo;
    if (argc >= 3) {
        metodo = argv[2];
    } else {
        std::cout << "Seleziona il metodo per i cicli fondamentali:\n"
                  << "  1) Dfs\n"
                  << "  2) DePina\n"
                  << "Scelta [1/2]: ";
        std::string scelta;
        std::getline(std::cin, scelta);
        if (scelta == "1" || scelta == "Dfs") {
            metodo = "dfs";
        } else if (scelta == "2" || scelta == "DePina") {
            metodo = "depina";
        } else {
            std::cerr << "Scelta non valida: " << scelta << " (usa 1/Dfs o 2/Depina)\n";
            return 1;
        }
        std::cout << "Metodo selezionato: " << metodo << "\n";
    }

    // parsing + costruzione grafo
    un_graph<int, double> G;
    netlist<int, double> net;
    try {
        net = read_netlist<int, double>(argv[1], G);
    } catch (const std::exception& e) {
        // es. file inesistente/non leggibile: read_netlist lancia runtime_error
        std::cerr << "Errore nella lettura della netlist: " << e.what() << "\n";
        return 1;
    }

    // scelta del metodo per i cicli fondamentali
    std::vector<std::vector<int>> cicli;
    if (metodo == "dfs") {
        cicli = Cicli_dfs(G);
    } else if (metodo == "depina") {
        cicli = Cicli_depina(G);
    } else {
        std::cerr << "Metodo sconosciuto: " << metodo << " (usa dfs o depina)\n";
        return 1;
    }

    // matrici B, R, v (build_system definita in system_constructor.hpp)
    auto sys = build_system<int, double>(net.components,
                                         static_cast<int>(net.resistor_count),
                                         cicli, G);

    // A = B^T R B, SDP -> applicabile gradiente coniugato
	Eigen::MatrixXd A = sys.B.transpose() * sys.r.asDiagonal() * sys.B;	

    // risoluzione del sistema A * x = v
    auto result = conjugate_gradient(A, sys.v);
    if (!result.converged) {
        std::cerr << "CG non converge: iter=" << result.iterations
                  << ", res_rel=" << result.relative_residual << "\n";
        return 1;
    }

    // tensioni e correnti sui resistori (un'entrata per riga di B/R)
    Eigen::VectorXd iR = sys.B * result.x;
	Eigen::VectorXd vR = sys.r.asDiagonal() * iR;   // = R * iR
	
    // Stampa: itera 'components' in ordine lessicografico (l'ordine di
    // selectionSort in read_netlist), ma indicizza vR/iR con c.number() perché
    // la riga di B/R è quella, non la posizione di iterazione. Il +1 ricompensa
    // lo shift a 0-based fatto in input_read, ricostruendo "R1", "R2", ...
    for (const auto& c : net.components) {
        if (c.kind() == Kind::R) {
            std::cout << "R" << (c.number() + 1)
                      << ": V = " << vR(c.number()) << " volts, "
                      << "I = " << iR(c.number()) << " amps.\n";
        }
    }
    return 0;
}
