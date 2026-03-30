#ifndef SUBCKT_HPP
#define SUBCKT_HPP

#include "Circuit.hpp"

class Subckt : public Circuit 
{
    std::string subckt_name;
    std::vector<std::shared_ptr<Component>> components_;
    std::vector<std::shared_ptr<Vsource>> vsources_;
    std::vector<std::shared_ptr<Inductor>> inductors_;
    std::vector<std::string> terminal_names;
public:
    Subckt(std::string name, std::vector<std::string> terminals) : Circuit(), subckt_name(name), terminal_names(terminals) {};
};


#endif /* SUBCKT_HPP */