#include "../include/component.hpp"
void Capacitor::stamp(std::shared_ptr<Matrix<double>> circuit_matrix, std::shared_ptr<Matrix<double>> output_matrix) {
    std::cout << "goodbye" << std::endl;
}

void Capacitor::stamp(std::shared_ptr<Matrix<double>> circuit_matrix, std::shared_ptr<Matrix<double>> output_matrix, double frequency) {
    std::cout << "goodbye AC" << std::endl;
}