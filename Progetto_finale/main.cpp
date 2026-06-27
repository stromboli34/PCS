#include <iostream>
#include <string>
#include <Eigen/Dense>
#include "un_graph.hpp"
#include "auxiliary.hpp"
#include "system_constructor.hpp"
#include "conjugate_gradient.hpp"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <netlist.txt> [dfs|depina]\n";
        return 1;
    }
    std::string metodo;
    if (argc >= 3) {
        metodo = argv[2];
    } else {
        std::cout << "Seleziona il metodo per i cicli fondamentali:\n"
                  << "  1) dfs\n"
                  << "  2) depina\n"
                  << "Scelta [1/2]: ";
        std::string scelta;
        std::getline(std::cin, scelta);
        if (scelta == "2" || scelta == "depina") {
            metodo = "depina";
        } else {
            metodo = "dfs";
        }
        std::cout << "Metodo selezionato: " << metodo << "\n";
    }

    // parsing + costruzione grafo
    un_graph<int, double> G;
    auto net = read_netlist<int, double>(argv[1], G);

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
    Eigen::MatrixXd A = sys.B.transpose() * sys.R * sys.B;

    // risoluzione del sistema A * x = v
    auto result = conjugate_gradient(A, sys.v);
    if (!result.converged) {
        std::cerr << "CG non converge: iter=" << result.iterations
                  << ", res_rel=" << result.relative_residual << "\n";
        return 1;
    }

    // tensioni e correnti sui resistori (un'entrata per riga di B/R)
    Eigen::VectorXd vR = sys.R * sys.B * result.x;
    Eigen::VectorXd iR = sys.B * result.x;

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
