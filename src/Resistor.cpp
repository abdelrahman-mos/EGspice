#include "../include/component.hpp"
void Resistor::stamp(std::shared_ptr<Matrix<double>> circuit_matrix, std::shared_ptr<Matrix<double>> output_matrix) {
    int node_1 = terminals[0];
    int node_2 = terminals[1];

    if (node_1 != 0) {
        (*circuit_matrix)[node_1-1][node_1-1] += 1.0/value;
    }

    if (node_2 != 0) {
        (*circuit_matrix)[node_2-1][node_2-1] += 1.0/value;
    }

    if ((node_1 != 0) && (node_2 != 0)) {
        (*circuit_matrix)[node_1-1][node_2-1] -= 1.0/value;
        (*circuit_matrix)[node_2-1][node_1-1] -= 1.0/value;
    }
}

void Resistor::stamp(std::shared_ptr<Matrix<double>> circuit_matrix, std::shared_ptr<Matrix<double>> output_matrix, double frequency) {
    std::cout << "goodbye AC" << std::endl;
}