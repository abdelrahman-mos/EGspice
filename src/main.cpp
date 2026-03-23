#include "../include/component.hpp"
#include "../include/Matrix.hpp"
#include "../include/Parser.hpp"
#include "../include/Circuit.hpp"
#include <memory>

int main() {
    Parser parser("test_netlists/netlist1.cir");
    std::unique_ptr<Circuit> circuit = parser.parse();
    circuit->stamp_circuit();
    auto circuit_matrix = circuit->get_matrix();
    std::cout << circuit_matrix << std::endl;
    auto output_matrix = circuit->get_output_matrix();
    std::cout << output_matrix << std::endl;
    auto output = Matrix<double>::solve_matrix(circuit_matrix, output_matrix);
    std::cout << output << std::endl;
}