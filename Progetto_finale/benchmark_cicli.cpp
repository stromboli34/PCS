// ===========================================================================
// benchmark_cicli.cpp
//
// Confronta i TEMPI dei due metodi che trovano i cicli fondamentali (DFS e
// De Pina) al crescere della dimensione del grafo. Stampa una tabella e un
// piccolo grafico a barre (fatto solo di caratteri '#') per vedere a colpo
// d'occhio quanto piu' lento diventa De Pina.
//
// Compilazione:  g++ -std=c++17 -O2 -I. -I/percorso/eigen benchmark_cicli.cpp -o benchmark_cicli
// Esecuzione:    ./benchmark_cicli
// ===========================================================================

#include <iostream>
#include <string>
#include <vector>
#include <ctime>     // clock(): il cronometro piu' semplice del C++

#include "un_graph.hpp"
#include "Cicli_dfs.hpp"
#include "Cicli_depina.hpp"

// --- due funzioncine per stampare tabelle ordinate, senza librerie speciali ---

// trasforma un numero in stringa con un numero fisso di cifre decimali
std::string numero(double x, int decimali = 2) {
    std::string s = std::to_string(x);          // es. "3.025000"
    std::size_t punto = s.find('.');
    if (punto != std::string::npos) s = s.substr(0, punto + 1 + decimali);
    return s;                                    // es. "3.02"
}

// completa una stringa con spazi finche' non raggiunge 'larghezza' caratteri,
// cosi' le colonne risultano allineate
std::string colonna(const std::string& testo, int larghezza) {
    std::string s = testo;
    while ((int)s.size() < larghezza) s += " ";
    return s;
}

// Costruisce un grafo di prova DETERMINISTICO (uguale a ogni esecuzione):
// un anello di n nodi 0-1-2-...-(n-1)-0 piu' alcune "corde" che lo attraversano.
// L'anello garantisce che il grafo sia connesso; ogni corda aggiunge un ciclo.
un_graph<int,double> grafo_di_prova(int n, int corde) {
    un_graph<int,double> g;
    for (int i = 0; i < n; ++i)
        g.add_edge(i, (i + 1) % n, 1.0 + i);     // arco dell'anello (peso qualsiasi > 0)
    int passo = n / 3;
    if (passo < 2) passo = 2;
    for (int k = 1; k <= corde; ++k) {
        int a = k % n;
        int b = (k + passo) % n;
        if (a != b) g.add_edge(a, b, 1.0 + k);   // se l'arco esiste gia', add_edge lo ignora
    }
    return g;
}

// stampa una barra fatta di '#', lunga in proporzione al valore (max 50 caratteri)
void barra(double valore, double valore_massimo) {
    int lunghezza = (valore_massimo > 0) ? (int)(valore / valore_massimo * 50) : 0;
    std::cout << std::string(lunghezza, '#');
}

int main() {
    std::vector<int> taglie = {20, 40, 60, 80, 100};   // numero di nodi da provare
    int ripetizioni = 3;                               // ripeto e faccio la media

    std::cout << "=== TEMPI: ricerca dei cicli fondamentali (DFS vs De Pina) ===\n";
    std::cout << "Grafi di dimensione crescente. Tempi medi in millisecondi.\n\n";

    // intestazione della tabella
    std::cout << colonna("nodi", 7) << colonna("archi", 7) << colonna("cicli", 7)
              << colonna("DFS [ms]", 12) << colonna("DePina [ms]", 14) << "DePina/DFS\n";
    std::cout << std::string(58, '-') << "\n";

    std::vector<double> tempi_dfs;     // li salvo per disegnare il grafico dopo
    std::vector<double> tempi_dp;

    for (int n : taglie) {
        un_graph<int,double> g = grafo_di_prova(n, n / 4);
        int archi = (int)g.all_edges().size();
        int cicli = archi - n + 1;     // grafo connesso: |E| - |V| + 1

        // cronometro DFS (assegno il risultato cosi' il compilatore non lo elimina)
        std::vector<std::vector<int>> risultato;
        clock_t inizio = clock();
        for (int r = 0; r < ripetizioni; ++r) risultato = Cicli_dfs(g);
        clock_t fine = clock();
        double t_dfs = 1000.0 * (fine - inizio) / CLOCKS_PER_SEC / ripetizioni;

        // cronometro De Pina
        inizio = clock();
        for (int r = 0; r < ripetizioni; ++r) risultato = Cicli_depina(g);
        fine = clock();
        double t_dp = 1000.0 * (fine - inizio) / CLOCKS_PER_SEC / ripetizioni;

        tempi_dfs.push_back(t_dfs);
        tempi_dp.push_back(t_dp);

        std::cout << colonna(std::to_string(n), 7)
                  << colonna(std::to_string(archi), 7)
                  << colonna(std::to_string(cicli), 7)
                  << colonna(numero(t_dfs), 12)
                  << colonna(numero(t_dp), 14)
                  << numero(t_dp / t_dfs, 1) << "x\n";
    }

    // ----------------- grafico a barre -----------------
    double massimo = 0;
    for (double t : tempi_dp) if (t > massimo) massimo = t;

    std::cout << "\nGrafico dei tempi (la barra piu' lunga = " << numero(massimo, 1) << " ms):\n\n";
    for (std::size_t i = 0; i < taglie.size(); ++i) {
        std::cout << "n = " << colonna(std::to_string(taglie[i]), 4) << "\n";
        std::cout << "  DFS    | ";  barra(tempi_dfs[i], massimo);  std::cout << "  " << numero(tempi_dfs[i]) << " ms\n";
        std::cout << "  DePina | ";  barra(tempi_dp[i],  massimo);  std::cout << "  " << numero(tempi_dp[i])  << " ms\n\n";
    }

    std::cout << "Conclusione: al crescere del grafo la DFS resta velocissima, mentre\n"
              << "De Pina rallenta molto in fretta. E' il prezzo da pagare per avere i\n"
              << "cicli MINIMI invece di cicli qualunque.\n";
    return 0;
}
