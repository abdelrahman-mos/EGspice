#include "../include/Circuit.hpp"
#include "../include/Command.hpp"

void OP::run(std::shared_ptr<Circuit> circuit) {
    circuit->stamp_circuit();
    auto circuit_matrix = circuit->get_matrix();
    auto output_matrix = circuit->get_output_matrix();
    auto outputs = Matrix<double>::solve_matrix(circuit_matrix, output_matrix);
    std::cout << outputs << std::endl;
    return;
}