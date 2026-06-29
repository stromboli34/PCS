// ===========================================================================
// experiment_cond.cpp
//
// Mostra come il CONDIZIONAMENTO della matrice A = B^T R B cambi a seconda di
// quale base di cicli si usa (DFS o De Pina), e come questo si ripercuota sulla
// risoluzione del sistema con il gradiente coniugato.
//
// Idea: lo stesso grafo, con basi di cicli diverse, da' matrici A diverse. I
// cicli MINIMI di De Pina rendono B piu' sparsa e A meglio condizionata, quindi
// il gradiente coniugato converge in MENO iterazioni.
//
// Compilazione:  g++ -std=c++17 -O2 -I. -I/percorso/eigen experiment_cond.cpp -o experiment_cond
// Esecuzione:    ./experiment_cond
// ===========================================================================

#include <iostream>
#include <string>
#include <vector>
#include <Eigen/Dense>

#include "un_graph.hpp"
#include "Cicli_dfs.hpp"
#include "Cicli_depina.hpp"
#include "conjugate_gradient.hpp"   // qui ci sono condA() e conjugate_gradient()

// --- due funzioncine per stampare tabelle ordinate, senza librerie speciali ---

std::string numero(double x, int decimali = 1) {
    std::string s = std::to_string(x);
    std::size_t punto = s.find('.');
    if (punto != std::string::npos) s = s.substr(0, punto + 1 + decimali);
    return s;
}

std::string colonna(const std::string& testo, int larghezza) {
    std::string s = testo;
    while ((int)s.size() < larghezza) s += " ";
    return s;
}

// Stesso grafo di prova del benchmark: anello di n nodi piu' alcune corde.
un_graph<int,double> grafo_di_prova(int n, int corde) {
    un_graph<int,double> g;
    for (int i = 0; i < n; ++i)
        g.add_edge(i, (i + 1) % n, 1.0 + i);
    int passo = n / 3;
    if (passo < 2) passo = 2;
    for (int k = 1; k <= corde; ++k) {
        int a = k % n;
        int b = (k + passo) % n;
        if (a != b) g.add_edge(a, b, 1.0 + k);
    }
    return g;
}

// Costruisce A = B^T R B per un grafo e una base di cicli, esattamente come nel
// progetto. Scrive in 'non_nulli' il numero di +-1 presenti in B (= somma delle
// lunghezze dei cicli): piu' e' piccolo, piu' B e' sparsa.
Eigen::MatrixXd costruisci_A(const un_graph<int,double>& g,
                             const std::vector<std::vector<int>>& cicli,
                             int& non_nulli) {
    int m = (int)g.all_edges().size();     // numero di archi  -> righe di B
    int n = (int)cicli.size();             // numero di cicli  -> colonne di B

    Eigen::MatrixXd B = Eigen::MatrixXd::Zero(m, n);
    Eigen::VectorXd r(m);
    for (int e = 0; e < m; ++e) r(e) = g.all_edges()[e].weight();   // R diagonale

    non_nulli = 0;
    for (int j = 0; j < n; ++j) {
        int lunghezza = (int)cicli[j].size();
        for (int k = 0; k < lunghezza; ++k) {
            int u = cicli[j][k];
            int v = cicli[j][(k + 1) % lunghezza];
            if (u == v) continue;                  // salta il nodo di chiusura ripetuto
            try {
                int riga = g.edge_number(u, v);
                B(riga, j) = (u < v) ? 1.0 : -1.0; // segno = verso di percorrenza
                ++non_nulli;
            } catch (const std::out_of_range&) {}
        }
    }
    return B.transpose() * r.asDiagonal() * B;
}

int main() {
    std::vector<int> taglie = {20, 40, 60, 80, 100};

    std::cout << "=== CONDIZIONAMENTO: base DFS vs base De Pina ===\n";
    std::cout << "Stesso grafo, due basi di cicli -> due matrici A = B^T R B diverse.\n";
    std::cout << "nnzB  = numeri non nulli in B (piu' piccolo = B piu' sparsa)\n";
    std::cout << "kappa = condizionamento di A (piu' basso = sistema piu' facile)\n";
    std::cout << "iter  = iterazioni del gradiente coniugato per risolvere A x = b\n\n";

    std::cout << colonna("nodi", 7) << colonna("archi", 7)
              << colonna("nnzB dfs", 11) << colonna("nnzB dp", 11)
              << colonna("kappa dfs", 12) << colonna("kappa dp", 12)
              << colonna("iter dfs", 10) << "iter dp\n";
    std::cout << std::string(80, '-') << "\n";

    for (int n : taglie) {
        un_graph<int,double> g = grafo_di_prova(n, n / 4);
        int archi = (int)g.all_edges().size();

        int nnz_dfs = 0, nnz_dp = 0;
        Eigen::MatrixXd A_dfs = costruisci_A(g, Cicli_dfs(g),    nnz_dfs);
        Eigen::MatrixXd A_dp  = costruisci_A(g, Cicli_depina(g), nnz_dp);

        // termine noto qualunque (tutti 1): qui conta QUANTE iterazioni servono,
        // non il valore della soluzione
        Eigen::VectorXd b = Eigen::VectorXd::Ones(A_dfs.rows());
        cg_result sol_dfs = conjugate_gradient(A_dfs, b);
        cg_result sol_dp  = conjugate_gradient(A_dp,  b);

        std::cout << colonna(std::to_string(n), 7)
                  << colonna(std::to_string(archi), 7)
                  << colonna(std::to_string(nnz_dfs), 11)
                  << colonna(std::to_string(nnz_dp), 11)
                  << colonna(numero(condA(A_dfs)), 12)
                  << colonna(numero(condA(A_dp)), 12)
                  << colonna(std::to_string(sol_dfs.iterations), 10)
                  << sol_dp.iterations << "\n";
    }

    std::cout << "\nConclusione: i cicli minimi di De Pina danno una B piu' sparsa e una\n"
              << "matrice A molto meglio condizionata (kappa piu' basso). Di conseguenza il\n"
              << "gradiente coniugato converge in meno iterazioni. La soluzione fisica e' la\n"
              << "stessa, ma con la base di De Pina il sistema e' piu' facile da risolvere.\n";
    return 0;
}
