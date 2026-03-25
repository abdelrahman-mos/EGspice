#include "../../include/component.hpp"
void Inductor::stamp(std::shared_ptr<Matrix<double>> circuit_matrix, std::shared_ptr<Matrix<double>> output_matrix, int num_vsources) {
    std::cout << "goodbye" << std::endl;
}

void Inductor::stamp(std::shared_ptr<Matrix<std::complex<double>>> circuit_matrix, std::shared_ptr<Matrix<std::complex<double>>> output_matrix, int num_vsources, double frequency) {
    std::cout << "goodbye AC" << std::endl;
}