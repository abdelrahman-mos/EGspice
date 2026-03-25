#ifndef COMMAND_HPP
#define COMMAND_HPP
#include <string>
#include <vector>
#include <cmath>
#include <memory>

class Circuit;

class Command {
protected:
    std::string name_;
    std::vector<std::string> command_options;
public:
    Command() = default;
    Command(std::string name) : name_(name) {}
    Command(std::string name, std::vector<std::string> command_options) : name_(name), command_options(command_options) {}

    std::string name() const {
        return name_;
    }
    
    virtual void run(std::shared_ptr<Circuit> circuit) {
        return;
    }
};

class Simulation : public Command {
public:
    Simulation(std::string name) : Command(name) {}
};

class OP : public Simulation {
public:
    OP(std::string name) : Simulation(name) {}
    void run(std::shared_ptr<Circuit> circuit) override;
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
    AC(std::string name, double fstart, double fend, size_t numpoints, AC_TYPE type) : Simulation(name), fstart(fstart),
     fend(fend), numpoints(numpoints), type(type) {
        expand_freq();
    }
    void run(std::shared_ptr<Circuit> circuit) override;
};

#endif