#pragma once 
#include "input_read.hpp"
#include "un_graph.hpp"
#include <Eigen/Dense>

struct circuit_system {
    Eigen::MatrixXd B;
    Eigen::VectorXd r;
    Eigen::VectorXd v;
};

template <typename N, typename W>
circuit_system build_system(const std::vector<component<N, W>>& components,
                            int resistor_count,
                            const std::vector<std::vector<N>>& cycles,
                            const un_graph<N, W>& G) {
    int n_R = resistor_count;
    int n_C = static_cast<int>(cycles.size());

    circuit_system sys;
    sys.B = Eigen::MatrixXd::Zero(n_R, n_C);
    sys.r = Eigen::VectorXd::Zero(n_R);
    sys.v = Eigen::VectorXd::Zero(n_C);

    for (int i = 0; i < n_C; ++i) {
        const auto& ciclo_corrente = cycles[i];
        int num_nodi = static_cast<int>(ciclo_corrente.size());

        // I cicli arrivano già chiusi: il primo nodo è ripetuto in fondo (es. [start, ..., start]), quindi l'arco di chiusura è già la coppia
        // (ciclo[num_nodi-2], ciclo[num_nodi-1]). Per questo itero fino a num_nodi-1 leggendo direttamente ciclo[j+1], senza modulo (%)
        for (int j = 0; j < num_nodi - 1; ++j) {
            N nodo_partenza = ciclo_corrente[j];
            N nodo_arrivo = ciclo_corrente[j + 1];

            // edge_number lancia std::out_of_range se (partenza, arrivo) non è un arco di G. Qui tutte le coppie consecutive sono archi reali (provengono dall'albero o dalla corda che chiude la maglia), quindi
            // il catch è una rete di sicurezza difensiva (in realtà un refuso per saltare vecchi bug dovuti al modulo % nella numerazione)
            try {
                int indice_arco = G.edge_number(nodo_partenza, nodo_arrivo);
                const component<N, W>& comp = components[indice_arco];
                int segno = (comp.get_edge().from() == nodo_partenza) ? 1 : -1;

                if (comp.kind() == Kind::R) {
                    sys.r(comp.number()) = comp.get_edge().weight();
                    sys.B(comp.number(), i) = segno;
                } else if (comp.kind() == Kind::V) {
                    int segno_v = segno * (comp.is_oriented() ? 1 : -1);
                    sys.v(i) += comp.get_edge().weight() * segno_v;
                }
            } catch (const std::out_of_range&) {
            }
        }
    }

    return sys;
}
