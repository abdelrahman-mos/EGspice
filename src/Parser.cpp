#include "../include/Parser.hpp"
#include <algorithm>

std::string str_tolower(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c){ return std::tolower(c); } // correct
                  );
    return s;
}

std::vector<std::string> split(std::string str, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while((pos_end = str.find(delimiter, pos_start)) != std::string::npos) {
        token = str.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(str.substr(pos_start));
    return res;
}

double value_to_double(std::string str_value) {
    // to be pre-compiled only once
    static std::regex atto_amp_reg = std::regex("[Aa]+", std::regex_constants::extended);
    static std::regex femto_farrad_reg = std::regex("[Ff]+", std::regex_constants::extended);
    static std::regex postfix_reg = std::regex("(Amp|H|V|OHM)$", std::regex_constants::extended | std::regex_constants::icase);
    static std::regex meg_reg = std::regex("(MEG)", std::regex_constants::extended | std::regex_constants::icase);
    static std::regex tera_reg = std::regex("(T)", std::regex_constants::extended | std::regex_constants::icase);
    static std::regex giga_reg = std::regex("(G)", std::regex_constants::extended | std::regex_constants::icase);
    static std::regex kilo_reg = std::regex("(K)", std::regex_constants::extended | std::regex_constants::icase);
    static std::regex milli_reg = std::regex("(m)", std::regex_constants::extended | std::regex_constants::icase);
    static std::regex micro_reg = std::regex("(u)", std::regex_constants::extended | std::regex_constants::icase);
    static std::regex nano_reg = std::regex("(n)", std::regex_constants::extended | std::regex_constants::icase);
    static std::regex pico_reg = std::regex("(p)", std::regex_constants::extended | std::regex_constants::icase);
    static std::regex femto_reg = std::regex("(f)", std::regex_constants::extended | std::regex_constants::icase);
    static std::regex atto_reg = std::regex("(a)", std::regex_constants::extended | std::regex_constants::icase);
    std::string output = std::regex_replace(str_value, atto_amp_reg, "a");
    output = std::regex_replace(output, femto_farrad_reg, "f");
    output = std::regex_replace(output, postfix_reg, "");
    output = std::regex_replace(output, meg_reg, "e6");
    output = std::regex_replace(output, tera_reg, "e12");
    output = std::regex_replace(output, giga_reg, "e9");
    output = std::regex_replace(output, kilo_reg, "e3");
    output = std::regex_replace(output, milli_reg, "e-3");
    output = std::regex_replace(output, micro_reg, "e-6");
    output = std::regex_replace(output, nano_reg, "e-9");
    output = std::regex_replace(output, pico_reg, "e-12");
    output = std::regex_replace(output, femto_reg, "e-15");
    output = std::regex_replace(output, atto_reg, "e-18");
    double value = std::stod(output);
    return value;
}

std::shared_ptr<Vsource> Parser::parseVsource(const std::string& line, int& vsource_id) {
    std::istringstream iss(line);
    std::string name, t1, t2, str_value;
    std::string temp;
    std::vector<std::string> line_split;
    double value, ac_val = 0.0;
    bool found_val = false;
    while (iss >> temp) {
        line_split.push_back(temp);
    }
    // iss >> name >> t1 >> t2 >> str_value;
    size_t line_size = line_split.size();
    if ((line_size < 4) || (line_size > 6)) {
        std::string message = "Incorrect Voltage Source definition: ";
        for (auto& curr : line_split) message += curr + " ";
        logger_->log(LogLevel::ERROR, message);
        throw std::runtime_error(message);
    }
    name = line_split[0];
    t1 = line_split[1];
    t2 = line_split[2];
    for (size_t i = 3; i < line_split.size(); i++) {
        auto val_split = split(line_split[i], "=");
        if ((val_split.size() == 1) && (i == 3)) {
            value = value_to_double(val_split[0]);
            found_val = true;
            continue;
        } else if (val_split.size() != 2) {
            std::string message = "Incorrect Voltage Source parameter: " + line_split[i];
            logger_->log(LogLevel::ERROR, message);
            throw std::runtime_error(message);
        }

        if (val_split[0] == "dc") {
            value = value_to_double(val_split[1]);
            found_val = true;
        } else if (val_split[0] == "ac") {
            ac_val = value_to_double(val_split[1]);
        }
    }
    std::string message = "Parsed Vsource: ";
    for (auto& curr : line_split) message += curr + " ";
    logger_->log(LogLevel::INFO, message);
    return std::shared_ptr<Vsource>(new Vsource({t1, t2}, vsource_id++, ac_val, name, value));
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

std::unique_ptr<AC> Parser::parseAC(std::istringstream& iss) {
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

std::unique_ptr<DC> Parser::parseDC(std::istringstream& iss) {
    std::vector<std::string> split_command;
    std::string curr;
    while (iss >> curr) split_command.push_back(curr);
    size_t cmd_size = split_command.size();
    if ((cmd_size < 4) || ((cmd_size > 4) && (cmd_size < 8)) || (cmd_size > 8)) {
        std::string msg = "Incorrect DC command: ";
        for (auto& curr_cmd : split_command) msg += curr_cmd;
        logger_->log(LogLevel::ERROR, msg);
        return nullptr;
    }

    std::string outer_name = split_command[0];
    double outer_start = value_to_double(split_command[1]);
    double outer_end = value_to_double(split_command[2]);
    double outer_step = value_to_double(split_command[3]);
    if (cmd_size == 4) {
        logger_->log(LogLevel::INFO, "Parsed DC command: " + split_command[0] + " " + split_command[1] + " " + split_command[2] 
            + " " + split_command[3]);
        return std::unique_ptr<DC>(new DC("dc", outer_start, outer_end, outer_step, outer_name, logger_));
    } else {
        std::string inner_name = split_command[4];
        double inner_start = value_to_double(split_command[5]);
        double inner_end = value_to_double(split_command[6]);
        double inner_step = value_to_double(split_command[7]);
        logger_->log(LogLevel::INFO, "Parsed DC command: " + split_command[0] + " " + split_command[1] + " " + split_command[2] + " " 
            + split_command[3] + " " + split_command[4] + " " + split_command[5] + " " + split_command[6] + " " + split_command[7]);
        return std::unique_ptr<DC>( new DC("dc", outer_start, outer_end, outer_step, outer_name, logger_, 
            inner_start, inner_end, inner_step, inner_name));
    }
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
    } else if (command == "dc") {
        return parseDC(iss);
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
    auto subckt = std::make_shared<Subckt>(subckt_name, terminals, logger_);
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
    return parse(file);
}

std::shared_ptr<Circuit> Parser::parse(std::ifstream& file) {
    static std::regex equal_reg(R"(\s*=\s*)");
    std::shared_ptr<Circuit> circuit(new Circuit(logger_));
    std::string line;
    int curr_id = 0;
    while (std::getline(file, line)) {
        logger_->log(line + "\n");
        line = str_tolower(line);
        line = std::regex_replace(line, equal_reg, "=");
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
    circuit->assign_subckts();
    return circuit;
}