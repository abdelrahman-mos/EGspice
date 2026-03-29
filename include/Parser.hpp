#ifndef PARSER_HPP
#define PARSER_HPP
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <memory>
#include "component.hpp"
#include "Command.hpp"
#include "Circuit.hpp"
#include "Logger.hpp"
#include <regex>

class Parser {
    std::string filename;
    std::shared_ptr<Vsource> parseVsource(const std::string& line, int& vsource_id);
    std::shared_ptr<Resistor> parseResistor(const std::string& line);
    std::shared_ptr<Isource> parseIsource(const std::string& line);
    std::shared_ptr<Capacitor> parseCapacitor(const std::string& line);
    std::shared_ptr<Inductor> parseInductor(const std::string& line);
    std::shared_ptr<VCCS> parseVCCS(const std::string& line);
    std::shared_ptr<CCCS> parseCCCS(const std::string& line, int& cccs_id);
    std::unique_ptr<Command> parseCommand(const std::string& line);
    std::unique_ptr<Command> parseAC(std::istringstream& iss);
    std::shared_ptr<Logger> logger_;
public:
    Parser(std::shared_ptr<Logger> logger) : logger_(logger) {}
    std::shared_ptr<Circuit> parse(std::string filename);
};

#endif /* PARSER_HPP */