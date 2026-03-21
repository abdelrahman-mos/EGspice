#include "../include/component.hpp"
void Vsource::stamp(std::shared_ptr<Matrix<double>> circuit_matrix) {
    std::cout << "hello" << std::endl;
}

void Vsource::stamp(std::shared_ptr<Matrix<double>> circuit_matrix, double frequency) {
    std::cout << "hello AC" << std::endl;
}