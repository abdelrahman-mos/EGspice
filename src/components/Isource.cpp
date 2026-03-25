#include "../../include/component.hpp"
void Isource::stamp(std::shared_ptr<Matrix<double>> circuit_matrix, std::shared_ptr<Matrix<double>> output_matrix, int num_vsources, int num_inductors) {
    int node_1 = terminals_int[0];
    int node_2 = terminals_int[1];

    if (node_1 != 0) {
        (*output_matrix)[node_1-1][0] += value;
    }

    if (node_2 != 0) {
        (*output_matrix)[node_2-1][0] -= value;
    }
}

void Isource::stamp(std::shared_ptr<Matrix<std::complex<double>>> circuit_matrix, std::shared_ptr<Matrix<std::complex<double>>> output_matrix, int num_vsources, double frequency) {
    int node_1 = terminals_int[0];
    int node_2 = terminals_int[1];

    if (node_1 != 0) {
        (*output_matrix)[node_1-1][0] += ac_val;
    }

    if (node_2 != 0) {
        (*output_matrix)[node_2-1][0] -= ac_val;
    }
}