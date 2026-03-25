#include "../../include/component.hpp"
void Capacitor::stamp(std::shared_ptr<Matrix<double>> circuit_matrix, std::shared_ptr<Matrix<double>> output_matrix, int num_vsources, int num_inductors) {
    std::cout << "goodbye" << std::endl;
}

void Capacitor::stamp(std::shared_ptr<Matrix<std::complex<double>>> circuit_matrix, std::shared_ptr<Matrix<std::complex<double>>> output_matrix, int num_vsources, double frequency) {
    size_t node_1 = terminals_int[0];
    size_t node_2 = terminals_int[1];
    
    if (node_1 != 0) {
        (*circuit_matrix)[node_1-1][node_1-1] += std::complex<double>(0.0, 2.0*M_PI*frequency*value);
    }

    if (node_2 != 0) {
        (*circuit_matrix)[node_2-1][node_2-1] += std::complex<double>(0.0, 2.0*M_PI*frequency*value);
    }

    if ((node_1 != 0) && (node_2 != 0)) {
        (*circuit_matrix)[node_1-1][node_2-1] += std::complex<double>(0.0, -2.0*M_PI*frequency*value);
        (*circuit_matrix)[node_2-1][node_1-1] += std::complex<double>(0.0, -2.0*M_PI*frequency*value);
    }
}