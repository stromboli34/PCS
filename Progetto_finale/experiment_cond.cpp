#include <iostream>
#include <iomanip>
#include <vector>
#include <Eigen/Dense>
#include "un_graph.hpp"
#include "Cicli_dfs.hpp"
#include "Cicli_depina.hpp"
#include "conjugate_gradient.hpp"

// Grafo di prova DETERMINISTICO (uguale a ogni esecuzione, niente numeri casuali):
// un anello 0-1-2-...-(n-1)-0 che garantisce la connessione, piu' alcune "corde"
// che lo attraversano: ogni corda aggiunge un ciclo. I pesi (1.0+i, 1.0+k) variano
// da arco ad arco cosi' fanno da resistenze diverse tra loro.
un_graph<int, double> grafo_di_prova(int n, int corde) {
    un_graph<int, double> g;
    for (int i = 0; i < n; ++i)
        g.add_edge(i, (i + 1) % n, 1.0 + i);     // arco dell'anello
    int passo = n / 3;
    if (passo < 2) passo = 2;
    for (int k = 1; k <= corde; ++k) {
        int a = k % n;
        int b = (k + passo) % n;
        if (a != b) g.add_edge(a, b, 1.0 + k);   // se l'arco esiste gia', add_edge lo ignora
    }
    return g;
}

// Costruisce B (m righe = archi, n colonne = cicli) dalla lista dei cicli.
// Segno +1/-1 = verso con cui il ciclo percorre l'arco. Conta i non-zeri in nnz.
Eigen::MatrixXd buildB(const un_graph<int, double>& g,
                       const std::vector<std::vector<int>>& cicli, long& nnz) {
    int m = g.all_edges().size();
	int n = cicli.size();
    Eigen::MatrixXd B = Eigen::MatrixXd::Zero(m, n);
    nnz = 0;
    for (int j = 0; j < n; ++j)                                  // un ciclo per colonna
        for (std::size_t k = 0; k + 1 < cicli[j].size(); ++k) { // i cicli arrivano gia' chiusi
            int u = cicli[j][k], v = cicli[j][k + 1];
            B(g.edge_number(u, v), j) = (u < v) ? +1.0 : -1.0;
            ++nnz;
        }
    return B;
}

int main() {
    std::cout << "Confronto delle due basi di cicli: DFS (cicli qualunque) vs De Pina (cicli minimi)\n\n";
    std::cout << "nodi\tarchi\tcicli\tnnzDFS\tnnzDP\tcondDFS\tcondDP\n";
    std::cout << "--------------------------------------------------------\n";
    // {numero di nodi, numero di corde}: piu' corde -> piu' cicli
    for (auto [V, corde] : std::vector<std::pair<int, int>>{{20, 10}, {40, 20}, {60, 30}, {90, 45}, {120, 60}}) {
        un_graph<int, double> g = grafo_di_prova(V, corde);
        int m = g.all_edges().size();   
        // R diagonale = i pesi degli archi (le resistenze)
        Eigen::VectorXd r(m);
        for (int e = 0; e < m; ++e) r(e) = g.all_edges()[e].weight();

        long nnz_dfs = 0, nnz_dp = 0;
        Eigen::MatrixXd Bd = buildB(g, Cicli_dfs(g),    nnz_dfs);
        Eigen::MatrixXd Bp = buildB(g, Cicli_depina(g), nnz_dp);

        Eigen::MatrixXd Ad = Bd.transpose() * r.asDiagonal() * Bd; // A = B^T R B
        Eigen::MatrixXd Ap = Bp.transpose() * r.asDiagonal() * Bp;

        std::cout << std::fixed << std::setprecision(1)
                  << V << "\t" << m << "\t" << (m - V + 1) << "\t"
                  << nnz_dfs << "\t" << nnz_dp << "\t"
                  << condA(Ad) << "\t" << condA(Ap) << "\n";
    }

    std::cout << "\nnnz piu' basso  = B piu' sparsa.\n"
              << "cond piu' basso = sistema piu' facile e preciso da risolvere col gradiente coniugato.\n";
    return 0;
}
