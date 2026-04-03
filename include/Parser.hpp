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
#include "Subckt.hpp"
#include <regex>

struct componentData {
    std::string name;
    std::vector<std::string> terminals;
    double value;
    double ac_val;
};

class Parser {
    componentData parseComponentData(const std::string& line, int num_terminals);
    std::shared_ptr<Vsource> parseVsource(const std::string& line);
    std::shared_ptr<Isource> parseIsource(const std::string& line);
    std::shared_ptr<Resistor> parseResistor(const std::string& line);
    std::shared_ptr<Capacitor> parseCapacitor(const std::string& line);
    std::shared_ptr<Inductor> parseInductor(const std::string& line);
    std::shared_ptr<VCCS> parseVCCS(const std::string& line);
    std::shared_ptr<CCCS> parseCCCS(const std::string& line);
    std::shared_ptr<VCVS> parseVCVS(const std::string& line);
    std::shared_ptr<CCVS> parseCCVS(const std::string& line);
    std::shared_ptr<SubcktInstance> parseSubcktInstance(const std::string& line);
    std::unique_ptr<Command> parseCommand(const std::string& line);
    std::unique_ptr<AC> parseAC(std::istringstream& iss);
    std::unique_ptr<DC> parseDC(std::istringstream& iss);
    std::shared_ptr<Subckt> parseSubckt(std::istream& file, const std::string& curr_line);
    void parseAndAddDevice(std::shared_ptr<Circuit> circuit, const std::string& line);
    std::shared_ptr<Circuit> parse(std::ifstream& file);
    bool is_subckt(const std::string& line);
    bool is_ends(const std::string& line);
    std::shared_ptr<Logger> logger_;
public:
    Parser(std::shared_ptr<Logger> logger) : logger_(logger) {}
    std::shared_ptr<Circuit> parse(const std::string& filename);
};

#endif /* PARSER_HPP */