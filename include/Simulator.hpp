#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP
#include "Circuit.hpp"
#include "Parser.hpp"
#include "InputParser.hpp"
#include "InputData.hpp"
#include "Logger.hpp"

class Simulator {
    std::unordered_map<std::string, std::string> inputs;
    std::shared_ptr<Circuit> circuit;
    std::shared_ptr<Matrix<double>> circuit_matrix;
    std::shared_ptr<Matrix<double>> output_matrix;
    std::shared_ptr<Matrix<std::complex<double>>> circuit_matrix_ac;
    std::shared_ptr<Matrix<std::complex<double>>> output_matrix_ac;
    std::unique_ptr<Parser> parser;
    std::shared_ptr<Logger> logger_;
public:
    Simulator(int argc, char* argv[]) {
        inputs = InputParser::parse_inputs(argc, argv);
        for (auto curr_input : inputs) {
            std::cout << curr_input.first << ": " << curr_input.second << std::endl;
        }
        logger_ = std::make_shared<Logger>(inputs[InputDataNames::OUTPUT_LOG_FILE]);
        parser = std::make_unique<Parser>(logger_);
        logger_->log(LogLevel::INFO, "EGspice: Egyptian SPICE simulator");
        std::string run_command = argv[0];
        for (size_t i = 1; i < argc; i++) run_command = run_command + " " + argv[i];
        logger_->log(LogLevel::INFO, "Run command: " + run_command);
    }

    void run() {
        circuit = parser->parse(inputs.at(InputDataNames::INPUT_NETLIST));
        circuit->flatten();
        auto& commands = circuit->commands();
        for (const auto& command : commands) {
            if (!command) continue;
            logger_->log(LogLevel::INFO, "running command " + command->name());
            if (typeid(*command) == typeid(AC)) {
                command->run(circuit, circuit_matrix_ac, output_matrix_ac);
            } else {
                command->run(circuit, circuit_matrix, output_matrix);
            }
        }
    }
};

#endif /* SIMULATOR_HPP */