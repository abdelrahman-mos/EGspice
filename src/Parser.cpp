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

std::shared_ptr<Vsource> Parser::parseVsource(const std::string& line, int& vsource_id) {
    std::istringstream iss(line);
    std::string name, t1, t2, str_value;
    iss >> name >> t1 >> t2 >> str_value;
    double value = value_to_double(str_value);
    logger_->log(LogLevel::INFO, "Parsed Vsource: " + name + " " + t1 + " " + t2 + " " + str_value);
    return std::shared_ptr<Vsource>(new Vsource({t1, t2}, vsource_id++, name, value));
}

std::shared_ptr<Isource> Parser::parseIsource(const std::string& line) {
    std::istringstream iss(line);
    std::string name, t1, t2, str_value;
    iss >> name >> t1 >> t2 >> str_value;
    double value = value_to_double(str_value);
    logger_->log(LogLevel::INFO, "Parsed Isource: " + name + " " + t1 + " " + t2 + " " + str_value);
    return std::shared_ptr<Isource>(new Isource({t1, t2}, name, value));
}

std::shared_ptr<Resistor> Parser::parseResistor(const std::string& line) {
    std::istringstream iss(line);
    std::string name, t1, t2, str_value;
    iss >> name >> t1 >> t2 >> str_value;
    double value = value_to_double(str_value);
    logger_->log(LogLevel::INFO, "Parsed Resistor: " + name + " " + t1 + " " + t2 + " " + str_value);
    return std::shared_ptr<Resistor>(new Resistor({t1, t2}, name, value));
}

std::shared_ptr<Capacitor> Parser::parseCapacitor(const std::string& line) {
    std::istringstream iss(line);
    std::string name, t1, t2, str_value;
    iss >> name >> t1 >> t2 >> str_value;
    double value = value_to_double(str_value);
    logger_->log(LogLevel::INFO, "Parsed Capacitor: " + name + " " + t1 + " " + t2 + " " + str_value);
    return std::shared_ptr<Capacitor>(new Capacitor({t1, t2}, name, value));
}

std::shared_ptr<Inductor> Parser::parseInductor(const std::string& line) {
    static int inductor_id = 0;
    std::istringstream iss(line);
    std::string name, t1, t2, str_value;
    iss >> name >> t1 >> t2 >> str_value;
    double value = value_to_double(str_value);
    logger_->log(LogLevel::INFO, "Parsed Inductor: " + name + " " + t1 + " " + t2 + " " + str_value);
    return std::shared_ptr<Inductor>(new Inductor({t1, t2}, inductor_id++, name, value));
}

std::shared_ptr<VCCS> Parser::parseVCCS(const std::string& line) {
    std::istringstream iss(line);
    std::string name, t1, t2, t3, t4, str_value;
    iss >> name >> t1 >> t2 >> t3 >> t4 >> str_value;
    double value = value_to_double(str_value);
    logger_->log(LogLevel::INFO, "Parsed VCCS: " + name + " " + t1 + " " + t2 + " " + t3 + " " + t4 + " " + str_value);
    return std::shared_ptr<VCCS>(new VCCS({t1, t2, t3, t4}, name, value));
}

std::shared_ptr<CCCS> Parser::parseCCCS(const std::string& line, int& cccs_id) {
    std::istringstream iss(line);
    std::string name, t1, t2, t3, t4, str_value;
    iss >> name >> t1 >> t2 >> t3 >> t4 >> str_value;
    double value = value_to_double(str_value);
    logger_->log(LogLevel::INFO, "Parsed CCCS: " + name + " " + t1 + " " + t2 + " " + t3 + " " + t4 + " " + str_value);
    return std::shared_ptr<CCCS>(new CCCS({t1, t2, t3, t4}, cccs_id++, name, value));
}

std::shared_ptr<VCVS> Parser::parseVCVS(const std::string& line, int& vcvs_id) {
    std::istringstream iss(line);
    std::string name, t1, t2, t3, t4, str_value;
    iss >> name >> t1 >> t2 >> t3 >> t4 >> str_value;
    double value = value_to_double(str_value);
    logger_->log(LogLevel::INFO, "Parsed VCVS: " + name + " " + t1 + " " + t2 + " " + t3 + " " + t4 + " " + str_value);
    return std::shared_ptr<VCVS>(new VCVS({t1, t2, t3, t4}, vcvs_id++, name, value));
}

std::shared_ptr<CCVS> Parser::parseCCVS(const std::string& line, int& ccvs_id) {
    std::istringstream iss(line);
    std::string name, t1, t2, t3, t4, str_value;
    iss >> name >> t1 >> t2 >> t3 >> t4 >> str_value;
    double value = value_to_double(str_value);
    logger_->log(LogLevel::INFO, "Parsed CCVS: " + name + " " + t1 + " " + t2 + " " + t3 + " " + t4 + " " + str_value);
    return std::shared_ptr<CCVS>(new CCVS({t1, t2, t3, t4}, ccvs_id++, name, value));
}

