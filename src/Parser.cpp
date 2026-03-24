#include "../include/Parser.hpp"
#include <algorithm>

std::string str_tolower(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c){ return std::tolower(c); } // correct
                  );
    return s;
}

double value_to_double(std::string str_value) {
    std::string output = std::regex_replace(str_value, std::regex("[Aa]+", std::regex_constants::extended), "a");
    output = std::regex_replace(output, std::regex("[Ff]+", std::regex_constants::extended), "f");
    output = std::regex_replace(output, std::regex("(Amp|H|V|OHM)$", std::regex_constants::extended | std::regex_constants::icase), "");
    output = std::regex_replace(output, std::regex("(MEG)", std::regex_constants::extended | std::regex_constants::icase), "e6");
    output = std::regex_replace(output, std::regex("(T)", std::regex_constants::extended | std::regex_constants::icase), "e12");
    output = std::regex_replace(output, std::regex("(G)", std::regex_constants::extended | std::regex_constants::icase), "e9");
    output = std::regex_replace(output, std::regex("(T)", std::regex_constants::extended | std::regex_constants::icase), "e12");
    output = std::regex_replace(output, std::regex("(K)", std::regex_constants::extended | std::regex_constants::icase), "e3");
    output = std::regex_replace(output, std::regex("(m)", std::regex_constants::extended | std::regex_constants::icase), "e-3");
    output = std::regex_replace(output, std::regex("(u)", std::regex_constants::extended | std::regex_constants::icase), "e-6");
    output = std::regex_replace(output, std::regex("(n)", std::regex_constants::extended | std::regex_constants::icase), "e-9");
    output = std::regex_replace(output, std::regex("(p)", std::regex_constants::extended | std::regex_constants::icase), "e-12");
    output = std::regex_replace(output, std::regex("(f)", std::regex_constants::extended | std::regex_constants::icase), "e-15");
    output = std::regex_replace(output, std::regex("(a)", std::regex_constants::extended | std::regex_constants::icase), "e-18");
    double value = std::stod(output);
    return value;
}

std::unique_ptr<Vsource> Parser::parseVsource(const std::string& line) {
    static int vsource_id = 0;
    std::istringstream iss(line);
    std::string name, t1, t2, str_value;
    iss >> name >> t1 >> t2 >> str_value;
    double value = value_to_double(str_value);
    std::cout << "Parsed Vsource: " << name << " " << t1 << " " << t2 << " " << value << std::endl;
    return std::unique_ptr<Vsource>(new Vsource({t1, t2}, vsource_id++, name, value));
}

std::unique_ptr<Isource> Parser::parseIsource(const std::string& line) {
    std::istringstream iss(line);
    std::string name, t1, t2, str_value;
    iss >> name >> t1 >> t2 >> str_value;
    double value = value_to_double(str_value);
    std::cout << "Parsed Isource: " << name << " " << t1 << " " << t2 << " " << value << std::endl;
    return std::unique_ptr<Isource>(new Isource({t1, t2}, name, value));
}

std::unique_ptr<Resistor> Parser::parseResistor(const std::string& line) {
    std::istringstream iss(line);
    std::string name, t1, t2, str_value;
    iss >> name >> t1 >> t2 >> str_value;
    double value = value_to_double(str_value);
    std::cout << "Parsed Resistor: " << name << " " << t1 << " " << t2 << " " << value << std::endl;
    return std::unique_ptr<Resistor>(new Resistor({t1, t2}, name, value));
}

std::unique_ptr<Command> Parser::parseCommand(const std::string& line) {
    std::istringstream iss(line);
    std::string command;
    iss >> command;
    command.erase(0, 1);
    std::cout << "Unsupported command " << command << std::endl;
    return nullptr;
}

std::unique_ptr<Circuit> Parser::parse(std::string filename) {
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
        if (line[0] == 'v') {
            circuit->add_component(parseVsource(line));
        } else if (line[0] == 'i') {
            circuit->add_component(parseIsource(line));
        } else if (line[0] == 'r') {
            circuit->add_component(parseResistor(line));
        } else if (line[0] == '.') {
            circuit->add_command(parseCommand(line));
        } else {
            std::cout << "Unsupported component " << line[0] << std::endl;
        }
    }
    file.close();
    return circuit;
}