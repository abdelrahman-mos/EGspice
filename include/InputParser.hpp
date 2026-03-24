#ifndef INPUTPARSER_HPP
#define INPUTPARSER_HPP

#include <iostream>
#include <unordered_map>
#include <string>
#include "InputData.hpp"

class InputParser {
public:
    InputParser() = default;
    static std::unordered_map<std::string, std::string> parse_inputs(int argc, char* argv[]) {
        std::unordered_map<std::string, std::string> inputs;
        if (argc < 2) {
            std::cout << "Please provide a netlist file as an argument." << std::endl;
            exit(1);
        }
        inputs[InputDataNames::INPUT_NETLIST] = argv[1];
        return inputs;
    }
};


#endif /* INPUTPARSER_HPP */