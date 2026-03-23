#include "../../include/component.hpp"
void Vsource::stamp(std::shared_ptr<Matrix<double>> circuit_matrix, std::shared_ptr<Matrix<double>> output_matrix, int num_vsources) {
    size_t stamp_index = circuit_matrix->numRows() - num_vsources + vsource_id;
    size_t node_1 = terminals[0];
    size_t node_2 = terminals[1];

    if (node_1 != 0) {
        (*circuit_matrix)[node_1-1][stamp_index] += 1;
        (*circuit_matrix)[stamp_index][node_1-1] += 1;
    }

    if (node_2 != 0) {
        (*circuit_matrix)[node_2-1][stamp_index] -= 1;
        (*circuit_matrix)[stamp_index][node_2-1] -= 1;
    }

    (*output_matrix)[stamp_index][0] += value;
}

void Vsource::stamp(std::shared_ptr<Matrix<double>> circuit_matrix, std::shared_ptr<Matrix<double>> output_matrix, int num_vsources, double frequency) {
    size_t stamp_index = circuit_matrix->numRows() - num_vsources + vsource_id;
    size_t node_1 = terminals[0];
    size_t node_2 = terminals[1];

    if (node_1 != 0) {
        (*circuit_matrix)[node_1-1][stamp_index] += 1;
        (*circuit_matrix)[stamp_index][node_1-1] += 1;
    }

    if (node_2 != 0) {
        (*circuit_matrix)[node_2-1][stamp_index] -= 1;
        (*circuit_matrix)[stamp_index][node_2-1] -= 1;
    }

    (*output_matrix)[stamp_index][0] += ac_val;
}