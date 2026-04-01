#include "../../include/component.hpp"
void CCVS::stamp(std::shared_ptr<Matrix<double>> circuit_matrix, std::shared_ptr<Matrix<double>> output_matrix, int num_vsources, int num_inductors) {
    size_t stamp_index = circuit_matrix->numRows() - num_vsources - num_inductors + id;
    // current between nodes 1 and 2 (pq)
    size_t node_1 = terminals_int[0];
    size_t node_2 = terminals_int[1];
    // controlling current through nodes 3 and 4 (kl)
    size_t node_3 = terminals_int[2];
    size_t node_4 = terminals_int[3];

    if (node_1 != 0) {
        (*circuit_matrix)[node_1-1][stamp_index+1] += 1;
        (*circuit_matrix)[stamp_index+1][node_1-1] += 1;
    }

    if (node_2 != 0) {
        (*circuit_matrix)[node_2-1][stamp_index+1] -= 1;
        (*circuit_matrix)[stamp_index+1][node_2-1] -= 1;
    }

    if (node_3 != 0) {
        (*circuit_matrix)[node_3-1][stamp_index] += 1;
        (*circuit_matrix)[stamp_index][node_3-1] += 1;
    }

    if (node_4 != 0) {
        (*circuit_matrix)[node_4-1][stamp_index] -= 1;
        (*circuit_matrix)[stamp_index][node_4-1] -= 1;
    }

    (*circuit_matrix)[stamp_index+1][stamp_index] -= value;
}

void CCVS::stamp(std::shared_ptr<Matrix<std::complex<double>>> circuit_matrix, std::shared_ptr<Matrix<std::complex<double>>> output_matrix, 
    int num_vsources, double frequency, double prev_frequency) {
    size_t stamp_index = circuit_matrix->numRows() - num_vsources + id;
    // current between nodes 1 and 2 (pq)
    size_t node_1 = terminals_int[0];
    size_t node_2 = terminals_int[1];
    // controlling current through nodes 3 and 4 (kl)
    size_t node_3 = terminals_int[2];
    size_t node_4 = terminals_int[3];

    if (node_1 != 0) {
        (*circuit_matrix)[node_1-1][stamp_index+1] += 1;
        (*circuit_matrix)[stamp_index+1][node_1-1] += 1;
    }

    if (node_2 != 0) {
        (*circuit_matrix)[node_2-1][stamp_index+1] -= 1;
        (*circuit_matrix)[stamp_index+1][node_2-1] -= 1;
    }

    if (node_3 != 0) {
        (*circuit_matrix)[node_3-1][stamp_index] += 1;
        (*circuit_matrix)[stamp_index][node_3-1] += 1;
    }

    if (node_4 != 0) {
        (*circuit_matrix)[node_4-1][stamp_index] -= 1;
        (*circuit_matrix)[stamp_index][node_4-1] -= 1;
    }

    (*circuit_matrix)[stamp_index+1][stamp_index] -= ac_val;
}

std::shared_ptr<Component> CCVS::clone() const {
    return std::make_shared<CCVS>(*this);
}