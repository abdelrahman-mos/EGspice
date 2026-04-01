#include "../../include/component.hpp"
void VCCS::stamp(std::shared_ptr<Matrix<double>> circuit_matrix, std::shared_ptr<Matrix<double>> output_matrix, int num_vsources, int num_inductors) {
    // current between nodes 1 and 2 (pq)
    size_t node_1 = terminals_int[0];
    size_t node_2 = terminals_int[1];
    // controlling voltage nodes 3 and 4 (kl)
    size_t node_3 = terminals_int[2];
    size_t node_4 = terminals_int[3];

    if (node_1 != 0) {
        if (node_3 != 0) {
            // row p col k
            (*circuit_matrix)[node_1-1][node_3-1] += value;
        }

        if (node_4 != 0) {
            // row p col l
            (*circuit_matrix)[node_1-1][node_4-1] -= value;
        }
    }

    if (node_2 != 0) {
        if (node_3 != 0) {
            // row q col k
            (*circuit_matrix)[node_2-1][node_3-1] -= value;
        }

        if (node_4 != 0) {
            // row q col l
            (*circuit_matrix)[node_2-1][node_4-1] += value;
        }
    }
}

void VCCS::stamp(std::shared_ptr<Matrix<std::complex<double>>> circuit_matrix, std::shared_ptr<Matrix<std::complex<double>>> output_matrix, 
    int num_vsources, double frequency, double prev_frequency) {
    // current between nodes 1 and 2 (pq)
    size_t node_1 = terminals_int[0];
    size_t node_2 = terminals_int[1];
    // controlling voltage nodes 3 and 4 (kl)
    size_t node_3 = terminals_int[2];
    size_t node_4 = terminals_int[3];

    if (node_1 != 0) {
        if (node_3 != 0) {
            // row p col k
            (*circuit_matrix)[node_1-1][node_3-1] += ac_val;
        }

        if (node_4 != 0) {
            // row p col l
            (*circuit_matrix)[node_1-1][node_4-1] -= ac_val;
        }
    }

    if (node_2 != 0) {
        if (node_3 != 0) {
            // row q col k
            (*circuit_matrix)[node_2-1][node_3-1] -= ac_val;
        }

        if (node_4 != 0) {
            // row q col l
            (*circuit_matrix)[node_2-1][node_4-1] += ac_val;
        }
    }
}

std::shared_ptr<Component> VCCS::clone() const {
    return std::make_shared<VCCS>(*this);
}