#ifndef PARSER_HPP
#define PARSER_HPP
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <memory>
#include "component.hpp"
#include "Command.hpp"
#include "Circuit.hpp"
#include <regex>

class Parser {
    std::string filename;
    std::unique_ptr<Vsource> parseVsource(const std::string& line);
    std::unique_ptr<Resistor> parseResistor(const std::string& line);
    std::unique_ptr<Isource> parseIsource(const std::string& line);
    std::unique_ptr<Capacitor> parseCapacitor(const std::string& line);
    std::unique_ptr<Inductor> parseInductor(const std::string& line);
    std::unique_ptr<Command> parseCommand(const std::string& line);
    std::unique_ptr<Command> parseAC(std::istringstream& iss);
public:
    Parser() = default;
    std::shared_ptr<Circuit> parse(std::string filename);
};

#endif /* PARSER_HPP */