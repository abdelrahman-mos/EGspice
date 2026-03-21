#ifndef PARSER_HPP
#define PARSER_HPP
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <memory>
#include "component.hpp"
#include "Circuit.hpp"

class Parser {
    std::string filename;
    std::unique_ptr<Vsource> parseVsource(const std::string& line);
    std::unique_ptr<Resistor> parseResistor(const std::string& line);
    std::unique_ptr<Isource> parseIsource(const std::string& line);
public:
    Parser(const std::string& filename) : filename(filename) {}
    std::unique_ptr<Circuit> parse();
};

#endif /* PARSER_HPP */