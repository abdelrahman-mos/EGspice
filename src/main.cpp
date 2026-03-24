#include "../include/Simulator.hpp"
#include <memory>

int main(int argc, char* argv[]) {
    Simulator simulator = Simulator(argc, argv);
    simulator.run();
    // Parser parser = Parser();
    // std::unique_ptr<Circuit> circuit = parser.parse(argv[1]);
    // auto& commands = circuit->commands();
    // std::cout << "Running commands..." << std::endl;
    // std::cout << commands.size() << " commands found." << std::endl;
    // for (const auto& command : commands) {
    //     if (!command) continue;
    //     std::cout << "Running command: " << command->name() << std::endl;
    //     command->run();
    // }
    // circuit->stamp_circuit();
    // auto circuit_matrix = circuit->get_matrix();
    // std::cout << circuit_matrix << std::endl;
    // auto output_matrix = circuit->get_output_matrix();
    // std::cout << output_matrix << std::endl;
    // auto output = Matrix<double>::solve_matrix(circuit_matrix, output_matrix);
    // std::cout << output << std::endl;
}