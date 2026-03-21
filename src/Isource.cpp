#include "../include/component.hpp"
void Isource::stamp(std::shared_ptr<Matrix<double>> circuit_matrix, std::shared_ptr<Matrix<double>> output_matrix) {
    std::cout << "hello" << std::endl;
}

void Isource::stamp(std::shared_ptr<Matrix<double>> circuit_matrix, std::shared_ptr<Matrix<double>> output_matrix, double frequency) {
    std::cout << "hello AC" << std::endl;
}