std::shared_ptr<SubcktInstance> Parser::parseSubcktInstance(const std::string& line) {
    std::istringstream iss(line);
    std::string curr;
    std::vector<std::string> subckt_definition;
    while(iss >> curr) {
        subckt_definition.push_back(curr);
    }
    std::vector<std::string> terminals;
    size_t subckt_definition_size = subckt_definition.size();
    for (size_t i = 1; i < subckt_definition_size-1; i++) {
        terminals.push_back(subckt_definition[i]);
    }
    std::string name = subckt_definition[0];
    std::string subckt_name = subckt_definition[subckt_definition_size-1];
    std::string message = "Parsed Subckt Instance: " + name + " ";
    for (auto& terminal : terminals) {
        message += terminal + " ";
    }
    message += subckt_name;
    logger_->log(LogLevel::INFO, message);
    return std::shared_ptr<SubcktInstance>(new SubcktInstance(name, terminals, subckt_name));
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
        logger_->log(LogLevel::ERROR, "Unknown AC type " + type_str);
        return nullptr;
    }

    int numpoints = value_to_double(numpoints_str);
    double fstart = value_to_double(fstart_str);
    double fend = value_to_double(fend_str);
    return std::unique_ptr<AC>(new AC("ac", fstart, fend, numpoints, type, logger_));
}

bool Parser::is_subckt(const std::string& line) {
    std::istringstream iss(line);
    std::string command;
    iss >> command;
    if (command == ".subckt") return true;
    return false;
}

bool Parser::is_ends(const std::string& line) {
    std::istringstream iss(line);
    std::string command;
    iss >> command;
    if (command == ".ends") return true;
    return false;
}

std::unique_ptr<Command> Parser::parseCommand(const std::string& line) {
    std::istringstream iss(line);
    std::string command;
    iss >> command;
    command.erase(0, 1);
    if (command == "op") {
        return std::unique_ptr<OP>(new OP("op", logger_));
    } else if (command == "ac") {
        return parseAC(iss);
    }
    logger_->log(LogLevel::ERROR, "Unsupported command " + command);
    return nullptr;
}

std::vector<std::string> parseSubcktHeader(const std::string& header_line) {
    std::istringstream iss(header_line);
    std::string curr;
    std::vector<std::string> subckt_header;
    while(iss >> curr) subckt_header.push_back(curr);
    return subckt_header;
}

void Parser::parseAndAddDevice(std::shared_ptr<Circuit> circuit, const std::string& line, int& curr_id) {
    if (line[0] == 'v') {
        circuit->add_component(parseVsource(line, curr_id));
    } else if (line[0] == 'i') {
        circuit->add_component(parseIsource(line));
    } else if (line[0] == 'r') {
        circuit->add_component(parseResistor(line));
    } else if (line[0] == 'c') {
        circuit->add_component(parseCapacitor(line));
    } else if (line[0] == 'l') {
        circuit->add_component(parseInductor(line));
    } else if (line[0] == 'g') {
        circuit->add_component(parseVCCS(line));
    } else if (line[0] == 'f') {
        circuit->add_component(std::static_pointer_cast<Vsource>(parseCCCS(line, curr_id)));
    } else if (line[0] == 'e') {
        circuit->add_component(std::static_pointer_cast<Vsource>(parseVCVS(line, curr_id)));
    } else if (line[0] == 'h') {
        circuit->add_component(parseCCVS(line, curr_id));
    } else if (line[0] == 'x') {
        circuit->add_component(parseSubcktInstance(line));
    } else {
        logger_->log(LogLevel::ERROR, "Unsupported component " + line[0]);
    }
}

std::shared_ptr<Subckt> Parser::parseSubckt(std::istream& file, const std::string& curr_line) {
    std::vector<std::string> subckt_header = parseSubcktHeader(curr_line);
    std::string subckt_name = subckt_header[1];
    std::vector<std::string> terminals(subckt_header.begin()+2, subckt_header.end());
    auto subckt = std::make_shared<Subckt>(subckt_name, terminals);
    std::string line;
    int curr_id = 0;
    while(std::getline(file, line)) {
        logger_->log(line + "\n");
        line = str_tolower(line);
        if (is_ends(line)) break;
        parseAndAddDevice(subckt, line, curr_id);
    }
    return subckt;
}

std::shared_ptr<Circuit> Parser::parse(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        logger_->log(LogLevel::ERROR, "Could not open file: " + filename);
        exit(1);
    }
    parse(file);
}

std::shared_ptr<Circuit> Parser::parse(std::ifstream& file) {
    std::shared_ptr<Circuit> circuit(new Circuit());
    std::string line;
    int curr_id = 0;
    while (std::getline(file, line)) {
        logger_->log(line + "\n");
        line = str_tolower(line);
        while(line.begin() != line.end() && std::isspace(*line.begin())) {
            line.erase(line.begin());
        }
        if (line.empty() || line[0] == '*') {
            continue; // Skip comments and empty lines
        }

        if (is_subckt(line)) {
            circuit->add_subckt(parseSubckt(file, line));
            continue;
        }

        if (line[0] == '.') {
            auto command = parseCommand(line);
            if (command) {
                circuit->add_command(std::move(command));
            }
            continue;
        }

        parseAndAddDevice(circuit, line, curr_id);

    }
    file.close();
    return circuit;
}