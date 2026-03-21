#ifndef COMMAND_HPP
#define COMMAND_HPP
#include <string>
#include <vector>
#include <cmath>

class Command {
protected:
    std::string name;
    std::vector<std::string> command_options;
public:
    Command(std::string name) : name(name) {}
};

class Simulation : public Command {
public:
    Simulation(std::string name) : Command(name) {}
};

class OP : public Simulation {
public:
    OP(std::string name) : Simulation(name) {}
};

enum class AC_TYPE {
    DEC,
    OCT,
    LIN
};

class AC : public Simulation {
    double fstart;
    double fend;
    size_t numpoints;
    AC_TYPE type;
    std::vector<double> frequency_points;
    void expand_freq();
    void expand_freq_dec();
    void expand_freq_oct();
    void expand_freq_lin();
public:
    AC(std::string name, double fstart, double fend, AC_TYPE type) : Simulation(name), fstart(fstart), fend(fend), type(type) {
        expand_freq();
    }
};

#endif