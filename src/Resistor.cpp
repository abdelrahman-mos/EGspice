#include "../include/component.hpp"
void Resistor::stamp(std::shared_ptr<Matrix<double>> circuit_matrix) {
    std::cout << terminals.size() << std::endl;
    std::cout << "goodbye" << std::endl;
}

void Resistor::stamp(std::shared_ptr<Matrix<double>> circuit_matrix, double frequency) {
    std::cout << "goodbye AC" << std::endl;
}