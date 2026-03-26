#ifndef INPUTPARSER_HPP
#define INPUTPARSER_HPP

#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <stdexcept>
#include <optional>
#include <filesystem>
#include "InputData.hpp"

class InputParser {
public:
    InputParser() = default;
    static std::unordered_map<std::string, std::string> parse_inputs(int argc, char* argv[]) {
        std::vector<std::string> inputs;
        std::unordered_map<std::string, std::string> output;
        if (argc < 2) {
            throw std::invalid_argument("No input file provided");
        }
        for (size_t i = 1; i < argc; i++) {
            inputs.push_back(argv[i]);
        }
        
        for (size_t i = 0; i < inputs.size(); i++) {
            const std::string& curr_input = inputs[i];

            if (curr_input == "-o") {
                if ((i+1 < inputs.size()) && (inputs[i+1][0] != '-')) {
                    output[InputDataNames::OUTPUT_FILE_NAME]  = inputs[++i];
                } else {
                    throw std::invalid_argument("-o requires an output filename.");
                }
            } else if (curr_input == "-l") {
                if ((i+1 < inputs.size()) && (inputs[i+1][0] != '-')) {
                    output[InputDataNames::OUTPUT_LOG_FILE]  = inputs[++i];
                } else {
                    throw std::invalid_argument("-l requires a log filename.");
                }
            } else if (curr_input[0] != '-') {
                if (output.find(InputDataNames::INPUT_NETLIST) == output.end()) {
                    output[InputDataNames::INPUT_NETLIST] = curr_input;
                } else {
                    throw std::invalid_argument("Too many input files: " + curr_input);
                }
            } else {
                throw std::invalid_argument("Unknown argument: " + curr_input);
            }
        }

        if (output.find(InputDataNames::INPUT_NETLIST) == output.end()) {
            throw std::invalid_argument("Missing required input file");
        }

        std::string input_netlist = output[InputDataNames::INPUT_NETLIST];
        
        if (!std::filesystem::exists(input_netlist)) {
            throw std::runtime_error("Error: input netlist " + input_netlist + " does not exist.");
        }

        if (!std::filesystem::is_regular_file(input_netlist)) {
            throw std::runtime_error("Error: input netlist " + input_netlist + " is not a file.");
        }

        if (output.find(InputDataNames::OUTPUT_LOG_FILE) == output.end()) {
            std::filesystem::path p(input_netlist);
            output[InputDataNames::OUTPUT_LOG_FILE] = p.stem().string() + ".log";
        }

        if (output.find(InputDataNames::OUTPUT_FILE_NAME) == output.end()) {
            std::filesystem::path p(input_netlist);
            output[InputDataNames::OUTPUT_FILE_NAME] = p.stem().string();
        }
        return output;
    }
};


#endif /* INPUTPARSER_HPP */