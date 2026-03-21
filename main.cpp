#include "include/component.hpp"
#include "include/Matrix.hpp"
#include "include/Parser.hpp"
#include "include/Circuit.hpp"
#include <memory>

int main() {
    Parser parser("test/test.cir");
    std::unique_ptr<Circuit> circuit = parser.parse();
    circuit->stamp_circuit();
    auto circuit_matrix = circuit->get_matrix();
    std::cout << circuit_matrix << std::endl;
}