#include "../../include/component.hpp"
void Vsource::stamp(std::shared_ptr<Matrix<double>> circuit_matrix, std::shared_ptr<Matrix<double>> output_matrix, int num_vsources, 
    int num_inductors) {
    size_t stamp_index = circuit_matrix->numRows() - num_vsources - num_inductors + id;
    size_t node_1 = terminals_int[0];
    size_t node_2 = terminals_int[1];

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

void Vsource::stamp(std::shared_ptr<Matrix<double>> circuit_matrix, std::shared_ptr<Matrix<double>> output_matrix, double temp_value, 
    double prev_value, int num_vsources, int num_inductors) {
    // first point won't even be an option here
    size_t stamp_index = circuit_matrix->numRows() - num_vsources - num_inductors + id;
    (*output_matrix)[stamp_index][0] += (temp_value - prev_value);
}

void Vsource::stamp(std::shared_ptr<Matrix<std::complex<double>>> circuit_matrix, std::shared_ptr<Matrix<std::complex<double>>> output_matrix, 
    int num_vsources, double frequency, double prev_frequency) {
    size_t stamp_index = circuit_matrix->numRows() - num_vsources + id;
    size_t node_1 = terminals_int[0];
    size_t node_2 = terminals_int[1];

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

std::shared_ptr<Component> Vsource::clone() const {
    return std::make_shared<Vsource>(*this);
}