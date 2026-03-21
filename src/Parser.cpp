#include "../include/Parser.hpp"
#include <algorithm>

std::string str_tolower(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c){ return std::tolower(c); } // correct
                  );
    return s;
}

std::unique_ptr<Vsource> Parser::parseVsource(const std::string& line) {
    std::istringstream iss(line);
    std::string name;
    int t1, t2;
    double value;
    iss >> name >> t1 >> t2 >> value;
    std::cout << "Parsed Vsource: " << name << " " << t1 << " " << t2 << " " << value << std::endl;
    return std::unique_ptr<Vsource>(new Vsource({t1, t2}, name, value));
}

std::unique_ptr<Isource> Parser::parseIsource(const std::string& line) {
    std::istringstream iss(line);
    std::string name;
    int t1, t2;
    double value;
    iss >> name >> t1 >> t2 >> value;
    std::cout << "Parsed Isource: " << name << " " << t1 << " " << t2 << " " << value << std::endl;
    return std::unique_ptr<Isource>(new Isource({t1, t2}, name, value));
}

std::unique_ptr<Resistor> Parser::parseResistor(const std::string& line) {
    std::istringstream iss(line);
    std::string name;
    int t1, t2;
    double value;
    iss >> name >> t1 >> t2 >> value;
    std::cout << "Parsed Resistor: " << name << " " << t1 << " " << t2 << " " << value << std::endl;
    return std::unique_ptr<Resistor>(new Resistor({t1, t2}, name, value));
}

std::unique_ptr<Circuit> Parser::parse() {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Could not open file: " << filename << std::endl;
        return nullptr;
    }
    // std::vector<std::unique_ptr<Component>> components;
    std::unique_ptr<Circuit> circuit(new Circuit());
    std::string line;
    while (std::getline(file, line)) {
        std::cout << line << std::endl;
        line = str_tolower(line);
        while(line.begin() != line.end() && std::isspace(*line.begin())) {
            line.erase(line.begin());
        }
        if (line.empty() || line[0] == '*') {
            continue; // Skip comments and empty lines
        }
        if (line[0] == 'v' || line[0] == 'V') {
            circuit->add_component(parseVsource(line));
        } else if (line[0] == 'i' || line[0] == 'I') {
            circuit->add_component(parseIsource(line));
        } else if (line[0] == 'r' || line[0] == 'R') {
            circuit->add_component(parseResistor(line));
        } else if (line[0] == '.') {
            std::istringstream iss(line);
            std::string command;
            iss >> command;
            command.erase(0, 1);
            std::cout << "Unsupported command " << command << std::endl;
        } else {
            std::cout << "Unsupported component " << line[0] << std::endl;
        }
    }
    file.close();
    return circuit;
}