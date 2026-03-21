#include "../include/Parser.hpp"

std::unique_ptr<Vsource> Parser::parseVsource(const std::string& line) {
    std::istringstream iss(line);
    std::string name;
    int t1, t2;
    double value;
    iss >> name >> t1 >> t2 >> value;
    std::cout << "Parsed Vsource: " << name << " " << t1 << " " << t2 << " " << value << std::endl;
    return std::unique_ptr<Vsource>(new Vsource({t1, t2}, name, value));
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
        while(line.begin() != line.end() && std::isspace(*line.begin())) {
            line.erase(line.begin());
        }
        if (line.empty() || line[0] == '*') {
            continue; // Skip comments and empty lines
        }
        if (line[0] == 'v' || line[0] == 'V') {
            circuit->add_component(parseVsource(line));
        } else if (line[0] == 'r' || line[0] == 'R') {
            circuit->add_component(parseResistor(line));
        }
        else {
            std::cout << "Unsupported component " << line[0];
        }
    }
    file.close();
    return circuit;
}