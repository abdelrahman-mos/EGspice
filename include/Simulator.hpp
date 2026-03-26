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
        auto& commands = circuit->commands();
        for (const auto& command : commands) {
            if (!command) continue;
            std::cout << "running command " << command->name() << std::endl;
            command->run(circuit);
        }
    }
};

#endif /* SIMULATOR_HPP */