#include "auxiliary.hpp"

struct circuit_system {
    Eigen::MatrixXd B;
    Eigen::MatrixXd R;
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
    sys.R = Eigen::MatrixXd::Zero(n_R, n_R);
    sys.v = Eigen::VectorXd::Zero(n_C);

    for (int i = 0; i < n_C; ++i) {
        const auto& ciclo_corrente = cycles[i];
        int num_nodi = static_cast<int>(ciclo_corrente.size());

        for (int j = 0; j < num_nodi; ++j) {
            N nodo_partenza = ciclo_corrente[j];
            N nodo_arrivo = ciclo_corrente[(j + 1) % num_nodi];

            try {
                int indice_arco = G.edge_number(nodo_partenza, nodo_arrivo);
                const component<N, W>& comp = components[indice_arco];
                int segno = (comp.get_edge().from() == nodo_partenza) ? 1 : -1;

                if (comp.kind() == Kind::R) {
                    sys.R(comp.number(), comp.number()) = comp.get_edge().weight();
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
