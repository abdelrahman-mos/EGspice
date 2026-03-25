#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP
#include "Circuit.hpp"
#include "Parser.hpp"
#include "InputParser.hpp"
#include "InputData.hpp"

class Simulator {
    std::unordered_map<std::string, std::string> inputs;
    std::shared_ptr<Circuit> circuit;
    Parser parser;
    
public:
    Simulator(int argc, char* argv[]) {
        inputs = InputParser::parse_inputs(argc, argv);
        for (auto curr_input : inputs) {
            std::cout << curr_input.first << ": " << curr_input.second << std::endl;
        }
        parser = Parser();
    }

    void run() {
        circuit = parser.parse(inputs.at(InputDataNames::INPUT_NETLIST));
        auto& commands = circuit->commands();
        for (const auto& command : commands) {
            if (!command) continue;
            std::cout << "running command " << command->name() << std::endl;
            command->run(circuit);
        }
    }
};

#endif /* SIMULATOR_HPP */