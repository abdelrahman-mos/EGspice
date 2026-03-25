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

std::unique_ptr<Capacitor> Parser::parseCapacitor(const std::string& line) {
    std::istringstream iss(line);
    std::string name, t1, t2, str_value;
    iss >> name >> t1 >> t2 >> str_value;
    double value = value_to_double(str_value);
    std::cout << "Parsed Resistor: " << name << " " << t1 << " " << t2 << " " << value << std::endl;
    return std::unique_ptr<Capacitor>(new Capacitor({t1, t2}, name, value));
}

std::unique_ptr<Inductor> Parser::parseInductor(const std::string& line) {
    static int inductor_id = 0;
    std::istringstream iss(line);
    std::string name, t1, t2, str_value;
    iss >> name >> t1 >> t2 >> str_value;
    double value = value_to_double(str_value);
    std::cout << "Parsed Resistor: " << name << " " << t1 << " " << t2 << " " << value << std::endl;
    return std::unique_ptr<Inductor>(new Inductor({t1, t2}, inductor_id++, name, value));
}

std::unique_ptr<Command> Parser::parseAC(std::istringstream& iss) {
    std::string type_str, numpoints_str, fstart_str, fend_str;
    iss >> type_str >> numpoints_str >> fstart_str >> fend_str;
    AC_TYPE type;
    if (type_str == "dec") {
        type = AC_TYPE::DEC;
    } else if (type_str == "oct")
    {
        type = AC_TYPE::OCT;
    } else if (type_str == "lin")
    {
        type = AC_TYPE::LIN;
    } else {
        std::cout << "unknown ac type " << type_str << std::endl;
        return nullptr;
    }

    int numpoints = value_to_double(numpoints_str);
    double fstart = value_to_double(fstart_str);
    double fend = value_to_double(fend_str);
    return std::unique_ptr<AC>(new AC("ac", fstart, fend, numpoints, type));
}

std::unique_ptr<Command> Parser::parseCommand(const std::string& line) {
    std::istringstream iss(line);
    std::string command;
    iss >> command;
    command.erase(0, 1);
    if (command == "op") {
        return std::unique_ptr<OP>(new OP("op"));
    } else if (command == "ac")
    {
        return parseAC(iss);
    }
    std::cout << "Unsupported command " << command << std::endl;
    return nullptr;
}

std::shared_ptr<Circuit> Parser::parse(std::string filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Could not open file: " << filename << std::endl;
        return nullptr;
    }
    // std::vector<std::unique_ptr<Component>> components;
    std::shared_ptr<Circuit> circuit(new Circuit());
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

        if (line[0] == '.') {
            auto command = parseCommand(line);
            if (command) {
                circuit->add_command(std::move(command));
            }
            continue;
        }

        std::unique_ptr<Component> component = nullptr;
        if (line[0] == 'v') {
            component = parseVsource(line);
        } else if (line[0] == 'i') {
            component = parseIsource(line);
        } else if (line[0] == 'r') {
            component = parseResistor(line);
        } else if (line[0] == 'c') {
            component = parseCapacitor(line);
        } else if (line[0] == 'l') {
            component = parseInductor(line);
        } else {
            std::cout << "Unsupported component " << line[0] << std::endl;
        }

        if (component) {
            circuit->add_component(std::move(component));
        }
    }
    file.close();
    return circuit;
}