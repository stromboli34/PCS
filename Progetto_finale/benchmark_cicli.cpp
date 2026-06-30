#include <iostream>
#include <fstream>   // ofstream: per scrivere su file
#include <vector>
#include <ctime>     // clock(): per misurare il tempo

#include "un_graph.hpp"
#include "Cicli_dfs.hpp"
#include "Cicli_depina.hpp"

// Costruisce un grafo di prova deterministico (uguale a ogni esecuzione):
// un anello di n nodi piu' alcune corde che aggiungono cicli.
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

int main() {
    std::vector<int> taglie = {20, 40, 60, 80, 100};   // numero di nodi da provare
    int ripetizioni = 3;                               // ripeto e faccio la media

    std::ofstream out("tempi_cicli.txt");   // 3 colonne: nodi  tempo_DFS  tempo_DePina

    for (int n : taglie) {
        un_graph<int,double> g = grafo_di_prova(n, n / 4);

        std::vector<std::vector<int>> risultato;   // serve solo per cronometrare

        // cronometro DFS
        clock_t inizio = clock();
        for (int r = 0; r < ripetizioni; ++r) risultato = Cicli_dfs(g);
        double t_dfs = 1000.0 * (clock() - inizio) / CLOCKS_PER_SEC / ripetizioni;

        // cronometro De Pina
        inizio = clock();
        for (int r = 0; r < ripetizioni; ++r) risultato = Cicli_depina(g);
        double t_dp = 1000.0 * (clock() - inizio) / CLOCKS_PER_SEC / ripetizioni;

        // stampa semplice a video e scrittura nel file
        std::cout << "nodi=" << n << "  DFS=" << t_dfs << " ms  DePina=" << t_dp << " ms\n";
        out << n << " " << t_dfs << " " << t_dp << "\n";
    }

    out.close();
    std::cout << "\nTempi salvati in 'tempi_cicli.txt'.\n";
    return 0;
}
