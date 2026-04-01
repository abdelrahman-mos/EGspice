#include "../../include/component.hpp"
void Resistor::stamp(std::shared_ptr<Matrix<double>> circuit_matrix, std::shared_ptr<Matrix<double>> output_matrix, int num_vsources, int num_inductors) {
    size_t node_1 = terminals_int[0];
    size_t node_2 = terminals_int[1];

    double conductance = 1.0/value;

    if (node_1 != 0) {
        (*circuit_matrix)[node_1-1][node_1-1] += conductance;
    }

    if (node_2 != 0) {
        (*circuit_matrix)[node_2-1][node_2-1] += conductance;
    }

    if ((node_1 != 0) && (node_2 != 0)) {
        (*circuit_matrix)[node_1-1][node_2-1] -= conductance;
        (*circuit_matrix)[node_2-1][node_1-1] -= conductance;
    }
}

void Resistor::stamp(std::shared_ptr<Matrix<std::complex<double>>> circuit_matrix, std::shared_ptr<Matrix<std::complex<double>>> output_matrix, 
    int num_vsources, double frequency, double prev_frequency) {
    size_t node_1 = terminals_int[0];
    size_t node_2 = terminals_int[1];

    double conductance = 1.0/ac_val;

    if (node_1 != 0) {
        (*circuit_matrix)[node_1-1][node_1-1] += conductance;
    }

    if (node_2 != 0) {
        (*circuit_matrix)[node_2-1][node_2-1] += conductance;
    }

    if ((node_1 != 0) && (node_2 != 0)) {
        (*circuit_matrix)[node_1-1][node_2-1] -= conductance;
        (*circuit_matrix)[node_2-1][node_1-1] -= conductance;
    }
}

std::shared_ptr<Component> Resistor::clone() const {
    return std::make_shared<Resistor>(*this);
}