#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP
#include "Circuit.hpp"
#include "Parser.hpp"
#include "InputParser.hpp"
#include "InputData.hpp"

class Simulator {
    std::unordered_map<std::string, std::string> inputs;
    std::shared_ptr<Circuit> circuit;
    Parser parser = Parser();
    
public:
    Simulator(int argc, char* argv[]) {
        inputs = InputParser::parse_inputs(argc, argv);
    }

    void run() {
        circuit = parser.parse(inputs.at(InputDataNames::INPUT_NETLIST));
        auto& commands = circuit->commands();
        for (const auto& command : commands) {
            if (!command) continue;
            command->run(circuit);
        }
    }
};

#endif /* SIMULATOR_HPP */