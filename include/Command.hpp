#ifndef COMMAND_HPP
#define COMMAND_HPP
#include <string>
#include <vector>

class Command {
protected:
    std::string name;
    std::vector<std::string> command_options;
public:
    Command() = default;
    Command(std::string name) : name(name) {}
};

class Simulation : public Command {
public:
    Simulation() : Command() {}
    Simulation(std::string name) : Command(name) {}
};

class OP : public Simulation {
public:
    OP() : Simulation() {}
    OP(std::string name) : Simulation(name) {}
};

class AC : public Simulation {
    double fstart;
    double fend;
    double fstep;
public:
    AC() : Simulation() {}
    AC(std::string name) : Simulation(name) {}
};

#endif